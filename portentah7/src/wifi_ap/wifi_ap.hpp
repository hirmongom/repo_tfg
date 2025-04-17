#ifndef PORTENTA_HOST_WIFI_AP_H
#define PORTENTA_HOST_WIFI_AP_H


#include <Arduino.h>
#include <WiFi.h>


class WiFiAP
{
private:
	const char *_ssid = "PortentaHost";
	const char  *_password = "default123";
	int _status;

public:
	WiFiAP() = default; 
	~WiFiAP();
	bool begin();
	String getDeviceAndNetworkInfo();

private:
	String sanitizePassword(String input);
	void hasDeviceConnected();	
};


#endif