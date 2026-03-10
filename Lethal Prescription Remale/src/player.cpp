#include "../include/player.h"
#include "../include/game.h"

namespace LethalPrescription
{
	void Player::init(Game* game)
	{
		if (currentSprite == PlayerSprite::brown)
			sprite.load("assets/SpriteSheets/jack.png", 4, 4, 0, 352, 240, 288, 0);
		else if (currentSprite == PlayerSprite::white)
			sprite.load("assets/SpriteSheets/Jojo.png", 4, 4, 0, 352, 240, 288, 0);

		pos = { {50, 50}, {240, 288} };
		velocity = { 0 };
		speed = 0;
	}
	void Player::update(Game* game)
	{
		previousDirection = currentDirection;
		previousState = currentState;
		velocity = { 0 };
		speed = 0;
		sprite.update();
		if (game->game.keyDown(Keys::a))
		{
			velocity = { -1, 0 };
			currentDirection = PlayerDirection::left;
			if(game->game.keyDown(Keys::lshift))
			{
				currentState = PlayerState::run;
			}
			else
			{
				currentState = PlayerState::walk;
			}
		}
		if (game->game.keyDown(Keys::d))
		{
			velocity = { 1, 0 };
			currentDirection = PlayerDirection::right;
			if (game->game.keyDown(Keys::lshift))
			{
				currentState = PlayerState::run;
			}
			else
			{
				currentState = PlayerState::walk;
			}
		}

		if (previousDirection != currentDirection)
		{
			if(currentDirection == PlayerDirection::left)
			{
				sprite.setsx(2900);
			}
			else
			{
				sprite.setsx(0);
			}
			sprite.reset();
		}

		switch (currentState)
		{
		case PlayerState::walk:
			speed = 400.0f;
			break;
		case PlayerState::run:
			speed = 600.0f;
			break;
		default:
			speed = 0.0f;
			break;
		}

		Vector2 w = velocity.normalise() * speed * game->game.getdt();
		pos.addPos(w);
		App::debugString += "Player vx: " + std::to_string(w.x) + " Player vy: " + std::to_string(w.x) + "\n";
	}
	void Player::fixedUpdate(Game* game)
	{

	}
	void Player::render(Game* game)
	{
		sprite.render(pos);
	}
	void Player::destroy(Game* game)
	{

	}
}