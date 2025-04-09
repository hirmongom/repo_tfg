#include "wifi_ap.hpp"


WiFiAP::~WiFiAP()
{
	if (_server) {
		delete _server;
		_server = nullptr;
	}
}


bool WiFiAP::begin()
{
	if (WiFi.status() == WL_NO_MODULE) {
		Serial.println("Failed to communicate with the WiFi module.");
		return false;
	}
	_server = new WiFiServer(80);
	WiFi.config(IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 0, 1));

	Serial.setTimeout(10000);
	Serial.println("Provide a password for the Access Point (Min. 8 characters):");
	String input = Serial.readStringUntil('\n');
	if (input.length() == 0) {
		Serial.println("No input received, using default password.");
	}
	else {
		String sanePassword  = sanitizePassword(input);
		if (sanePassword.length() < 8 || sanePassword.length() > 32) {
			Serial.println("Invalid password length.");
			Serial.println("Using default password.");
		}
		else {
			_password = sanePassword.c_str();
		}
	}

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
	
	_server->begin();
	return true;
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