#include "src/cam/camera.hpp"
#include "src/img/image.hpp"
#include "src/fs/filesystem.hpp"
#include "src/util/http_resp.hpp"


using namespace cam;
using namespace img;
using namespace fs;
using namespace util;

CameraClient camera;
file_system_t filesystem;


/******************************************************************************/
void setup()
{
	Serial.begin(112500);
	while (!Serial);

	if (!camera.begin()) {
		while (1);
	}

	if (!filesystem.mountFileSystem()) {
		while(1);
	}
}


/******************************************************************************/
void loop()
{
	http_response_t resp;

	if (camera.capture(resp)) {
		String msg = resp.to_string().c_str();
		Serial.println(msg);

		print_hex_dump(resp.body, resp.body_len, 16, 64);
		std::string dump = hex_dump_to_string(resp.body, resp.body_len, 16);

		filesystem.writeToFile("image.jpeg", resp.body, resp.body_len);
		filesystem.writeToFile("hexdump.txt", (uint8_t *)dump.data(), dump.size());
	}

	// delay(2000);
	while(1);
}