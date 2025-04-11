#include <Arduino.h>
#include "src/wifi_ap/wifi_ap.hpp"
#include "src/wifi_web_client/wifi_web_client.hpp"

WiFiAP ap;
WiFiWebClient client;


void setup()
{
	Serial.begin(112500);
	while (!Serial);
	if (!ap.begin()) {
		Serial.println("Failed to start AP.");
		while (1);
	}
	if (!client.begin()) {
		Serial.println("Failed to start client.");
		while (1);
	}
}


void loop()
{
	//Serial.println(ap.getDeviceAndNetworkInfo());
	ap.update();

	delay(1000);
}
