#pragma once

#include <cstdint>

#include "slibmisc.h"

namespace slib
{
	struct Vector2
	{
		float x = 0;
		float y = 0;
	};

	struct Dimensions
	{
		float w = 0;
		float h = 0;
	};

	Vector2 operator+(const Vector2 first, const Vector2 second);
	Vector2 operator-(const Vector2 first, const Vector2 second);
	Vector2 operator+=(Vector2 &first, Vector2 second);
	Vector2 operator*(const float scalar, const Vector2 v);
	Vector2 operator*(const Vector2 v, const float scalar);

	class Circle;
	class Rect
	{
	public:
		Rect() = default;
		Rect(Vector2 p, Dimensions s);
		Rect(const Rect& other);
		~Rect() = default;

		void set(Vector2 p, Dimensions s);
		void setPos(Vector2 p);
		void setSize(Dimensions s);

		bool collision(const Rect& other) const;
		bool collision(const Circle& other) const;
		Rect intersection(const Rect& other) const;

		Rect get() const;
		Vector2 getPos() const;
		Dimensions getSize() const;

		void addPos(Vector2 v);
		void addSize(Dimensions s);

		float getx() const;
		float gety() const;
		float getw() const;
		float geth() const;
		float getr() const;
		float getb() const;

		bool empty() const;

		void render(Color color = {255, 0, 0, 255}) const;
		void render(int windowIndex, Color color = { 255, 0, 0, 255 }) const;

		void renderFill(Color color = { 255, 0, 0, 255 }) const;
		void renderFill(int windowIndex, Color color = { 255, 0, 0, 255 }) const;


	private:
		Vector2 pos;
		Dimensions size;
	};

	class Circle
	{
	public:

		float getx() const;
		float gety() const;
		float getr() const;

		bool empty() const;


	private:
		Vector2 center;
		float radius;
	};
}