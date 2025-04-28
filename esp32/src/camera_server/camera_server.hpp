#ifndef SRC_CAMERA_SERVER_CAMERA_SERVER_HPP
#define SRC_CAMERA_SERVER_CAMERA_SERVER_HPP


#include <Arduino.h>
#include <WiFi.h>

#include "camera_config.hpp"
#include "esp_camera.h"


class CameraServer
{
private:
	WiFiServer _server;

public:
	CameraServer() : _server(80) {}
	bool begin();
	void handleClient();

private:
	bool parseRequest(const String &requestLine, String *method, 
		String *uri, String *protocol);
	void handleNone(WiFiClient &client);
	void handleRoot(WiFiClient &client);
	void handlePing(WiFiClient &client);
	void handleIP(WiFiClient &client);
	void handleMac(WiFiClient &client);
	void handleCapture(WiFiClient &client);
};


#endif
