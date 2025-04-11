#include "wifi_web_client.hpp"


bool WiFiWebClient::begin()
{
	Serial.print("Waiting for server to connect to AP");
	while (WiFi.status() != WL_AP_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println();
	Serial.println("Device connected.");

	_client = new WiFiClient();
	if (!_client) {
		Serial.println("Failed to create client.");
		return false;
	}

	Serial.print("Attempting to connect to server");
	while (!_client->connect(_server_addr, _server_port)) {
		Serial.print(".");
		delay(500);
	}
	Serial.println();

	_client->println("GET / HTTP/1.1");
	_client->print("Host: ");
	_client->println(_server_addr);
	_client->println("Connection: close");
	_client->println();  // Blank line ends the HTTP header.

	String response = "";
	unsigned long startTime = millis();
	while (_client->connected() || _client->available()) {
		if (_client->available()) {
			response += (char)_client->read();
		}
		if (millis() - startTime > 5000) {
			break;
		}
	}
	
	Serial.println("Server Response:");
	Serial.println(response);
	return true;
}