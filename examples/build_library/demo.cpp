#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ugot_broadcast_message.h"

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc, const char *argv[])
{
	uGotBroadcastChannel channel;

	// Optional set communication channel, default is 0
	// channel.setChannel(0);

	// Enable broadcast funtion, default is off
	channel.setEnable(true);

	// Set broadcast message received handler
	channel.setMessageReceivedCallback([](const char* msg) {
	  printf("Received Message: %s\n", msg);
	});

	// Send broadcast to current channel
	channel.sendBroadcastMessage("Hello world");

	// Send unicast message to specify address
	channel.sendMessageTo("Hello world", "10.11.12.13");
	
	while(1) {
		sleep(1);
	}

	return 0;
}

#ifdef __cplusplus
}
#endif