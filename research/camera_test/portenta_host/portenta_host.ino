#include <Arduino.h>
#include "src/wifi_ap/wifi_ap.hpp"


WiFiAP ap;

void setup()
{
	Serial.begin(112500);
	while (!Serial);
	if (!ap.begin()) {
		while (1);
	}
}


void loop()
{
	Serial.println(ap.getDeviceAndNetworkInfo());
	delay(10000);
}
