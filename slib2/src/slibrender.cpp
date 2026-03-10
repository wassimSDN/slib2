#if _DEBUG
#include <iostream>
#include <assert.h>
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
		if (surface)
			SDL_DestroySurface(surface);

		surface = nullptr;
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

	static int missingSize = 50;

	SDL_Surface *missingSurface()
	{
		SDL_Surface* result = SDL_CreateSurface(missingSize, missingSize, SDL_PIXELFORMAT_RGBA8888);
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
#if _DEBUG
		if (!texture)
			std::cout << SDL_GetError() << '\n';
#endif

		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

		if (surface.isMissing())
			SDL_SetRenderTextureAddressMode(SDL_GetRendererFromTexture(texture), SDL_TEXTURE_ADDRESS_WRAP, SDL_TEXTURE_ADDRESS_WRAP);

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
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

		if (surface.isMissing())
			SDL_SetRenderTextureAddressMode(SDL_GetRendererFromTexture(texture), SDL_TEXTURE_ADDRESS_WRAP, SDL_TEXTURE_ADDRESS_WRAP);

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
		SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

#if _DEBUG
		std::cout << SDL_GetError() << '\n';
#endif

		return false;
	}

	bool Texture::createAsTarget(int w, int h, int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= App::secondaryWindows.size())
			return false;

		texture = SDL_CreateTexture(App::secondaryWindows[windowIndex].renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		if (texture)
			return true;

#if _DEBUG
		std::cout << SDL_GetError() << '\n';
#endif
		return false;
	}

	void Texture::destroy()
	{
		if (texture)
			SDL_DestroyTexture(texture);
		
		texture = nullptr;
	}

	void Texture::render(Rect &dst)
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };

#if _DEBUG
		SDL_Renderer* rend = SDL_GetRendererFromTexture(texture);
		if(!rend)
		{
			std::cout << SDL_GetError() << '\n';
			return;
		}
		if (App::renderTextures)
			if (!SDL_RenderTexture(rend, texture, nullptr, &dstrect))
				std::cout << SDL_GetError() << '\n';
		dst.renderDebug();
#else
		SDL_RenderTexture(SDL_GetRendererFromTexture(texture), texture, nullptr, &dstrect);
#endif
	}
	void Texture::render(Rect &dst, Rect src)
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };
		SDL_FRect srcrect = { src.getx(), src.gety(), src.getw(), src.geth() };

#if _DEBUG
		SDL_Renderer* rend = SDL_GetRendererFromTexture(texture);
		if (!rend)
		{
			std::cout << SDL_GetError() << '\n';
			return;
		}
		if (App::renderTextures)
			if (!SDL_RenderTexture(rend, texture, &srcrect, &dstrect))
				std::cout << SDL_GetError() << '\n';

		dst.renderDebug();
#else
		SDL_RenderTexture(SDL_GetRendererFromTexture(texture), texture, &srcrect, &dstrect);
#endif
	}
	void Texture::render(float angle, Rect &dst)
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };
#if _DEBUG
		SDL_Renderer* rend = SDL_GetRendererFromTexture(texture);
		if (!rend)
		{
			std::cout << SDL_GetError() << '\n';
			return;
		}
		if (App::renderTextures)
			if(!SDL_RenderTextureRotated(rend, texture, nullptr, &dstrect, angle, nullptr, SDL_FLIP_NONE))
				std::cout << SDL_GetError() << '\n';

		dst.renderDebug();
#else
		SDL_RenderTextureRotated(SDL_GetRendererFromTexture(texture), texture, nullptr, &dstrect, angle, nullptr, SDL_FLIP_NONE);
