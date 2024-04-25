#ifndef __ASYNC_UDP_H__
#define __ASYNC_UDP_H__

#if defined(__APPLE__)
#error "Not supported for MacOS"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if defined(__ASYNC_UDP_IMPL_WIN__)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#if defined(__ASYNC_UDP_IMPL_LINUX__)
#include <arpa/inet.h>
#endif

#include <functional>
#include <thread>
#include <string>

#if defined(__ASYNC_UDP_IMPL_WIN__)
#define socket_fd SOCKET 
#define socket_close(fd) closesocket(fd) 
#define socket_buffer char
#define socket_optval char

#define socklen_t int
#endif

#if defined(__ASYNC_UDP_IMPL_LINUX__)
#define socket_fd int
#define socket_close(fd) ::close(fd) 
#define socket_buffer uint8_t
#define socket_optval int

#define INVALID_SOCKET -1
#endif

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

#define RECEIVE_BUFFER_SIZE 1024

class AsyncUDP;
class AsyncUDPPacket;
class IPAddress;

typedef std::function<void(AsyncUDPPacket& packet)> AuPacketHandlerFunction;

class DLL_API IPAddress {
public:
	IPAddress();
    IPAddress(struct sockaddr_in addr);
    virtual ~IPAddress();

    void fromString(const char* addr);
	std::string toString();

protected:
	struct sockaddr_in _addr = {0};
};

class DLL_API AsyncUDPPacket {
public:
	AsyncUDPPacket(AsyncUDPPacket &packet);
    AsyncUDPPacket(uint8_t *data, size_t length, struct sockaddr_in local_addr, struct sockaddr_in remote_addr);
    virtual ~AsyncUDPPacket();

	uint8_t* data();
	size_t length();
    const char* localIP();
    uint16_t localPort();
    const char* remoteIP();
    uint16_t remotePort();
	
protected:
    char _localIp[16];
    uint16_t _localPort;
    char _remoteIp[16];
    uint16_t _remotePort;
    uint8_t *_data;
    size_t _len;
};

class DLL_API AsyncUDP {
public:
    AsyncUDP();
    virtual ~AsyncUDP();

    void onPacket(AuPacketHandlerFunction cb);
    bool listen(uint16_t port);
	void close();
    size_t writeTo(const uint8_t *data, size_t len, const char *addr, uint16_t port);
    size_t writeTo(const uint8_t *data, size_t len, IPAddress addr, uint16_t port);
    size_t broadcastTo(const uint8_t *data, size_t len, uint16_t port);
	IPAddress listenIP();

protected:
    socket_fd listen_sockfd;
	struct sockaddr_in bind_addr = {0};
    IPAddress _listenIP;
	AuPacketHandlerFunction callback;

    size_t sendTo(const uint8_t *data, size_t len, const char *addr, uint16_t port);

private:
	static bool bStartup;
	static uint32_t iStartupCounter;

	static void startup();
	static void cleanup();
};

#endif
