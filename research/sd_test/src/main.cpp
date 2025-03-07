/*
 * SPI:
 * pin 4(D4) -> CS
 * SPI connected to SPI connector (page 2 of pinout doc)
*/

#include <Arduino.h>
#include <SD.h>


#define BUILTIN_LED 13

const int chipSelect = 4;	// Chip Select pin
File sequence;
File outFile;

void setup() 
{
	Serial.begin(9600);
	while (!Serial);
  
	if (!SD.begin(chipSelect)) {
		Serial.println("SD Initialization failed");
		while (true);
	}
	Serial.print("SD Initialization finished");

	sequence = SD.open("sequence", FILE_READ);
	if (!sequence) {
		Serial.println("Unable to open sequence file");
		while (true);
	}
	Serial.println("Sequence file opened");

	outFile = SD.open("outfile.txt", FILE_WRITE);
	if (!outFile) {
		Serial.println("Unable to open output file");
		while (true);
	}
	Serial.println("Output file opened");
	Serial.println("Writing to file...");
	outFile.println("test");
	Serial.println("Writting complete");
	outFile.close();
}

void loop() 
{
	while (sequence.available()) {
		Serial.println(char(sequence.read()));
		delay(500);
	}
	sequence.seek(0);
}