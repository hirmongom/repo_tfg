#ifndef SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP
#define SRC_WIFI_WEB_CLIENT_WIFI_WEB_CLIENT_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>


struct HttpResponse {
	String headers;
	String body;
};


class WiFiWebClient
{
public:
	WiFiWebClient() = default; 
	bool begin();
	bool verifyConnection();
	uint8_t status(String &status_out);
	uint8_t status();
	static String httpResponseToString(const HttpResponse &response);
	HttpResponse serverGet(String uri);
	HttpResponse serverRoot();
	HttpResponse serverPing();
	HttpResponse serverIP();
	HttpResponse serverMac();
	HttpResponse serverCapture();

private:
	WiFiClient _client;
	const char *_server_addr = "192.168.0.32";
	const uint8_t _server_port = 80;

private:
	void waitForServerConnected();
	bool connectToServer();
	void flushRecvBuffer();
	String parseStatusCode(uint8_t code);
	HttpResponse getResponse(uint32_t timeout_ms);
	bool getHeaders(HttpResponse &response_out, uint32_t start_time,
		uint32_t timeout);
	bool getBody(HttpResponse &response_out, uint32_t start_time,
		uint32_t timeout, uint32_t length);
	String getHeaderField(const HttpResponse &response, const String &field);
};


inline String WiFiWebClient::httpResponseToString(const HttpResponse &response)
{
	return response.headers + "\r\n\r\n" + response.body;
}


#endif