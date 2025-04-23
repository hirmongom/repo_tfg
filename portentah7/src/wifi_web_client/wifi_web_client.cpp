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
	status_out = parseStatusCode(code);
	return code;
}


/******************************************************************************/
HttpResponse WiFiWebClient::serverGet(String uri)
{
	String request = "GET /";
	request += uri;
	request += " HTTP/1.1\r\n\r\n";
	_client.print(request);
	return getResponse(5000);
}


/******************************************************************************/
HttpResponse WiFiWebClient::serverRoot()
{
	_client.print("GET / HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
HttpResponse WiFiWebClient::serverPing()
{
	_client.print("GET /ping HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
HttpResponse WiFiWebClient::serverIP()
{
	_client.print("GET /ip HTTP/1.1\r\n\r\n");
	return getResponse(5000);
}


/******************************************************************************/
HttpResponse WiFiWebClient::serverMac()
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
void WiFiWebClient::flushRecvBuffer()
{
	while (_client.available()) {
		_client.read();
	}
}


/******************************************************************************/
String WiFiWebClient::parseStatusCode(uint8_t code)
{
	String status_str;
	switch (code) {
		case WL_NO_SHIELD:				status_str = "WL_NO_SHIELD";				break;
		case WL_IDLE_STATUS:			status_str = "WL_IDLE_STATUS";			break;
		case WL_NO_SSID_AVAIL:		status_str = "WL_NO_SSID_AVAIL";		break;
		case WL_SCAN_COMPLETED:		status_str = "WL_SCAN_COMPLETED";		break;
		case WL_CONNECTED:				status_str = "WL_CONNECTED";				break;
		case WL_CONNECT_FAILED:		status_str = "WL_CONNECT_FAILED";		break;
		case WL_CONNECTION_LOST:	status_str = "WL_CONNECTION_LOST";	break;
		case WL_DISCONNECTED:			status_str = "WL_DISCONNECTED";			break;
		case WL_AP_LISTENING:			status_str = "WL_AP_LISTENING";			break;
		case WL_AP_CONNECTED:			status_str = "WL_AP_CONNECTED";			break;
		case WL_AP_FAILED:				status_str = "WL_AP_FAILED";				break;
		default:									status_str = "UNKNOWN_STATUS";			break;
	}
	return status_str;
}


/******************************************************************************/
HttpResponse WiFiWebClient::getResponse(uint32_t timeout_ms)
/*
 * The function client.flush() lacks any implementation
 * From "SocketWrapper/src/MbedClient.cpp"
 * on "ArduinoCore-mbed".
 */
{
	HttpResponse response;
	uint32_t content_length = 0;
	uint32_t start_time = millis();

	while (!_client.available()) {
		if (millis() - start_time > timeout_ms) {
			response.headers = 
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 7";
			response.body = "Timeout";
			flushRecvBuffer();
			return response;
		}
	}

	if (!getHeaders(response, start_time, timeout_ms)) {
		flushRecvBuffer();
		return response;
	}

	content_length = (uint32_t)getHeaderField(response, "Content-Length").toInt();	
	if (content_length > 0) {
		if (!getBody(response, start_time, timeout_ms, content_length)) {
			flushRecvBuffer();
			return response;
		}
	}
	else {
		response.body = "";
	}
	flushRecvBuffer();

	return response; 
}


/******************************************************************************/
bool WiFiWebClient::getHeaders(HttpResponse &response_out, uint32_t start_time,
	uint32_t timeout)
{
	while (!response_out.headers.endsWith("\r\n\r\n")) {
		if (millis() - start_time > timeout) {
			response_out.headers = 
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 7";
			response_out.body = "Timeout during header read.";
			return false;
		}
		if (_client.available()) {
			response_out.headers += (char)_client.read();
		}
	}
	response_out.headers = response_out.headers.substring(
		0, 
		response_out.headers.length() - 4
	);
	return true;
}


/******************************************************************************/
bool WiFiWebClient::getBody(HttpResponse &response_out, uint32_t start_time,
	uint32_t timeout, uint32_t length)
{
	size_t bytes_read = 0;
	while (bytes_read < length) {
		if (millis() - start_time > timeout) {
			response_out.headers = 
				"HTTP/1.1 500 Internal Server Error\r\n"
				"Content-Type: text/plain\r\n"
				"Content-Length: 7";
			response_out.body = "Timeout during body read.";
			return false;
		}
		if (_client.available()) {
			response_out.body += (char)_client.read();
			++bytes_read;
		}
	}
	return true;
}


/******************************************************************************/
String WiFiWebClient::getHeaderField(const HttpResponse &response, 
	const String &field)
{
	String lookFor = field + ": ";
	int start = response.headers.indexOf(lookFor);
	if (start < 0) {
		return "";
	}

	start += lookFor.length();
	int end = response.headers.indexOf("\r\n", start);
	if (end < 0) {
		end = response.headers.length();
	}

	return response.headers.substring(start, end);
}