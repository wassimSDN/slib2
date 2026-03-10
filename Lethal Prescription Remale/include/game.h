#pragma once

#include <slib.h>

#include "ui.h"
#include "player.h"

namespace LethalPrescription
{
	using namespace slib;

	struct Game;
	struct Scene
	{
		virtual void enter(Game*game) {}
		virtual void exit(Game*game) {}
		virtual void update(Game* game) {}
		virtual void fixedUpdate(Game* game) {}
		virtual void render(Game* game) {}
		virtual void load(Game* game) {}
	};

	struct LoadingScreen : public Scene
	{
		void enter(Game* game) override;
		void render(Game* game) override;		
		void exit(Game* game) override;
		void update(Game* game) override;
		void fixedUpdate(Game* game) override;
		void load(Game* game) override;

		Texture frames[8];
		long double accumulatedTime = 0;
		int currentFrame = 0;

		Text pressAnyKey;
		Color textColor;
		int nbrTicks;
		int ticksToWait;
	};

	enum class MainMenuButtonLabels
	{
		play,
		options,
		highscores,
		exit
	};
	struct MainMenu : public Scene
	{
		void enter(Game* game) override;
		void render(Game* game) override;
		void exit(Game* game) override;
		void update(Game* game) override;
		void fixedUpdate(Game* game) override;
		void load(Game* game) override;
		
		Text title;
		Texture backgroundFrames[24];
		Rect backgroundPos;
		long double accumulatedTime = 0;
		int currentFrame = 0;

		Button buttons[4];
	};
	
	enum class OptionsMenuSliderLabels
	{
		sound,
		music
	};
	enum class OptionsMenuButtonLabels
	{
		fullscreen,
		windowed,
		back
	};
	struct OptionsMenu : public Scene
	{
		void enter(Game* game) override;
		void render(Game* game) override;
		void exit(Game* game) override;
		void update(Game* game) override;
		void fixedUpdate(Game* game) override;

		Slider sliders[2];
		Button buttons[3];

		Texture document[9];
		long double accumulatedTime = 0;
		int currentFrame = 0;
		int adder = 1;
		Rect docPos;

		Text title;
		Text slidersText[2];
	};

	enum class PlayerClassLabels
	{
		brute,
		diplomat,
		sensitive
	};
	struct CharacterMenu : public Scene
	{
		void enter(Game* game) override;
		void render(Game* game) override;
		void exit(Game* game) override;
		void update(Game* game) override;
		void fixedUpdate(Game* game) override;
		void load(Game* game) override;

		Button classButtons[3];

		Texture document[9];
		long double accumulatedTime = 0;
		int currentFrame = 0;
		int adder = 1;

	};

	struct GamePlay : public Scene
	{
		void enter(Game* game) override;
		void render(Game* game) override;
		void exit(Game* game) override;
		void update(Game* game) override;
		void fixedUpdate(Game* game) override;
	
		Player player;
	};

	struct Game
	{
		Game();
		~Game();
		slib::App game;

		MainMenu mainMenu;
		OptionsMenu optionsMenu;
		LoadingScreen loadingScreen;
		GamePlay gameplay;
		//CharacterMenu characterMenu;
		int chosenSprite;

		Sound menuMusic;
		Sound buttonSound;

		Scene* currentScene = &gameplay;
		Scene* previousScene = nullptr;
	};
}