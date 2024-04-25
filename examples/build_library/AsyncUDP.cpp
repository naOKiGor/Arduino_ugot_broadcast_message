#include "AsyncUDP.h"

bool AsyncUDP::bStartup = false;
uint32_t AsyncUDP::iStartupCounter = 0;

IPAddress::IPAddress() {}

IPAddress::IPAddress(struct sockaddr_in addr) {
    _addr = addr;
}

IPAddress::~IPAddress() {}

std::string IPAddress::toString() {
    return std::string(inet_ntoa(_addr.sin_addr));
}

void IPAddress::fromString(const char* addr) {
    struct sockaddr_in new_addr = {0};
    new_addr.sin_family = AF_INET;
    new_addr.sin_addr.s_addr = inet_addr(addr);
    _addr = new_addr;
}


AsyncUDPPacket::AsyncUDPPacket(AsyncUDPPacket &packet) {
    _data = (uint8_t*)malloc(packet._len);
    _len = packet._len;
    if(_data != NULL) {
        memcpy(_data, packet._data, _len);
    }
    strcpy(_localIp, packet._localIp);
    _localPort = packet._localPort;
    strcpy(_remoteIp, packet._remoteIp);
    _remotePort = packet._remotePort;
}
AsyncUDPPacket::AsyncUDPPacket(uint8_t *data, size_t length, struct sockaddr_in local_addr, struct sockaddr_in remote_addr) {
    _data = (uint8_t*)malloc(length);
    _len = length;
    if(_data != NULL) {
        memcpy(_data, data, _len);
    }
    strcpy(_localIp, inet_ntoa(local_addr.sin_addr));
    _localPort = ntohs(local_addr.sin_port);
    strcpy(_remoteIp, inet_ntoa(remote_addr.sin_addr));
    _remotePort = ntohs(remote_addr.sin_port);
}

AsyncUDPPacket::~AsyncUDPPacket() {
    if(_data != NULL) {
        free(_data);
        _data = NULL;
    }
}

uint8_t* AsyncUDPPacket::data() {
    return _data;
}

size_t AsyncUDPPacket::length() {
    return _len;
}

const char* AsyncUDPPacket::localIP() {
    return (const char*)_localIp;
}

uint16_t AsyncUDPPacket::localPort() {
    return _localPort;
}

const char* AsyncUDPPacket::remoteIP() {
    return (const char*)_remoteIp;
}

uint16_t AsyncUDPPacket::remotePort() {
    return _remotePort;
}


AsyncUDP::AsyncUDP() {
    listen_sockfd = INVALID_SOCKET;
    this->startup();
}

AsyncUDP::~AsyncUDP() {
    this->close();
    this->cleanup();
}

void AsyncUDP::onPacket(AuPacketHandlerFunction cb) {
    this->callback = cb;
}

bool AsyncUDP::listen(uint16_t port) {
    socket_fd sockfd;

    if(listen_sockfd != INVALID_SOCKET) {
        return false;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == INVALID_SOCKET) {
        return false;
    }

    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(port);
    bind_addr.sin_addr.s_addr = INADDR_ANY;
    _listenIP = IPAddress(bind_addr);

    if(bind(sockfd, (struct sockaddr*)&bind_addr, sizeof(struct sockaddr)) == -1) {
        socket_close(sockfd);
        return false;
    };

    listen_sockfd = sockfd;

    std::thread([this, port]() {
        uint8_t buffer[RECEIVE_BUFFER_SIZE] = {0};
        struct sockaddr_in remote_addr = {0};
        socklen_t remote_len = sizeof(remote_addr);
        while(this->listen_sockfd != INVALID_SOCKET) {
            int ret = recvfrom(this->listen_sockfd, (socket_buffer *)buffer, RECEIVE_BUFFER_SIZE, 0, (struct sockaddr*)&remote_addr, &remote_len);
            if(ret > 0) {
                if(this->callback != NULL) {
                    AsyncUDPPacket packet(buffer, (size_t)ret, this->bind_addr, remote_addr);
                    this->callback(packet);
                }
            }
        }
    }).detach();

    return true;
}

void AsyncUDP::close() {
    if(listen_sockfd != INVALID_SOCKET) {
        socket_close(listen_sockfd);
        listen_sockfd = INVALID_SOCKET;
    }
}

size_t AsyncUDP::writeTo(const uint8_t *data, size_t len, const char *addr, uint16_t port) {
    return sendTo(data, len, addr, port);
}

size_t AsyncUDP::writeTo(const uint8_t *data, size_t len, IPAddress addr, uint16_t port) {
    return sendTo(data, len, addr.toString().c_str(), port);
}

size_t AsyncUDP::broadcastTo(const uint8_t *data, size_t len, uint16_t port) {
    return sendTo(data, len, "255.255.255.255", port);
}

IPAddress AsyncUDP::listenIP() {
    return _listenIP;
}

size_t AsyncUDP::sendTo(const uint8_t *data, size_t len, const char *addr, uint16_t port) {
    socket_fd sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == INVALID_SOCKET) {
        return -1;
    }

    socket_optval optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1) {
        socket_close(sockfd);
        return -1;
    }

    struct sockaddr_in recv_addr = {0};
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(port);
    recv_addr.sin_addr.s_addr = inet_addr(addr);

    size_t sentLen = sendto(sockfd, (socket_buffer *)data, len, 0, (struct sockaddr*)&recv_addr, sizeof(struct sockaddr));

    socket_close(sockfd);

    return sentLen;
}

void AsyncUDP::startup() {
    if(!bStartup) {
#if defined(__ASYNC_UDP_IMPL_WIN__)
        WSADATA wsaData = {0};
        WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
        bStartup = true;
    }
    if(bStartup) {
        iStartupCounter++;
    }
}

void AsyncUDP::cleanup() {
    if(bStartup) {
        iStartupCounter--;
    }
    if(bStartup) {
        if(iStartupCounter == 0) {
#if defined(__ASYNC_UDP_IMPL_WIN__)
            WSACleanup();
#endif
            bStartup = false;
        }
    }
}
