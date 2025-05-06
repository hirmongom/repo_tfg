#ifndef IMG_IMAGE_UTILS_HPP
#define IMG_IMAGE_UTILS_HPP


#include <string>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include <cstdint>


namespace img
{


/******************************************************************************/
inline void print_hex_dump(const uint8_t *data, size_t length, 
	size_t bytes_per_line = 16, size_t max_bytes = SIZE_MAX)
{
	size_t limit = (length < max_bytes) ? length : max_bytes;

	for (size_t i = 0; i < limit; i += bytes_per_line) {
		// Print offset
		Serial.print(i, HEX);
		Serial.print(": \t");

		// Print hex bytes
		for (size_t j = 0; j < bytes_per_line; ++j) {
			if (i + j < limit) {
				if (data[i + j] < 0x10) {
					Serial.print("0");
				}
				Serial.print(data[i + j], HEX);
				Serial.print(" ");
			}
			else {
				Serial.print("  \t");
			}
		}

		// Print ASCII representation
		Serial.print("  ");
		for (size_t j = 0; j < bytes_per_line && (i + j) < limit; ++j) {
			char c = data[i + j];
			if (c >= 32 && c <= 126) {
				Serial.print(c);
			}
			else {
				Serial.print('.');
			}
		}
		Serial.println();
	}
}


/******************************************************************************/
inline std::string hex_dump_to_string(const uint8_t *data, size_t length,
	size_t bytes_per_line = 16, size_t max_bytes = SIZE_MAX)
{
	std::ostringstream oss;
	size_t limit = (length < max_bytes) ? length : max_bytes;

	for (size_t i = 0; i < limit; i += bytes_per_line) {
		// Offset
		oss << std::hex << std::setw(4) << std::setfill('0') << i << ": \t";

		// Hex bytes
		for (size_t j = 0; j < bytes_per_line; ++j) {
			if (i + j < limit) {
				oss << std::hex << std::setw(2) << std::setfill('0')
					<< static_cast<int>(data[i + j]) << " ";
			}
			else {
				oss << "   ";
			}
		}

		oss << "  ";

		// ASCII representation
		for (size_t j = 0; j < bytes_per_line && (i + j) < limit; ++j) {
			char c = static_cast<char>(data[i + j]);
			if (c >= 32 && c <= 126) {
				oss << c;
			}
			else {
				oss << '.';
			}
		}

		oss << '\n';
	}

	return oss.str();
}


}	// namespace img


#endif	// IMG_IMAGE_UTILS_HPP