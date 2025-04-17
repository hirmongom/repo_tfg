#ifndef SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP
#define SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>


class WiFiWebClient
{
private:
	WiFiClient _client;
	const char *_server_addr = "192.168.0.32";
	const uint8_t _server_port = 80;
	struct HttpResponse {
		String headers;
		String body;
	};

public:
	WiFiWebClient() = default; 
	bool begin();
	bool verifyConnection();
	uint8_t status(String &status_out);
	uint8_t status();
	String serverGet(String uri);
	String serverRoot();
	String serverPing();
	String serverIP();
	String serverMac();

private:
	void waitForServerConnected();
	bool connectToServer();
	String getResponse(uint16_t timeout_ms);
	HttpResponse parseHttpResponse(const String &response);
	String getHeaderField(const String &headers, const String &field);
};


#endif