#pragma once

#include <slib.h>

namespace LethalPrescription
{
	using namespace slib;
	enum class ButtonState
	{
		unhovered,
		hovered,
		clicked
	};

	struct Game;
	struct Button
	{
		ButtonState state = ButtonState::unhovered;
		Texture texture[3];
		Rect pos;
		Text label;

		bool isClicked() const;
		bool isHovered() const;
		void update(Game *game);
		void render(Game* game);
		void destroy();

		float offcenterx = 0.0f;
		float offcentery = 0.0f;
	};

	struct Slider
	{
		Texture rail, nob;
		Rect railPos, nobPos;
		bool held = false;

		float getValue() const;

		void update(Game* game);
		void render(Game* game);
		void destroy();
	};
}