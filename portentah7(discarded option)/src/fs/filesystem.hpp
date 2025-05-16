#ifndef FS_FILE_SYSTEM_HPP
#define FS_FILE_SYSTEM_HPP


#include <limits>

#include <Arduino.h>

#include <SDMMCBlockDevice.h>
#include <FATFileSystem.h>

#include "../util/http_resp.hpp"


namespace fs
{


/******************************************************************************/
typedef struct _file_system {
	SDMMCBlockDevice block_device;
	mbed::FATFileSystem fs;
	bool mounted;
	uint32_t write_count;

	_file_system()
		: fs("fs"), mounted(false), write_count(0)
	{}


/******************************************************************************/
	bool mountFileSystem()
	{
		Serial.println("Mounting FileSystem");

		int err =  fs.mount(&block_device);
		if (err) {
			Serial.println("No filesystem found, formatting... ");
			err = fs.reformat(&block_device);
		}
		if (err) {
			Serial.println("Error formatting SDCard");
			return false;
		}
	
		mounted = true;
		Serial.println("FileSystem mounted");
		return true;	
	}


/******************************************************************************/
	void writeToFile(const std::string &filename, uint8_t *data, size_t len)
	{
		if (!mounted) {
			Serial.println("FileSystem is not mounted");
			return;
		}
		
		std::string full_path = "fs/" + filename;

		FILE *fp = fopen(full_path.c_str(), "wb");
		if (fp == nullptr) {
			Serial.println("Failed to open file for writing");
			return;
		}
	
		size_t written = fwrite(data, 1, len, fp);
		fclose(fp);
	
		if (written != len) {
			Serial.println("Failed to write complete data to file");
		}

		Serial.print("Wrote ");
		Serial.print(written);
		Serial.println(" bytes to file");

		if (++write_count == std::numeric_limits<uint32_t>::max()) {
			write_count = 0;
		}
	}

}	file_system_t;


}	// namespace fs


#endif	// #ifndef FS_FILE_SYSTEM_HPP