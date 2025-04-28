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

	Serial.println("\n================\nFirst Message Wave");
	serverCheck();
	delay(5000);
	Serial.println("\n================\nSecond Message Wave");
	serverCheck();
	delay(5000);
	Serial.println("\n================\nThird Message Wave");
	serverCheck();
	Serial.println("\n================\nGetting frame");
	HttpResponse frame = client.serverCapture();
	Serial.println(WiFiWebClient::httpResponseToString(frame));
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
	HttpResponse ret;

	client.verifyConnection();

	ret = client.serverRoot();
	Serial.println("Server root:");
	Serial.println(WiFiWebClient::httpResponseToString(ret));
	
	ret = client.serverPing();
	Serial.println("Server ping:");
	Serial.println(WiFiWebClient::httpResponseToString(ret));

	ret = client.serverIP();
	Serial.println("Server ip:");
	Serial.println(WiFiWebClient::httpResponseToString(ret));

	ret = client.serverMac();
	Serial.println("Server mac:");
	Serial.println(WiFiWebClient::httpResponseToString(ret));
	
	ret = client.serverGet("thisisnotaproperuri");
	Serial.println("Server not found:");
	Serial.println(WiFiWebClient::httpResponseToString(ret));
}