#include <Arduino.h>
#include "src/wifi_ap/wifi_ap.hpp"
#include "src/wifi_web_client/wifi_web_client.hpp"


WiFiAP ap;
WiFiWebClient client;


void serverCheck();


/******************************************************************************/
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

	serverCheck();
	delay(20000);
	serverCheck();
	delay(10000);
	serverCheck();
}


/******************************************************************************/
void loop()
{
	//Serial.println(ap.getDeviceAndNetworkInfo());
	String status;
	client.status(status);
	// Serial.println(status);
	client.verifyConnection();
}


/******************************************************************************/
void serverCheck()
{
	String ret;

	client.verifyConnection();

	ret = client.serverRoot();
	Serial.println("Server root:");
	Serial.println(ret);

	ret = client.serverPing();
	Serial.println("Server ping:");
	Serial.println(ret);

	ret = client.serverIP();
	Serial.println("Server ip:");
	Serial.println(ret);

	ret = client.serverMac();
	Serial.println("Server mac:");
	Serial.println(ret);

	ret = client.serverGet("error");
	Serial.println("Server not found:");
	Serial.println(ret);
}