#ifndef SRC_CAMERA_CLIENT_CAMERA_CLIENT_HPP
#define SRC_CAMERA_CLIENT_CAMERA_CLIENT_HPP

#include <vector>
#include <limits>

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <NetworkInterface.h>
#include <utility/http_request.h>
#include <utility/http_response.h>

#include "SDMMCBlockDevice.h"
#include "FATFileSystem.h"

class CameraClient
{
public:
	CameraClient(): _srv_addr(192, 168, 1, 32), _fs("fs") {}
	bool begin();
	bool capture();

private:
	NetworkInterface *_interface;
	WiFiClient _client;
	const char *_ssid = "PortentaHost";
	const char *_password = "default123";
	const IPAddress _srv_addr;
	const uint16_t _srv_port = 80;
	SDMMCBlockDevice _block_device;
	mbed::FATFileSystem _fs;
	bool _have_fs = false;
	uint32_t _frame_count = 0;
private:
	bool beginWiFi();
	bool serverConnect();
	bool mountSDCard();
	bool writeImageToSDCard(uint8_t *buf, size_t len);
};


#endif