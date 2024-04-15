#include <M5StickC.h>
#include <WiFi.h>
#include <ugot_broadcast_message.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

static uGotBroadcastChannel channel;

void setup() {
  M5.begin();

  // Delete old wifi config
  WiFi.disconnect(true);

  delay(1000);

  Serial.printf("Connecting to WiFi[%s] ...\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("WiFi connect failure");
      while(1) {
          delay(1000);
      }
  }

  Serial.printf("WiFi connected, IP address: %s\n", WiFi.localIP().toString().c_str());

  // Optional set communication channel, default is 0
  // channel.setChannel(0);

  // Enable broadcast funtion, default is off
  channel.setEnable(true);

  // Set broadcast message received handler
  channel.setMessageReceivedCallback([](const char* msg) {
    Serial.printf("Received Message: %s\n", msg);
	});
}

void loop() {
  delay(5000);

  // Send broadcast to current channel
  channel.sendBroadcastMessage("Hello world");
}
