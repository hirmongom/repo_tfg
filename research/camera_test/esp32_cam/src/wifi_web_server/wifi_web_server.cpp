#include "wifi_web_server.hpp"


bool WiFiWebServer::begin()
{
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Not connected to a WiFi network. Aborting.");
		return false;
	}	
	_server = new WebServer(80);
	if (!_server) {
		Serial.println("Failed creating web server.");
		return false;
	}

	_server->on("/", [this]() {
		this->onConnect();
	});
	_server->begin();
	Serial.println("Server created.");

	return true;
}


void WiFiWebServer::onConnect()
{
	Serial.println("Client has connected to server");
	_server->send(200, "text/plain", WiFi.macAddress());
}


void WiFiWebServer::handleClient()
{
	_server->handleClient();
}