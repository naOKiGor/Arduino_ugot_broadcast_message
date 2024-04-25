#ifndef _UGOT_BROADCAST_MESSAGE_H_
#define _UGOT_BROADCAST_MESSAGE_H_

#include "AsyncUDP.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"
#include "broadcast_message.pb.h"

#define BOARDCAST_DEFAULT_CHANNEL_ID   0
#define BOARDCAST_MAX_CHANNEL_COUNT    100
#define BOARDCAST_CHANNEL_BASEID       25880

#define HEADER_INDICATE                0xDB

#if defined(__ASYNC_UDP_IMPL_WIN__)
#if defined(__WIN_EXPORT_DLL__)
#define DLL_API __declspec(dllexport)
#endif
#if defined(__WIN_IMPORT_DLL__)
#define DLL_API __declspec(dllimport)
#endif
#endif

#if !defined(DLL_API)
#define DLL_API
#endif

class DLL_API uGotBroadcastChannel
{
public:
	uGotBroadcastChannel();
	virtual ~uGotBroadcastChannel();

public:
	bool sendBroadcastMessage(const char* message);
	bool sendMessageTo(const char* message, const char* addr);
	void setMessageReceivedCallback(std::function<void(const char*)> callback);
	
	bool isEnable();
	void setEnable(bool enable);
	uint16_t getPort();
	uint8_t getChannel();
	void setChannel(uint8_t channel);
	
protected:
	void initUdp();
	void releaseUdp();
	uint8_t crc_checksum(uint8_t* data, size_t offset, size_t length);
	bool sendMessageImpl(const char* message, const char* addr = NULL);
	void onReceivedPacket(AsyncUDPPacket packet);

private:
	std::function<void(const char*)> callback;
	uint8_t channel;
	bool enable;
	AsyncUDP udp;
};

#endif
