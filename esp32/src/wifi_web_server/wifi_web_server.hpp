#ifndef SRC_WIFI_WEB_SERVER_WIFI_WEB_SERVER_HPP
#define SRC_WIFI_WEB_SERVER_WIFI_WEB_SERVER_HPP


#include <Arduino.h>
#include <WiFi.h>


class WiFiWebServer
{
private:
	WiFiServer *_server;	

public:
	WiFiWebServer() : _server(nullptr) {}
	bool begin();
	void handleClient();

private:
	bool parseRequest(const String &requestLine, String *method, 
		String *uri, String *protocol);
	void handleNone(WiFiClient *client);
	void handleRoot(WiFiClient *client);
	void handlePing(WiFiClient *client);
	void handleIP(WiFiClient *client);
	void handleMac(WiFiClient *client);
};


#endif
