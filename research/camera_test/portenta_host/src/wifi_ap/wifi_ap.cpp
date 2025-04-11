#include "wifi_ap.hpp"


WiFiAP::~WiFiAP()
{

}


bool WiFiAP::begin()
{
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Failed to communicate with the WiFi module.");
		return false;
	}
	WiFi.config(IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 0, 1));

	int status = WiFi.beginAP(_ssid, _password);
	if (status != WL_AP_LISTENING) {
		Serial.println("Access Point creation failed.");
		return false;
	}

	Serial.println("Created access point:");
	Serial.print("SSID: ");
	Serial.println(_ssid);
	Serial.print("Password: ");
	Serial.println(_password);
	_status = status;	

	return true;
}


void WiFiAP::update()
{
	hasDeviceConnected();
}


void WiFiAP::hasDeviceConnected()
{
	if (_status != WiFi.status()) {
		_status = WiFi.status();
		if (_status == WL_AP_CONNECTED) {
			Serial.println("Device connected to AP.");
		}
		else {
			Serial.println("Device disconnected from AP.");
		}
	}
}


String WiFiAP::getDeviceAndNetworkInfo() {
	String info = "";
	
	info += "> Device info:\n";
	info += "\tPortenta H7\n";
	info += "\tChipset: Murata® LBEE5KL1DX\n";
	info += "\tWi-Fi: 2.4 GHz 802.11 b/g/n\n";
	info += "\tFirmware Version: ";
	info += WiFi.firmwareVersion();
	info += "\n";

	info += "> Network info:\n";
	info += "\tSSID: ";
	info += WiFi.SSID();
	info += "\n";
	
	IPAddress ip = WiFi.localIP();
	info += "\tIP Address: ";
	info += ip.toString();
	info += "\n";
	
	info += "\tGateway: ";
	info += WiFi.gatewayIP().toString();
	info += "\n";
	
	info += "\tSubnet Mask: ";
	info += WiFi.subnetMask().toString();
	info += "\n";
	
	info += "\tMAC Address: ";
	info += WiFi.macAddress();
	info += "\n";
	
	return info;
}


String WiFiAP::sanitizePassword(String input)
{
	input.trim();   

	input.replace("\n", "");
	input.replace("\r", "");
	input.replace("\t", "");
	input.replace("\\", "");
	input.replace(" ", "");

	String allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:,.<>/?";
	String sanitized = "";
	for (size_t i = 0; i < input.length(); i++) {
		char c = input.charAt(i);
		if (allowed.indexOf(c) < 0) {
			Serial.println("Invalid character in password, using default.");
			sanitized = "default123";
			break	;
		}
		else {
			sanitized += c;
		}
	}
	return sanitized;
}