#ifndef SRC_CAMERA_CLIENT_CAMERA_CLIENT_HPP
#define SRC_CAMERA_CLIENT_CAMERA_CLIENT_HPP

#include <vector>

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <NetworkInterface.h>
#include <utility/http_request.h>
#include <utility/http_response.h>


class CameraClient
{
public:
	CameraClient(): _srv_addr(192, 168, 1, 32) {}
	bool begin();
	bool capture();

private:
	NetworkInterface *_interface;
	WiFiClient _client;
	const char *_ssid = "PortentaHost";
	const char *_password = "default123";
	const IPAddress _srv_addr;
	const uint16_t _srv_port = 80;

private:
	bool beginWiFi();
	bool serverConnect();
};


#endif