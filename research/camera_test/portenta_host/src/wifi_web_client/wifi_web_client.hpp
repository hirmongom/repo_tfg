#ifndef SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP
#define SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>


class WiFiWebClient
{
private:
	WiFiClient *_client;
	const char *_server_addr = "192.168.0.32";
	const uint8_t _server_port = 80;
public:
	WiFiWebClient() = default; 
	bool begin();

};


#endif