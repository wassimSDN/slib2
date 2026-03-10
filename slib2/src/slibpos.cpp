#include <cmath>
#if _DEBUG
#include <string>
#endif

#include "../include/slibpos.h"
#include "../include/slibapp.h"
#include "../include/slibmisc.h"

namespace slib
{

	Vector2 Vector2::normalise()
	{
		float length = std::sqrtf(x * x + y * y);
		if (length > 0.0f)
		{
			x /= length;
			y /= length;
		}
		
		
		return *this;
	}

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
#if _DEBUG
		if (App::editorMode)
			return;
#endif
		pos += p;
	}
	void Rect::addSize(Dimensions s)
	{
#if _DEBUG
		if (App::editorMode)
			return;
#endif
		size.w += s.w;
		size.h += s.h;
	}

	void Rect::set(Vector2 p, Dimensions s)
	{
#if _DEBUG
		if (App::editorMode)
			return;
#endif
		pos = p;
		size = s;
	}
	void Rect::setPos(Vector2 p)
	{
#if _DEBUG
		if (App::editorMode)
			return;
#endif
		pos = p;
	}
	void Rect::setSize(Dimensions s)
	{
#if _DEBUG
		if (App::editorMode)
			return;
#endif
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
	bool Rect::pointIn(Vector2 point) const
	{
		SDL_FPoint p = { point.x, point.y };
		SDL_FRect rect = { pos.x, pos.y, size.w, size.h };
		return SDL_PointInRectFloat(&p, &rect);
	}

	bool Rect::mouseIn() const
	{
		return pointIn({ App::mousex(), App::mousey() });
	}

	bool Rect::clicked() const
	{
#if _DEBUG
		if(App::editorMode)
			return false;
		
		return mouseIn() && App::mouseDown(Buttons::left);
#else
		return mouseIn() && App::mouseDown(Buttons::left);
#endif
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
#if _DEBUG
	void Rect::renderDebug()
	{
		if (!App::renderHitBoxes)
			return;
		if (index < 0)
		{
			index = nbr;
			nbr++;
		}



		//return;
		Color c;
		if (mouseIn())
		{
			if (App::mouseDown(Buttons::left))
			{
				c = { 0, 255, 0, 255 };
				if(App::editorMode && App::mouseJustDown(Buttons::left) && editable && !otherHeld)
				{
					held = true;
					otherHeld = true;
				}
			}
			else
			{
				c = { 0, 0, 255, 255 };
			}

			if (App::keyDown(Keys::lshift) && App::keyJustDown(Keys::w) && !otherHeld)
			{
				editable = !editable;
			}
			if (editable)
			{
				App::debugString += "box X: " + std::to_string(pos.x) + " ";
				App::debugString += "box y: " + std::to_string(pos.y) + "\n";
				App::debugString += "box W: " + std::to_string(size.w) + " ";
				App::debugString += "box H: " + std::to_string(size.h) + "\n";
			}
		}
		if(App::editorMode)
		{
			c = { 0, 128, 0, 255 };
		}
		else
		{
			c = { 255, 0, 0, 255 };
		}

		if (held)
		{
			if (editable)
			{
				pos.x = App::mousex() - size.w / 2;
				pos.y = App::mousey() - size.h / 2;
			}
			if (App::mouseJustUp(Buttons::left))
			{
				held = false;
				otherHeld = false;
			}
		}


		render(c);
	}
#endif

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

#if _DEBUG
	bool Rect::otherHeld = false;
	int Rect::nbr = 0; 
	int Rect::hoveredIndex = -1;
#endif
}