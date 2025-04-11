#ifndef SRC_WIFI_WEB_SERVER_WIFI_WEB_SERVER_HPP
#define SRC_WIFI_WEB_SERVER_WIFI_WEB_SERVER_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


class WiFiWebServer
{
private:
	WebServer *_server;	
public:
	WiFiWebServer() : _server(nullptr) {}
	bool begin();
	void handleClient();
private:
	void onConnect();
};


#endif
