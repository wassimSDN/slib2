#include "../include/slibmisc.h"

namespace slib
{
	uint32_t Color::packColor(Color c)
	{
		return  c.r << 24 | c.g << 16 | c.b << 8 | c.a ;
	}

	Color Color::extractColor(uint32_t c)
	{
		const uint8_t r = c >> 24 & 0xff;
		const uint8_t g = c >> 16 & 0xff;
		const uint8_t b = c >> 8 & 0xff;
		const uint8_t a = c & 0xff;

		return { r, g, b, a };
	}

}