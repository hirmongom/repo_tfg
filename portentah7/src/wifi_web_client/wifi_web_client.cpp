#include "wifi_web_client.hpp"


/******************************************************************************/
bool WiFiWebClient::begin()
{
	waitForServerConnected();
	return connectToServer();
}


/******************************************************************************/
bool WiFiWebClient::verifyConnection()
{
	if (WiFi.status() != WL_AP_CONNECTED) {
		Serial.println("Server disconnected. Attempting to reconnect.");
		waitForServerConnected();
		return connectToServer();
	}
	return true;
}


/******************************************************************************/
uint8_t WiFiWebClient::status() {
	return WiFi.status();
}


/******************************************************************************/
uint8_t WiFiWebClient::status(String &status_out) {
	uint8_t code = status();
	switch (code) {
		case WL_NO_SHIELD:				status_out = "WL_NO_SHIELD";				break;
		case WL_IDLE_STATUS:			status_out = "WL_IDLE_STATUS";			break;
		case WL_NO_SSID_AVAIL:		status_out = "WL_NO_SSID_AVAIL";		break;
		case WL_SCAN_COMPLETED:		status_out = "WL_SCAN_COMPLETED";		break;
		case WL_CONNECTED:				status_out = "WL_CONNECTED";				break;
		case WL_CONNECT_FAILED:		status_out = "WL_CONNECT_FAILED";		break;
		case WL_CONNECTION_LOST:	status_out = "WL_CONNECTION_LOST";	break;
		case WL_DISCONNECTED:			status_out = "WL_DISCONNECTED";			break;
		case WL_AP_LISTENING:			status_out = "WL_AP_LISTENING";			break;
		case WL_AP_CONNECTED:			status_out = "WL_AP_CONNECTED";			break;
		case WL_AP_FAILED:				status_out = "WL_AP_FAILED";				break;
		default:									status_out = "UNKNOWN_STATUS";			break;
	}
	return code;
}


/******************************************************************************/
String WiFiWebClient::serverGet(String uri)
{
	String request = "GET /";
	request += uri;
	request += " HTTP/1.1\r\n\r\n";
	_client.print(request);
	return getResponse(5000);
}


/******************************************************************************/
String WiFiWebClient::serverRoot()
{
	_client.print("GET / HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
String WiFiWebClient::serverPing()
{
	_client.print("GET /ping HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
String WiFiWebClient::serverIP()
{
	_client.print("GET /ip HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
String WiFiWebClient::serverMac()
{
	_client.print("GET /mac HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
void WiFiWebClient::waitForServerConnected()
{
	Serial.print("Waiting for server to connect to AP");
	while (WiFi.status() != WL_AP_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println();
	Serial.println("Server connected.");
}


/******************************************************************************/
bool WiFiWebClient::connectToServer()
{
	if (_client.connected()) {
		_client.stop();
	}

	Serial.print("Attempting to connect to server");
	while (!_client.connect(_server_addr, _server_port)) {
		Serial.print(".");
		delay(500);
	}
	Serial.println();
	Serial.println("Connected to server.");
	return true;
}


/******************************************************************************/
String WiFiWebClient::getResponse(uint16_t timeout_ms)
{
	String headers = "";
	String body = ""; // @TODO fill body if content in header
	unsigned long start_time = millis();
	
	while (_client.available()) {
		if (body.endsWith("\r\n\r\n")) {
			_client.flush();
		}
		if (headers.endsWith("\r\n\r\n")) {
			body += (char)_client.read();	
		}
		else {
			headers += (char)_client.read();
		}
		if (millis() - start_time > timeout_ms) {
			return "Timeout";
		}
	}
	String response = "Headers:\n" + headers + "\nBody: \n" + body;
	return response; 
}
