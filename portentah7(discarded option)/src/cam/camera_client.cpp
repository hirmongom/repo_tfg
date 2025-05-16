#include "camera_client.hpp"


namespace cam
{


/******************************************************************************/
bool CameraClient::begin()
{
	if (!beginWiFi()) {
		return false;
	}

	if (!serverConnect()) {
		return false;
	}

	_interface = WiFi.getNetwork();
	return true;
}


/******************************************************************************/
bool CameraClient::capture(util::http_response_t &out_http_resp)
{
	HttpRequest request(_interface, HTTP_GET, "http://192.168.1.32/capture");
	HttpResponse *response = request.send();

	if (response) {
		out_http_resp.fill_http_response(response);
		return true;
	}
	else {
		Serial.println("Failed to capture frame");
		return false;
	}
}


/******************************************************************************/
bool CameraClient::beginWiFi()
{
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Failed to communicate with the WiFi module.");
		return false;
	}

	WiFi.config(
		IPAddress(192, 168, 1, 254), 		// IP
		IPAddress(0, 0, 0, 0),					// DNS
		IPAddress(192, 168, 1, 254),		// Gateway
		IPAddress(255, 255, 255, 0)			// Subnet mask
	);

	int status = WiFi.beginAP(_ssid, _password);
	if (status != WL_AP_LISTENING) {
		Serial.println("Access Point creation failed.");
		return false;
	}

	Serial.println("Created access point");
	return true;
}


/******************************************************************************/
bool CameraClient::serverConnect()
{
	if (_client.connected()) {
		_client.stop();
	}
	
	_client.setSocketTimeout(500);
	Serial.print("Attempting to connect to server");
	while (!_client.connect(_srv_addr, _srv_port)) {
		Serial.print(".");
	}
	Serial.println();

	Serial.println("Connected to server");
	return true;
}


}	// namespace cam