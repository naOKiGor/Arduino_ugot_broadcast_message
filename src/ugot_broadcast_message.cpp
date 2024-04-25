#include "ugot_broadcast_message.h"

#if defined(__ASYNC_UDP_IMPL_WIN__) || defined(__ASYNC_UDP_IMPL_LINUX__)
#define delay(t) usleep((t) * 1000)
typedef int (*printf_func_ptr)(const char* fmt, ...);
struct SerialMock {
	printf_func_ptr printf;
} Serial = {
	.printf = ::printf
};
#endif

uGotBroadcastChannel::uGotBroadcastChannel() {
	this->callback = nullptr;
	this->channel = BOARDCAST_DEFAULT_CHANNEL_ID;
	this->enable = false;
}

uGotBroadcastChannel::~uGotBroadcastChannel() {
	releaseUdp();
}

bool uGotBroadcastChannel::sendBroadcastMessage(const char* message) {
	return sendMessageImpl(message);
}

bool uGotBroadcastChannel::sendMessageTo(const char* message, const char* addr) {
	return sendMessageImpl(message, addr);
}

void uGotBroadcastChannel::setMessageReceivedCallback(std::function<void(const char*)> callback) {
	this->callback = callback;
}

bool uGotBroadcastChannel::isEnable() {
	return enable;
}

void uGotBroadcastChannel::setEnable(bool enable) {
	this->enable = enable;
	if(enable) {
		initUdp();
	} else {
		releaseUdp();
	}
}

uint16_t uGotBroadcastChannel::getPort() {
	return BOARDCAST_CHANNEL_BASEID + channel;
}

uint8_t uGotBroadcastChannel::getChannel() {
	return channel;
}

void uGotBroadcastChannel::setChannel(uint8_t channel) {
	if(channel < 0 || channel >= BOARDCAST_MAX_CHANNEL_COUNT) {
		return;
	}
	this->channel = channel;
	initUdp();
}

void uGotBroadcastChannel::initUdp() {
	if(!enable) {
		return;
	}
	releaseUdp();
	delay(1000);
	if(udp.listen(getPort())) {
		Serial.printf("UDP listening: %s:%d\n", udp.listenIP().toString().c_str(), getPort());
		udp.onPacket([this](AsyncUDPPacket packet) {
			this->onReceivedPacket(packet);
		});
	} else {
		Serial.printf("UDP listen failure\n");
	}
}

void uGotBroadcastChannel::releaseUdp() {
	udp.close();
}

uint8_t uGotBroadcastChannel::crc_checksum(uint8_t* data, size_t offset, size_t length) {
	int32_t crc = 0;
	for(int i = 0; i < length; i++) {
		crc ^= data[i + offset];
		for(int j = 0; j < 8; j++) {
			if(crc & 0x80) {
				crc = crc << 1 ^ 0x07;
			} else {
				crc = crc << 1;
			}
		}
	}
	return (crc ^ 0x55) & 0xFF;
}

bool uGotBroadcastChannel::sendMessageImpl(const char* message, const char* addr) {
	if(!enable) return false;
	
	uint8_t buffer[128] = {0};
	uint8_t payload_buffer[128] = {0};
	pb_ostream_t stream;
	bool status;
	
	broadcast_message_header header = {
		.dev = 2,
		.cmd = 2700,
		.attr = broadcast_message_header_Attr_PUSH
	};
	
	broadcast_message_payload payload;
	strncpy(payload.name, "BMSG", sizeof(payload.name));
	strncpy(payload.value, message, sizeof(payload.value));
	
	stream = pb_ostream_from_buffer(payload_buffer, sizeof(payload_buffer));
	status = pb_encode(&stream, broadcast_message_payload_fields, &payload);
	if (!status) return false;
	
	uint8_t payload_size = stream.bytes_written;
	header.dataLen = payload_size;
	
	stream = pb_ostream_from_buffer(&buffer[2], sizeof(buffer) - 2);
	status = pb_encode(&stream, broadcast_message_header_fields, &header);
	if (!status) return false;
	
	uint8_t header_size = stream.bytes_written;
	
	buffer[0] = HEADER_INDICATE;
	buffer[1] = header_size;
	buffer[2 + header_size] = crc_checksum(&buffer[2], 0, header_size);
	memcpy(&buffer[3 + header_size], payload_buffer, payload_size);
	buffer[3 + header_size + payload_size] = crc_checksum(payload_buffer, 0, payload_size);
	
	if(addr == NULL) {
		udp.broadcastTo(buffer, 4 + header_size + payload_size, getPort());
	} else {
		IPAddress target = IPAddress();
		target.fromString(addr);
		udp.writeTo(buffer, 4 + header_size + payload_size, target, getPort());
	}
	
	return true;
}

void uGotBroadcastChannel::onReceivedPacket(AsyncUDPPacket packet) {
	pb_istream_t stream;
	bool status;
	
	uint8_t* data = packet.data();
	size_t data_size = packet.length();
	
	if(data_size < 3) return;
	
	if(data[0] != HEADER_INDICATE) return;
	
	uint8_t header_size = data[1];
	
	if(data_size < 3 + header_size) return;
	
	if(crc_checksum(&data[2], 0, header_size) != data[2 + header_size]) return;
	
	broadcast_message_header header;
	stream = pb_istream_from_buffer(&data[2], header_size);
	status = pb_decode(&stream, broadcast_message_header_fields, &header);
	if (!status) return;
	
	if(header.attr != broadcast_message_header_Attr_PUSH || header.dev != 2 || header.cmd != 2700) return;
	
	size_t payload_size = header.dataLen;
	
	if(data_size < 4 + header_size + payload_size) return;
	
	if(crc_checksum(&data[3 + header_size], 0, payload_size) != data[3 + header_size + payload_size]) return;
	
	broadcast_message_payload payload;
	stream = pb_istream_from_buffer(&data[3 + header_size], payload_size);
	status = pb_decode(&stream, broadcast_message_payload_fields, &payload);
	if (!status) return;
	
	if(this->callback != nullptr) {
		this->callback(payload.value);
	}
}
