#if _DEBUG
#include <iostream>
#endif

#include "../include/slibrender.h"
#include  "../include/slibapp.h"

namespace slib
{
	Surface::Surface()
	{
		surface = missingSurface();
	}

	Surface::Surface(const char* filename)
	{
		createFromImage(filename);
	}

	Surface::Surface(Color color)
	{
		surface = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
		if (!surface)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		static_cast<uint32_t*>(surface->pixels)[0] = color.r << 24 | color.g << 16 | color.b << 8 | color.a;
	}

	Surface::~Surface()
	{
		if (!surface)
			SDL_DestroySurface(surface);
	}

	bool Surface::load(const char* filename)
	{
		createFromImage(filename);

		return !missing;
	}

	SDL_Surface* Surface::get()
	{
		return surface;
	}

	void Surface::createFromImage(const char* filename)
	{
		if (surface)
			SDL_DestroySurface(surface);

		surface = IMG_Load(filename);
		if (surface)
			return;
		
		surface = missingSurface();
		missing = true;
		
#if _DEBUG
		std::cout << SDL_GetError() << '\n';
#endif
	}

	bool Surface::isMissing() const
	{
		return missing;
	}

	SDL_Surface *missingSurface()
	{
		SDL_Surface* result = SDL_CreateSurface(50, 50, SDL_PIXELFORMAT_RGBA8888);
		if (!result)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return nullptr;
		}

		uint32_t *pixels = static_cast<uint32_t*>(result->pixels);

		for (int i = 0; i < result->w; i++)
		{
			for (int j = 0; j < result->h; j++)
			{
				if (i % 2 == j % 2 == 0)
				{
					pixels[i * result->w + j] = 255 << 24 | 0 << 16 | 255 << 8 | 200;
				}
				else
				{
					pixels[i * result->w + j] = 0;
				}
			}
		}

		return result;
	}

	Texture::Texture()
	{
		texture = SDL_CreateTextureFromSurface(App::mainWindow.renderer, missingSurface());
#if _DEBUG
		std::cout << SDL_GetError() << '\n';
#endif
	}

	Texture::Texture(const char* filename)
	{
		createFromImage(filename);
	}

	Texture::Texture(const char* filename, int windowIndex)
	{
		createFromImage(filename, windowIndex);
	}

	Texture::Texture(Color color)
	{
		Surface surface = color;
		texture = SDL_CreateTextureFromSurface(App::mainWindow.renderer, surface.get());
	}

	Texture::Texture(Color color, int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= App::secondaryWindows.size())
			return;

		Surface surface = color;
		texture = SDL_CreateTextureFromSurface(App::secondaryWindows[windowIndex].renderer, surface.get());
	}

	Texture::~Texture()
	{
		if (texture)
			SDL_DestroyTexture(texture);
	}

	bool Texture::createFromImage(const char* filename)
	{
		if (texture)
			SDL_DestroyTexture(texture);

		Surface surface = filename;
		texture = SDL_CreateTextureFromSurface(App::mainWindow.renderer, surface.get());

		return !surface.isMissing();
	}

	bool Texture::createFromImage(const char* filename, int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= App::secondaryWindows.size())
			return false;

		if (texture)
			SDL_DestroyTexture(texture);

		Surface surface = filename;
		texture = SDL_CreateTextureFromSurface(App::secondaryWindows[windowIndex].renderer, surface.get());

		return !surface.isMissing();
	}

	bool Texture::load(const char* filename)
	{
		return createFromImage(filename);
	}

	bool Texture::load(const char* filename, int windowIndex)
	{
		return createFromImage(filename, windowIndex);
	}

	bool Texture::createAsTarget(int w, int h)
	{
		if (texture)
			SDL_DestroyTexture(texture);

		texture = SDL_CreateTexture(App::mainWindow.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if (texture)
			return true;

		SDL_CreateTextureFromSurface(App::mainWindow.renderer, missingSurface());

		return false;
	}

	bool Texture::createAsTarget(int w, int h, int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= App::secondaryWindows.size())
			return false;

		texture = SDL_CreateTexture(App::secondaryWindows[windowIndex].renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if (texture)
			return true;

		SDL_CreateTextureFromSurface(App::mainWindow.renderer, missingSurface());

		return false;
	}

	void Texture::render(Rect dst) const
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };

		SDL_RenderTexture(SDL_GetRendererFromTexture(texture), texture, nullptr, &dstrect);
	}
	void Texture::render(Rect dst, Rect src) const
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };
		SDL_FRect srcrect = { src.getx(), src.gety(), src.getw(), src.geth() };

		SDL_RenderTexture(SDL_GetRendererFromTexture(texture), texture, &srcrect, &dstrect);
	}
	void Texture::render(float angle, Rect dst) const
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };

		SDL_RenderTextureRotated(SDL_GetRendererFromTexture(texture), texture, nullptr, &dstrect, angle, nullptr, SDL_FLIP_NONE);
	}
	void Texture::render(float angle, Rect dst, Rect src) const
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };
		SDL_FRect srcrect = { src.getx(), src.gety(), src.getw(), src.geth() };

		SDL_RenderTextureRotated(SDL_GetRendererFromTexture(texture), texture, &srcrect, &dstrect, angle, nullptr, SDL_FLIP_NONE);
	}


	Text::Text(const char* filename, float size, const char* string)
	{
		create(filename, size, string);
	}
	
	Text::Text(const char* filename, float size, const char* string, int windowIndex)
	{
		create(filename, size, string, windowIndex);
	}

	void Text::render(float x, float y) const
	{
		
#if _DEBUG
		if (!TTF_DrawRendererText(text, x, y))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		TTF_DrawRendererText(text, x, y);
#endif
	}

	int Text::getw() const
	{
		int w;
		TTF_GetTextSize(text, &w, nullptr);

		return w;
	}

	int Text::geth() const
	{
		int h;
		TTF_GetTextSize(text, nullptr, &h);

		return h;
	}

	float Text::getfs() const
	{
		if (!text)
			return -1;

		return TTF_GetFontSize(TTF_GetTextFont(text));
	}

	void Text::setfs(float size)
	{
		if (!text)
			return;

		TTF_SetFontSize(TTF_GetTextFont(text), size);
	}

	void Text::setstr(const char* str)
	{
		TTF_SetTextString(text, str, std::strlen(str));
	}

	Color Text::getc() const
	{	
		Color color;
		if (!TTF_GetTextColor(text, &color.r, &color.g, &color.b, &color.a))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return { 0, 0, 0, 0 };
		}

		return color;
	}

	void Text::setc(Color color)
	{
#if _DEBUG
		if (!TTF_SetTextColor(text, color.r, color.g, color.b, color.a))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
#endif
	}

	bool Text::create(const char *filename, float size, const char *string)
	{
		TTF_Font* font = TTF_OpenFont(filename, size);	
		if (!font)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		text = TTF_CreateText(App::mainWindow.textEngine, font, string, std::strlen(string));
		if (!text)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		return true;
	}

	bool Text::create(const char* filename, float size, const char* string, int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= App::secondaryWindows.size())
			return false;

		TTF_Font* font = TTF_OpenFont(filename, size);
		if (!font)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		text = TTF_CreateText(App::secondaryWindows[windowIndex].textEngine, font, string, std::strlen(string));
		if (!text)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		return true;
	}
}