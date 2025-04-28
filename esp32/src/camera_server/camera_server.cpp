#include "camera_server.hpp"


/******************************************************************************/
bool CameraServer::begin()
{
	esp_err_t err = esp_camera_init(&camera_config);
	if (err != ESP_OK) {
		Serial.println("Failed to initialize camera.");
		return false;
	}

	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("Not connected to a WiFi network. Aborting.");
		return false;
	}	

	// if (!_server) {
	// 	Serial.println("Failed creating web server.");
	// 	return false;
	// }

	_server.begin();
	Serial.println("Server created.");

	return true;
}


/******************************************************************************/
void CameraServer::handleClient()
{
	WiFiClient client = _server.accept();
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
						handleRoot(client);
					}
					else if (uri == "/ping") {
						handlePing(client);
					}
					else if (uri == "/ip") {
						handleIP(client);
					}
					else if (uri == "/mac") {
						handleMac(client);
					}
					else if (uri == "/capture") {
						handleCapture(client);
					}
					else {
						handleNone(client);
					}
				}
			}	
			else {
				Serial.println("Malformed HTTP Request.");
			}
		}
	}
}


/******************************************************************************/
bool CameraServer::parseRequest(const String &requestLine, String *method, 
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
void CameraServer::handleNone(WiFiClient &client)
{
	client.print("HTTP/1.1 404 Not Found\r\n\r\n");
}


/******************************************************************************/
void CameraServer::handleRoot(WiFiClient &client)
{
	client.print("HTTP/1.1 204 No Content\r\n\r\n");
}


/******************************************************************************/
void CameraServer::handlePing(WiFiClient &client)
{
	String body = "Pong!";
	client.print("HTTP/1.1 200 OK\r\n");
	client.print("Content-Type: text/plain\r\n");
	client.print("Content-Length: ");
	client.print(body.length());
	client.print("\r\n\r\n");
	client.print(body);
	client.print("\r\n\r\n");
}


/******************************************************************************/
void CameraServer::handleIP(WiFiClient &client) 
{
	String body = WiFi.localIP().toString();
	client.print("HTTP/1.1 200 OK\r\n");
	client.print("Content-Type: text/plain\r\n");
	client.print("Content-Length: ");
	client.print(body.length());
	client.print("\r\n\r\n");
	client.print(body);
	client.print("\r\n\r\n");
}


/******************************************************************************/
void CameraServer::handleMac(WiFiClient &client)
{
	String body = WiFi.macAddress();
	client.print("HTTP/1.1 200 OK\r\n");
	client.print("Content-Type: text/plain\r\n");
	client.print("Content-Length: ");
	client.print(body.length());
	client.print("\r\n\r\n");
	client.print(body);
	client.print("\r\n\r\n");
}


/******************************************************************************/
void CameraServer::handleCapture(WiFiClient &client)
{
	camera_fb_t *fb = NULL;
	fb = esp_camera_fb_get();

	if (!fb) {
		const char *errHdr =
			"HTTP/1.1 500 Internal Server Error\r\n"
			"Content-Length: 0\r\n\r\n";
		client.print(errHdr);
		return;
	}

	client.print("HTTP/1.1 200 OK\r\n");
	client.print("Content-Type: text/plain\r\n");
	client.print("Content-Length: ");
	client.print(fb->len);
	client.print("\r\n\r\n");
	
	client.write(fb->buf, fb->len);
	client.print("\r\n\r\n");
	
	Serial.println("Camera capture:");
	Serial.print("\tLength = ");
	Serial.println(fb->len);

	esp_camera_fb_return(fb);
}