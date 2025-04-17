#include "wifi_web_server.hpp"


/******************************************************************************/
bool WiFiWebServer::begin()
{
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Not connected to a WiFi network. Aborting.");
		return false;
	}	
	_server = new WiFiServer(80);
	if (!_server) {
		Serial.println("Failed creating web server.");
		return false;
	}

	_server->begin();
	Serial.println("Server created.");

	return true;
}


/******************************************************************************/
void WiFiWebServer::handleClient()
{
	WiFiClient client = _server->accept();
	if (!client) {
		return;
	}
	while (client.connected()) {
		if (client.available()) {
			String request = client.readStringUntil('\r');
			client.clear();

			Serial.println("Client request:");
			Serial.println(request);
			Serial.println();

			String method, uri, protocol;
			if (parseRequest(request, &method, &uri, &protocol)) {
				if (method == "GET" && protocol == "HTTP/1.1") {
					if (uri == "/") {
						handleRoot(&client);
					}
					else if (uri == "/ping") {
						handlePing(&client);
					}
					else if (uri == "/ip") {
						handleIP(&client);
					}
					else if (uri == "/mac") {
						handleMac(&client);
					}
					else {
						handleNone(&client);
					}
				}
			}	
		}
	}
}


/******************************************************************************/
bool WiFiWebServer::parseRequest(const String &requestLine, String *method, 
	String *uri, String *protocol) 
{
	// The request line should be in the form: "GET /uri HTTP/1.1"
	int firstSpace = requestLine.indexOf(' ');
	int secondSpace = requestLine.indexOf(' ', firstSpace + 1);
	
	if (firstSpace != -1 && secondSpace != -1) {
		*method = requestLine.substring(0, firstSpace);
		*uri = requestLine.substring(firstSpace + 1, secondSpace);
		*protocol = requestLine.substring(secondSpace + 1);
		return true;
	} else {
		return false;
	}
}


/******************************************************************************/
void WiFiWebServer::handleNone(WiFiClient *client)
{
	if (!client->connected()) {
		Serial.print("Waiting for client.");
		while (!client->connected()) {
			Serial.print(".");
		}
		Serial.println();
	}
	client->print("HTTP/1.1 404 Not Found\r\n\r\n");
}


/******************************************************************************/
void WiFiWebServer::handleRoot(WiFiClient *client)
{
	if (!client->connected()) {
		Serial.print("Waiting for client.");
		while (!client->connected()) {
			Serial.print(".");
		}
		Serial.println();
	}
	client->print("HTTP/1.1 204 No Content\r\n\r\n");
}


/******************************************************************************/
void WiFiWebServer::handlePing(WiFiClient *client)
{
	if (!client->connected()) {
		Serial.print("Waiting for client.");
		while (!client->connected()) {
			Serial.print(".");
		}
		Serial.println();
	}
	String body = "Pong!";
	client->print("HTTP/1.1 200 OK\n");
	client->print("Content-Type: text/plain\n");
	client->print("Content-Length: ");
	client->print(body.length());
	client->print("\r\n\r\n");
	client->print(body);
	client->print("\r\n\r\n");
}


/******************************************************************************/
void WiFiWebServer::handleIP(WiFiClient *client) 
{
	if (!client->connected()) {
		Serial.print("Waiting for client.");
		while (!client->connected()) {
			Serial.print(".");
		}
		Serial.println();
	}
	String body = WiFi.localIP().toString();
	client->print("HTTP/1.1 200 OK\n");
	client->print("Content-Type: text/plain\n");
	client->print("Content-Length: ");
	client->print(body.length());
	client->print("\r\n\r\n");
	client->print(body);
	client->print("\r\n\r\n");
}


/******************************************************************************/
void WiFiWebServer::handleMac(WiFiClient *client)
{
	if (!client->connected()) {
		Serial.print("Waiting for client.");
		while (!client->connected()) {
			Serial.print(".");
		}
		Serial.println();
	}
	String body = WiFi.macAddress();
	client->print("HTTP/1.1 200 OK\n");
	client->print("Content-Type: text/plain\n");
	client->print("Content-Length: ");
	client->print(body.length());
	client->print("\r\n\r\n");
	client->print(body);
	client->print("\r\n\r\n");
}