#ifndef PORTENTA_HOST_WIFI_AP_H
#define PORTENTA_HOST_WIFI_AP_H


#include <Arduino.h>
#include <WiFi.h>


#define DEFAULT_PASSWORD "default123"


class WiFiAP
{
private:
	const char *_ssid = "PortentaHost";
	const char  *_password;
	WiFiServer *_server;

public:
	WiFiAP() : _password(DEFAULT_PASSWORD), _server(nullptr) {}
	~WiFiAP();
	bool begin();
	String getDeviceAndNetworkInfo();

private:
	String sanitizePassword(String input);
};


#endif