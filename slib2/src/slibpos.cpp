#include "../include/slibpos.h"
#include "../include/slibapp.h"
#include "../include/slibmisc.h"

namespace slib
{
	Vector2 operator+(const Vector2 first, const Vector2 second)
	{
		return { first.x + second.x, first.y + second.y };
	}

	Vector2 operator-(const Vector2 first, const Vector2 second)
	{
		return { first.x - second.x, first.y - second.y };
	}

	Vector2 operator+=(Vector2 &first, Vector2 second)
	{
		first.x += second.x;
		first.y += second.y;

		return first;
	}

	Vector2 operator*(const float scalar, const Vector2 v)
	{
		return { v.x * scalar, v.y * scalar };
	}

	Vector2 operator*(const Vector2 v, const float scalar)
	{
		return { v.x * scalar, v.y * scalar };
	}

	Rect::Rect(Vector2 p, Dimensions s)
	{
		pos = p;
		size = s;
	}
	Rect::Rect(const Rect& other)
	{
		pos = other.pos;
		size = other.size;
	}

	void Rect::addPos(Vector2 p)
	{
		pos += p;
	}
	void Rect::addSize(Dimensions s)
	{
		size.w += s.w;
		size.h += s.h;
	}

	void Rect::set(Vector2 p, Dimensions s)
	{
		pos = p;
		size = s;
	}
	void Rect::setPos(Vector2 p)
	{
		pos = p;
	}
	void Rect::setSize(Dimensions s)
	{
		size = s;
	}

	bool Rect::collision(const Rect& other) const
	{
		SDL_FRect first = { pos.x, pos.y, size.w, size.h };
		SDL_FRect second = { other.pos.x, other.pos.y, other.size.w, other.size.h };
		
		return SDL_HasRectIntersectionFloat(&first, &second);
	}

	Rect Rect::intersection(const Rect& other) const
	{
		SDL_FRect first = { pos.x, pos.y, size.w, size.h };
		SDL_FRect second = { other.pos.x, other.pos.y, other.size.w, other.size.h };
		SDL_FRect result;
		SDL_GetRectIntersectionFloat(&first, &second, &result);

		return { {result.x, result.y}, {result.w, result.h} };
	}

	Rect Rect::get() const { return *this; }
	Vector2 Rect::getPos() const { return pos; }
	Dimensions Rect::getSize() const { return size; }

	float Rect::getx() const { return pos.x; }
	float Rect::gety() const { return pos.y; }
	float Rect::getw() const { return size.w; }
	float Rect::geth() const { return size.h; }
	float Rect::getr() const { return pos.x + size.w; }
	float Rect::getb() const { return pos.y + size.h; }

	bool Rect::empty() const { return size.h < 0 || size.w < 0; }

	void Rect::render(Color color) const
	{ 
		if (empty())
			return;

		Color previous = App::mainWindow.getRenderColor();

		App::mainWindow.setRenderColor(color);
		SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
		SDL_RenderRect(App::mainWindow.renderer, &rect);

		App::mainWindow.setRenderColor(previous);
	}
	void Rect::renderFill(Color color) const
	{
		if (empty())
			return;

		Color previous = App::mainWindow.getRenderColor();

		App::mainWindow.setRenderColor(color);
		SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
		SDL_RenderFillRect(App::mainWindow.renderer, &rect);

		App::mainWindow.setRenderColor(previous);
	}

	void Rect::render(int windowIndex,	Color color) const
	{
		if (empty())
			return;
		
		if (windowIndex >= App::secondaryWindows.size())
			return;

		Color previous = App::secondaryWindows[windowIndex].getRenderColor();

		App::secondaryWindows[windowIndex].setRenderColor(color);
		SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
		SDL_RenderRect(App::secondaryWindows[windowIndex].renderer, &rect);

		App::secondaryWindows[windowIndex].setRenderColor(previous);
	}
	void Rect::renderFill(int windowIndex, Color color) const
	{
		if (empty())
			return;

		if (windowIndex >= App::secondaryWindows.size())
			return;

		Color previous = App::secondaryWindows[windowIndex].getRenderColor();

		App::secondaryWindows[windowIndex].setRenderColor(color);
		SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
		SDL_RenderFillRect(App::secondaryWindows[windowIndex].renderer, &rect);

		App::secondaryWindows[windowIndex].setRenderColor(previous);
	}
}