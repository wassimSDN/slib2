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

		float geth() const;
		float getw() const;

	private:
		SDL_Texture* texture = nullptr;
		bool createFromImage(const char* filename);
		bool createFromImage(const char* filename, int windowIndex);
	};

	class AnimatedTexture
	{
	public:
		AnimatedTexture();
		AnimatedTexture(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding);
		AnimatedTexture(const char* fileName, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int windowIndex);

		bool load(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding);
		bool load(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int windowIndex);

		void setsx(float x);
		void setsy(float y);
		void setfw(float w);
		void setfh(float h);
		void setfps(float fps);
		void setNbrFrames(int nbrFrames);

		bool finishedPlaying();

		void update();
		void render(Rect dstrect);

	private:
		Texture texture;
		Rect currentFrame;
		float frameTime;
		long double accumulatedTime = 0.0f;
		int nbrFrames = 0;
		float startX = 0;
		float startY = 0;
		float padding = 0;

		bool create(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding);
		bool create(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int windowIndex);
		void setup(int nbrFrames, float fps, float x, float y, float w, float  h, float padding);
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