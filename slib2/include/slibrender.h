#pragma once

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "slibmisc.h"
#include "slibpos.h"

namespace slib
{
	class Surface
	{
	public:
		Surface();
		Surface(const char* filename);
		Surface(Color color);
		~Surface();

		bool load(const char* filename);

		SDL_Surface* get();
		bool isMissing() const;

	private:
		void createFromImage(const char* filename);

		bool missing = false;
		SDL_Surface* surface = nullptr;
	};

	SDL_Surface* missingSurface();

	class Texture
	{
	public:
		Texture();
		Texture(const char* filename);
		Texture(const char* filename, int windowIndex);
		Texture(Color color);
		Texture(Color color, int windowIndex);
		~Texture();
		
		bool load(const char* filename);
		bool createAsTarget(int w, int h);

		bool load(const char* filename, int windowIndex);
		bool createAsTarget(int w, int h, int windowIndex);

		void render(Rect dst) const;
		void render(Rect dst, Rect src) const;

		void render(float angle, Rect dst) const;
		void render(float angle, Rect dst, Rect src) const;

	private:
		SDL_Texture* texture = nullptr;
		bool createFromImage(const char* filename);
		bool createFromImage(const char* filename, int windowIndex);
	};

	class AnimatedTexture
	{
	private:
		SDL_Texture* texture = nullptr;
		Rect currentFrame;
	};

	class Text
	{
	public:
		Text();
		Text(const char* filename, float size, const char* string);
		Text(const char* filename, float size, const char* string, int windowIndex);

		void render(float x, float y) const;

		int getw() const;
		int geth() const;

		float getfs() const;
		void setfs(float size);

		void setstr(const char* str);

		Color getc() const;
		void setc(Color color);

	private:
		TTF_Text *text = nullptr;
	
		bool create(const char* filename, float size, const char* string);
		bool create(const char* filename, float size, const char* string, int windowIndex);
	};
}