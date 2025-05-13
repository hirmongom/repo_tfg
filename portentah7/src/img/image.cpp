#include "image.hpp"

namespace img
{


Image *Image::_active_instance = nullptr;


/******************************************************************************/
bool Image::load(const uint8_t *jpeg_data, size_t size)
{
	Serial.println("Attempting to load image...");
	_active_instance = this;

	if (!_jpeg.openFLASH(jpeg_data, size, JPEGDraw)) {
		Serial.println("Failed to load jpeg into FLASH");
		return false;
	}
	Serial.println("Loaded jpeg into FLASH");

	_width = _jpeg.getWidth() / 2;
	_height = _jpeg.getHeight() / 2;

	Serial.println("Checkpoint");
	if (_buffer) {
		delete[] _buffer;
		_buffer = nullptr;
	}
	Serial.print("Allocating ");
	Serial.print(_width * _height * 2);
	Serial.println(" bytes");
	Serial.print("Width = ");
	Serial.println(_width);
	Serial.print("Height = ");
	Serial.println(_height);
	_buffer = new uint16_t[_width * _height];

	Serial.println("Attempting to decode image");
	if (!_jpeg.decode(0, 0, 1)) {
		Serial.println("Failed to decode");
		return false;
	}
	Serial.println("Decoded image");

	return true;
}


/******************************************************************************/
int Image::JPEGDraw(JPEGDRAW *pDraw)
{
	if (!_active_instance || !_active_instance->_buffer) {
		return 0;
	}

	int w = pDraw->iWidth;
	int h = pDraw->iHeight;
	int x = pDraw->x;
	int y = pDraw->y;

	uint16_t *src = (uint16_t *)pDraw->pPixels;
	uint16_t *dst = _active_instance->_buffer;

	for (int i = 0; i < h; i++) {
		int dst_offset = (x + i) * _active_instance->_width;
	}
	return 1;
}


}	// namespace img