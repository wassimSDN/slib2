#include "../include/ui.h"
#include "../include/game.h"

namespace LethalPrescription
{
	bool Button::isClicked() const
	{
		return state == ButtonState::clicked;
	}
	bool Button::isHovered() const
	{
		return state == ButtonState::hovered;
	}
	void Button::update(Game* game)
	{
		switch (state)
		{
		case ButtonState::unhovered:
			if (pos.pointIn({ game->game.mousex(), game->game.mousey() }))
			{
				if (state != ButtonState::hovered)
					game->buttonSound.play();

				state = ButtonState::hovered;
			}
			break;
		case ButtonState::hovered:
			if (pos.pointIn({game->game.mousex(), game->game.mousey()}))
			{
				if (pos.clicked())
				{
					state = ButtonState::clicked;
				}
				else
					state = ButtonState::hovered;
			}
			else
				state = ButtonState::unhovered;

			break;
		case ButtonState::clicked:
			if (!pos.pointIn({ game->game.mousex(), game->game.mousey() }))
			{
				state = ButtonState::unhovered;
			}
			break;
		default:
			break;
		}
	}

	void Button::render(Game* game)
	{
		Vector2 labelPos;
		labelPos.x = pos.getx() + (pos.getw() - label.getw()) / 2 - offcenterx;
		labelPos.y = pos.gety() + (pos.geth() - label.geth()) / 2;

		texture[static_cast<int>(state)].render(pos);
		label.render(labelPos.x, labelPos.y);
	}

	void Button::destroy()
	{
		for (Texture& it : texture)
			it.destroy();
		label.destroy();
	}

	void Slider::update(Game* game)
	{
		if (game->game.mouseJustDown(Buttons::left) && nobPos.pointIn({ game->game.mousex(), game->game.mousey() }) && !held)
		{
			held = true;
		}
		else if (game->game.mouseJustUp(Buttons::left))
			held = false;

		if (!held)
			return;

		if (game->game.mousex() < railPos.getx() || game->game.mousex() > railPos.getr())
			return;

		nobPos.setPos({ game->game.mousex() - nobPos.getw() / 2, nobPos.gety() });
	}

	float Slider::getValue() const
	{
		return (nobPos.getx() - railPos.getx() + nobPos.getw() / 2) / railPos.getw()  ;
	}

	void Slider::render(Game* game)
	{
		rail.render(railPos);
		nob.render(nobPos);
	}

	void Slider::destroy()
	{
		nob.destroy();
		rail.destroy();
	}
}