#endif
	}
	void Texture::render(float angle, Rect &dst, Rect src)
	{
		SDL_FRect dstrect = { dst.getx(), dst.gety(), dst.getw(), dst.geth() };
		SDL_FRect srcrect = { src.getx(), src.gety(), src.getw(), src.geth() };

#if _DEBUG
		SDL_Renderer* rend = SDL_GetRendererFromTexture(texture);
		if (!rend)
		{
			std::cout << SDL_GetError() << '\n';
			return;
		}
		if (App::renderTextures)
			if (!SDL_RenderTextureRotated(rend, texture, &srcrect, &dstrect, angle, nullptr, SDL_FLIP_NONE))
				std::cout << SDL_GetError() << '\n';

		dst.renderDebug();
#else
		SDL_RenderTextureRotated(SDL_GetRendererFromTexture(texture), texture, &srcrect, &dstrect, angle, nullptr, SDL_FLIP_NONE);
#endif
	}
	float Texture::getw() const
	{
		float w;
		if (!SDL_GetTextureSize(texture, &w, nullptr))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return -1.0f;
		}

		return w;

	}
	float Texture::geth() const
	{
		float h;
		if (!SDL_GetTextureSize(texture, nullptr, &h))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return -1.0f;
		}

		return h;
	}


	Text::Text(const char* filename, float size, const char* string)
	{
		create(filename, size, string);
	}
	
	Text::Text(const char* filename, float size, const char* string, int windowIndex)
	{
		create(filename, size, string, windowIndex);
	}

	Text::~Text()
	{
		destroy();
	}

	bool Text::load(const char* filename, float size, const char* string)
	{
		return create(filename, size, string);
	}
	bool Text::load(const char* filename, float size, const char* string, int windowIndex)
	{
		return create(filename, size, string, windowIndex);
	}
	bool Text::createAsDebug()
	{
		if (text)
			destroy();
		
		TTF_Font* font = missingFont(20);
		if (!font)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}


		char debug[] = "Debug";
		text = TTF_CreateText(App::mainWindow.textEngine, font, debug, std::strlen(debug));
		if (!text)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			TTF_CloseFont(font);
			return false;
		}

		return true;
	}

	void Text::destroy()
	{
		if (text)
		{
			TTF_CloseFont(TTF_GetTextFont(text));
			TTF_DestroyText(text);
		}

		text = nullptr;
	}

	void Text::render(float x, float y) const
	{
		
#if _DEBUG
		if(App::renderTextures)
		{
			if (!TTF_DrawRendererText(text, x, y))
			{
				std::cout << SDL_GetError() << '\n';
				return;
			}
		}
		if (App::renderHitBoxes)
		{
			int  w, h;
			if (!TTF_GetTextSize(text, &w, &h))
			{
				std::cout << SDL_GetError() << '\n';
				return;
			}
			Rect hitbox = { {x, y}, {static_cast<float>(w), static_cast<float>(h)} };
			hitbox.renderDebug();
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
		if (text)
			destroy();
		
		TTF_Font* font = TTF_OpenFont(filename, size);	
		if (!font)
		{
		
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			font = missingFont(size);
#if _DEBUG
			if (!font)
			{
				std::cout << SDL_GetError() << '\n';
				return false;
			}
#endif
		}

		text = TTF_CreateText(App::mainWindow.textEngine, font, string, std::strlen(string));
		if (!text)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			TTF_CloseFont(font);
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
			font = missingFont(size);
#if _DEBUG
			if (!font)
			{
				std::cout << SDL_GetError() << '\n';
				return false;
			}
#endif
		}

		text = TTF_CreateText(App::secondaryWindows[windowIndex].textEngine, font, string, std::strlen(string));
		if (!text)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			TTF_CloseFont(font);
			return false;
		}

		return true;
	}

	AnimatedTexture::AnimatedTexture(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding)
	{
		create(filename, nbrFrames, fps, x, y, w, h, padding);
	}
	AnimatedTexture::AnimatedTexture(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int windowIndex)
	{
		create(filename, nbrFrames, fps, x, y, w, h, padding, windowIndex);
	}

	bool AnimatedTexture::finishedPlaying()
	{
		return currentFrame.getx() == currentFrame.getw() * (nbrFrames - 1) + startX + padding * (nbrFrames - 1);
	}

	void AnimatedTexture::playOnce()
	{
		once = true;
		//currentFrame.setPos({ startX, startY });
	}

	void AnimatedTexture::playRepeat()
	{
		once = false;
	}

	void AnimatedTexture::update()
	{
		if (once && finishedPlaying())
			return;

		accumulatedTime += App::time.deltaTime;

		while (accumulatedTime >= frameTime)
		{
			currentFrame.addPos({ currentFrame.getw() + padding, 0 });
			if (currentFrame.getx() >= currentFrame.getw() * nbrFrames + startX + padding * (nbrFrames - 1))
				currentFrame.setPos({startX, currentFrame.gety()});

			accumulatedTime -= frameTime;
		}
	}

	void AnimatedTexture::render(Rect &dstrect)
	{
		texture.render(dstrect, currentFrame);
	}

	bool AnimatedTexture::load(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding)
	{
		return create(filename, nbrFrames, fps, x, y, w, h, padding);
	}
	bool AnimatedTexture::load(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int windowIndex)
	{
		return create(filename, nbrFrames, fps, x, y, w, h, padding, windowIndex);
	}

	bool AnimatedTexture::create(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding)
	{
		if (!texture.load(filename))
		{
			//fix

			return false;
		}
		
		setup(nbrFrames, fps, x, y, w, h, padding);

		return true;
	}

	bool AnimatedTexture::create(const char* filename, int nbrFrames, float fps, float x, float y, float w, float  h, float padding, int WindowIndex)
	{
		if (!texture.load(filename, WindowIndex))
		{
			//fix

			return false;
		}

		setup(nbrFrames, fps, x, y, w, h, padding);

		return true;
	}

	void AnimatedTexture::setup(int nbrFrames, float fps, float x, float y, float w, float  h, float padding)
	{
		setfps(fps);
		setNbrFrames(nbrFrames);
		setsx(x);
		setsy(y);
		setfw(w);
		setfh(h);
		setp(padding);
		currentFrame.setPos({ startX, startY });
	}


	void AnimatedTexture::setsx(float x)
	{
		if (x >= 0 && x < texture.getw())
		{
			startX = x;
		}
#if _DEBUG
		else
			std::cout << "Start X out of range\n";
#endif
	}
	void AnimatedTexture::setsy(float y)
	{
		if (y >= 0 && y < texture.geth())
		{
			startY = y;
		}
#if _DEBUG
		else
			std::cout << "Start Y out of range\n";
#endif
	}
	void AnimatedTexture::setfw(float w)
	{
		if (w > 0 && w < texture.getw())
			currentFrame.setSize({ w, currentFrame.geth() });

		else
		{
			currentFrame.setSize({ static_cast<float>(missingSize), currentFrame.geth() });
#if _DEBUG
			std::cout << "Frame W out of range\n";
#endif
		}

	}
	void AnimatedTexture::setfh(float h)
	{
		if (h > 0 && h < texture.geth())
			currentFrame.setSize({ currentFrame.getw(), h });
		else
		{
			currentFrame.setSize({ currentFrame.getw(), static_cast<float>(missingSize) });
#if _DEBUG
			std::cout << "Frame H out of range\n";
#endif
		}
	}
	void AnimatedTexture::setfps(float fps)
	{
		if (fps > 0)
			frameTime = 1.0f / fps;
#if _DEBUG
		else
			std::cout << "FPS must be Greater Than 0\n";
#endif
	}
	void AnimatedTexture::setNbrFrames(int nbrFrames)
	{
		if (nbrFrames > 0)
			this->nbrFrames = nbrFrames;
#if _DEBUG
		else
			std::cout << "NBRFRAMES must be Greater Than 0\n";
#endif
	}

	void AnimatedTexture::setp(float padding)
	{
		if (padding >= 0)
			this->padding = padding;
#if _DEBUG
		else
			std::cout << "Padding must be Positive\n";
#endif
	}

	void AnimatedTexture::reset()
	{
		currentFrame.setPos({ startX, currentFrame.gety() });
		currentFrame.setPos({ currentFrame.getx(), startY });
	}

	void renderPoint(Vector2 point, Color color)
	{
		Color previousColor = App::mainWindow.getRenderColor();
		App::mainWindow.setRenderColor(color);

#if _DEBUG
		if (!SDL_RenderPoint(App::mainWindow.renderer, point.x, point.y))
			std::cout << SDL_GetError() << '\n';
#else
		SDL_RenderPoint(App::mainWindow.renderer, point.x, point.y);
#endif

		App::mainWindow.setRenderColor(previousColor);
	}
}