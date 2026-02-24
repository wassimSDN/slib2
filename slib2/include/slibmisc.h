#pragma once

#include <SDL3/SDL.h>

namespace slib
{
	struct Color
	{
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 255;
		
		static uint32_t packColor(Color c);
		static Color extractColor(uint32_t c);
	};
}