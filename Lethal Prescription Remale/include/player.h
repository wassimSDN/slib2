#pragma once 

#include <slib.h>

namespace LethalPrescription
{
	using namespace slib;

	enum class PlayerState
	{
		idle,
		walk,
		run,
		jump,
		attack
	};
	enum class PlayerDirection
	{
	   left,
	   right
	};
	enum class PlayerSprite
	{
		brown,
		white
	};
	struct Game;
	struct Player
	{
		AnimatedTexture sprite;
		Rect pos;
		Vector2 velocity;
		float speed;
		PlayerSprite currentSprite;
		PlayerState currentState;
		PlayerState previousState;
		PlayerDirection currentDirection;
		PlayerDirection previousDirection;

		void init(Game *game);
		void update(Game* game);
		void fixedUpdate(Game* game);
		void render(Game* game);
		void destroy(Game* game);
	};
}