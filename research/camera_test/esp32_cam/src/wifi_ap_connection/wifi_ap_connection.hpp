#ifndef SRC_WIFI_AP_CONNECTION_WIFI_AP_CONNECTION_HPP
#define SRC_WIFI_AP_CONNECTION_WIFI_AP_CONNECTION_HPP


#include <Arduino.h>
#include <WiFi.h>


class WiFiAPConnection
{
private:
	const char *_ssid = "PortentaHost";
	const char *_password = "default123";

public:
	WiFiAPConnection() = default;
	void begin();

private:
	void onConnect();
};


#endif