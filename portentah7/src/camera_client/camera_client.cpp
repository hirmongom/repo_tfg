#include "camera_client.hpp"


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
bool CameraClient::capture()
{
	HttpRequest request(_interface, HTTP_GET, "http://192.168.1.32/capture");
	HttpResponse *response = request.send();
	if (response) {
		Serial.print("Status code: ");
		Serial.println(response->get_status_code());

		// Print all headers
		Serial.println("Headers:");
		vector<string*> fields = response->get_headers_fields();
		vector<string*> values = response->get_headers_values();

		for (size_t i = 0; i < fields.size() && i < values.size(); ++i) {
			Serial.print("\t");
			Serial.print(fields[i]->c_str());
			Serial.print(": ");
			Serial.println(values[i]->c_str());
		}

		Serial.println("Body (binary image):");
		
		uint8_t *body   = static_cast<uint8_t *>(response->get_body());
		size_t length   = response->get_body_length();

		Serial.write(body, length);

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
		IPAddress(192, 168, 1, 254), // IP
		IPAddress(0, 0, 0, 0),	// DNS
		IPAddress(192, 168, 1, 254),	// Gateway
		IPAddress(255, 255, 255, 0)	// Subnet mask
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
	Serial.print("Attempting to connect to server");
	while (!_client.connect(_srv_addr, _srv_port)) {
		Serial.print(".");
		delay(500);
	}
	Serial.println();

	Serial.println("Connected to server");
	return true;
}
