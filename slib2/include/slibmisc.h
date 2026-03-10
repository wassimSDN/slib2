#pragma once

#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)

#endif

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

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

	class DirectoryListener
	{
	public:
		DirectoryListener(const char* path);
		~DirectoryListener();

		bool changed();

	private:
#if defined(_WIN32)
		HANDLE m_handle = nullptr;
#elif defined(__linux__)

#endif
	};
	
	TTF_Font* missingFont(float size);
}