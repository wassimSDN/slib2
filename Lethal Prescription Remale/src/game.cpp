#include <string>
#include <iostream>

#include "../include/game.h"

namespace LethalPrescription
{
	void LoadingScreen::enter(Game* game)
	{
		currentFrame = 0;
		accumulatedTime = 0;
		for (int i = 1; i <= 8; i++)
		{
			std::string filepath = "assets/BackGrounds/Game opening (MMC)" + std::to_string(i) + ".png";
			frames[i - 1].load(filepath.c_str());
		}

		pressAnyKey.load("assets/Fonts/BlackStones-ovoXB.ttf", 50, "Press Any Key");
		textColor = { 255, 200, 0, 255 };
		nbrTicks = 0;
		ticksToWait = 40;
	}
	void LoadingScreen::exit(Game* game)
	{
		for (Texture& it : frames)
			it.destroy();

		game->previousScene = this;
	}
	void LoadingScreen::update(Game* game)
	{
		accumulatedTime += game->game.getdt();

		if (currentFrame < 7)
		{
			float frameTime = 1.0f / 8.0f;
			while (accumulatedTime >= frameTime)
			{
				currentFrame++;
				accumulatedTime -= frameTime;
			}
		}
		else
		{
			if (accumulatedTime >= 1.0f)
			{
				if (game->game.keyJustDown(Keys::space) || game->game.mouseJustDown(Buttons::left))
				{
					game->mainMenu.enter(game);
					exit(game);
					game->currentScene = &game->mainMenu;
				}
			}
		}
	}
	void LoadingScreen::fixedUpdate(Game* game)
	{
		nbrTicks++;
		if (nbrTicks >= ticksToWait)
		{
			nbrTicks = 0;
			ticksToWait = ticksToWait == 50 ? 25 : 40;
			textColor.a = textColor.a == 0 ? 255 : 0;
			pressAnyKey.setc(textColor);
		}
	}
	void LoadingScreen::render(Game* game)
	{
		Rect dst = { {0, 0 }, { 1920, 1080 } };
		frames[currentFrame].render(dst);
		if(currentFrame > 6 && accumulatedTime >= 1.0f)
			pressAnyKey.render((1920.0f - pressAnyKey.getw()) / 2.0f, 900.0f);
	}
	void LoadingScreen::load(Game* game)
	{

	}
	
	void MainMenu::enter(Game* game)		
	{
		currentFrame = 0;
		accumulatedTime = 0;
		backgroundPos = { {0, 0}, {1920, 1080} };

		for (int i = 1; i <= 24; i++)
		{
			std::string filepath = "assets/BackGrounds/MainMenu" + std::to_string(i) + ".png";
			backgroundFrames[i - 1].load(filepath.c_str());
		}
	
		title.load("assets/Fonts/TheDark-pr2Z.ttf", 40, "Lethal Prescription");
		title.setc({ 0, 255, 0, 50 });

		char unhover[] = "assets/Buttons/MainMenuButton.png";
		char hover[] = "assets/Buttons/HoverOverMainMenuButton.png";
		char font[] = "assets/Fonts/BlackStones-ovoXB.ttf";
		for (int i = 0; i < 4; i++)
		{
			buttons[i].texture[static_cast<int>(ButtonState::unhovered)].load(unhover);
			buttons[i].texture[static_cast<int>(ButtonState::hovered)].load(hover);
			buttons[i].texture[static_cast<int>(ButtonState::clicked)].load(hover);
			buttons[i].label.load(font, 35, "");
			buttons[i].label.setc({ 0, 255, 0, 128 });
			buttons[i].offcenterx = 30.0f;
		}


		float startX = 150.f;
		float startY = 350.0f;
		float padding = 30.0f;
		float sizeScaler = 1.5f;
		Dimensions size = { buttons[0].texture[static_cast<int>(ButtonState::unhovered)].getw(),
							buttons[0].texture[static_cast<int>(ButtonState::unhovered)].geth() };
		buttons[0].pos.setSize(size);
		buttons[0].pos.setPos({ startX, startY});

		for (int i = 1; i < 4; i++)
		{
			float ceiling = buttons[i - 1].pos.gety();

			size = { buttons[i].texture[static_cast<int>(ButtonState::unhovered)].getw(),
								buttons[i].texture[static_cast<int>(ButtonState::unhovered)].geth() };
			buttons[i].pos.setSize(size);
			buttons[i].pos.setPos({ startX, size.h + padding + ceiling});
		}

		buttons[static_cast<int>(MainMenuButtonLabels::play)].label.setstr("Play");
		buttons[static_cast<int>(MainMenuButtonLabels::options)].label.setstr("Options");
		buttons[static_cast<int>(MainMenuButtonLabels::highscores)].label.setstr("Highscores");
		buttons[static_cast<int>(MainMenuButtonLabels::exit)].label.setstr("exit");
	}
	void MainMenu::update(Game* game)
	{
		accumulatedTime += game->game.getdt();

		float frameTime;
		if (currentFrame == 0)
			frameTime = 2.0f;
		else
			frameTime = 1.0f / 24.0f;

		for (Button& it : buttons)
		{
			it.update(game);
		}
		
		while (accumulatedTime >= frameTime)
		{
				currentFrame++;
				accumulatedTime -= frameTime;
				if (currentFrame >= 24)
				{
					currentFrame = 0;
				}
		}

		if (buttons[static_cast<int>(MainMenuButtonLabels::exit)].isClicked())
		{
			SDL_Event e = {};
			e.type = SDL_EVENT_WINDOW_CLOSE_REQUESTED;
			e.window.windowID = game->game.getWindow()->ID;

			SDL_PushEvent(&e);
		}
		if (buttons[static_cast<int>(MainMenuButtonLabels::options)].isClicked())
		{
			game->optionsMenu.enter(game);
			exit(game);
			game->currentScene = &game->optionsMenu;
		}

	}
	void MainMenu::fixedUpdate(Game* game)
	{

	}
	void MainMenu::render(Game* game) 		
	{
		backgroundFrames[currentFrame].render(backgroundPos);

		title.render(85.0f, 100.0f);
		for (Button& it : buttons)
		{
			it.render(game);
		}
	}
	void MainMenu::exit(Game* game) 		
	{
		for (Texture& it : backgroundFrames)
			it.destroy();

		for (Button& it : buttons)
			it.destroy();

		title.destroy();

		game->previousScene = this;
	}
	void MainMenu::load(Game* game)
	{

	}

	void OptionsMenu::enter(Game* game)
	{
		accumulatedTime = 0;
		currentFrame = 0;
		adder = 1;
		docPos = { { 0, 0 }, { 1920, 1080 } };

		for (int i = 0; i < 9; i++)
		{
			std::string filepath = "assets/BackGrounds/Submenu_animated" + std::to_string(i + 1) + ".png";
			document[i].load(filepath.c_str());
		}

		Slider& musicSlider = sliders[static_cast<int>(OptionsMenuSliderLabels::music)];
		Slider& soundSlider = sliders[static_cast<int>(OptionsMenuSliderLabels::sound)];

		float starty = 850.0f;
		float padding = 100.0f;
		
		musicSlider.nob.load("assets/Buttons/Slider.png");
		musicSlider.rail.load("assets/Buttons/SliderRail.png");
		musicSlider.railPos.set({ starty, 400.0f }, { musicSlider.rail.getw(), musicSlider.rail.geth() });
		musicSlider.nobPos.setSize({ musicSlider.nob.getw(), musicSlider.nob.geth() });
		float startxMusic = Sound::getTagGain("music") * musicSlider.railPos.getw();
		musicSlider.nobPos.setPos({ musicSlider.railPos.getx() - musicSlider.nobPos.getw() / 2 + startxMusic,
									musicSlider.railPos.gety() - (musicSlider.nobPos.geth() - musicSlider.railPos.geth()) / 2 });

		soundSlider.nob.load("assets/Buttons/Slider.png");
		soundSlider.rail.load("assets/Buttons/SliderRail.png");
		soundSlider.railPos.set({ starty, musicSlider.railPos.gety() + padding }, {soundSlider.rail.getw(), soundSlider.rail.geth()});
		soundSlider.nobPos.setSize({ soundSlider.nob.getw(), soundSlider.nob.geth() });
		float startxSound = Sound::getTagGain("sound") * soundSlider.railPos.getw();
		soundSlider.nobPos.setPos({ soundSlider.railPos.getx() - soundSlider.nobPos.getw() / 2 + startxSound,
									soundSlider.railPos.gety() - (soundSlider.nobPos.geth() - soundSlider.railPos.geth()) / 2 });

		Color color = { 46, 48, 29 };

		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::unhovered)]
			.load("assets/Buttons/Button.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::hovered)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::clicked)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].label.load("assets/Fonts/BlackStones-ovoXB.ttf", 45, "fullscreen");
		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].label.setc(color);

		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].texture[static_cast<int>(ButtonState::unhovered)]
			.load("assets/Buttons/Button.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].texture[static_cast<int>(ButtonState::hovered)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].texture[static_cast<int>(ButtonState::clicked)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].label.load("assets/Fonts/BlackStones-ovoXB.ttf", 45, "windowed");
		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].label.setc(color);

		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].texture[static_cast<int>(ButtonState::unhovered)]
			.load("assets/Buttons/Button.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].texture[static_cast<int>(ButtonState::hovered)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].texture[static_cast<int>(ButtonState::clicked)]
			.load("assets/Buttons/HoveredOverButton5.png");
		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].label.load("assets/Fonts/BlackStones-ovoXB.ttf", 45.0f* 0.7f, "back");
		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].label.setc(color);

		const float w = buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::hovered)].getw(); 
		const float h = buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::unhovered)].geth();
		const float hoveredh = h - 10.0f;// buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].texture[static_cast<int>(ButtonState::hovered)].geth();
		float y = 560.0f;
		float x = 670.0f;
		padding = 150.0f + w;

		buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].pos.set({x, y}, { w, h });
		buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].pos.set({ x + padding, y }, { w, h });
		buttons[static_cast<int>(OptionsMenuButtonLabels::back)].pos.set({ x + padding + 250.0f, y + 150.0f}, { w * 0.7f, h * 0.7f });

		title.load("assets/Fonts/BlackStones-ovoXB.ttf", 35, "Options");
		title.setc({ 46, 48, 29 });
		slidersText[static_cast<int>(OptionsMenuSliderLabels::music)].load("assets/Fonts/BlackStones-ovoXB.ttf", 45, "Music: ");
		slidersText[static_cast<int>(OptionsMenuSliderLabels::music)].setc({ 46, 48, 29 });
		slidersText[static_cast<int>(OptionsMenuSliderLabels::sound)].load("assets/Fonts/BlackStones-ovoXB.ttf", 45, "Sound:");
		slidersText[static_cast<int>(OptionsMenuSliderLabels::sound)].setc({ 46, 48, 29 });
	}
	void OptionsMenu::render(Game* game)
	{
		document[currentFrame].render(docPos);
		if (currentFrame < 8)
			return;

		sliders[static_cast<int>(OptionsMenuSliderLabels::music)].render(game);
		sliders[static_cast<int>(OptionsMenuSliderLabels::sound)].render(game);
		for (Button& it : buttons)
			it.render(game);

		title.render(450.0f, 270.0f);
		slidersText[static_cast<int>(OptionsMenuSliderLabels::music)].render(620.0f, 380.0f);
		slidersText[static_cast<int>(OptionsMenuSliderLabels::sound)].render(620.0f, 480.0f);
	}
	void OptionsMenu::exit(Game* game)
	{
		for (Button& it : buttons)
			it.destroy();
		for (Texture& it : document)
			it.destroy();
		title.destroy();
		for (Text& it : slidersText)
			it.destroy();
	}
	void OptionsMenu::update(Game* game)
	{
		sliders[static_cast<int>(OptionsMenuSliderLabels::music)].update(game);
		sliders[static_cast<int>(OptionsMenuSliderLabels::sound)].update(game);
		Sound::setTagGain("music", sliders[static_cast<int>(OptionsMenuSliderLabels::music)].getValue());
		Sound::setTagGain("sound", sliders[static_cast<int>(OptionsMenuSliderLabels::sound)].getValue());

		if (game->game.keyDown(Keys::space))
		{
			game->mainMenu.render(game);
		}

		for (Button& it : buttons)
		{
			it.update(game);
		}

		if (buttons[static_cast<int>(OptionsMenuButtonLabels::back)].isClicked() && adder > 0)
		{	
			adder = -1;
			currentFrame--;
			game->mainMenu.enter(game);
		}

		if(buttons[static_cast<int>(OptionsMenuButtonLabels::fullscreen)].isClicked())
		{
			game->game.setFullscreen();
		}
		else if (buttons[static_cast<int>(OptionsMenuButtonLabels::windowed)].isClicked())
		{
			game->game.setWindowed();
		}


		if (currentFrame >= 8)
			return;

		accumulatedTime += game->game.getdt();
		float frameTime = 1.0f / 20.0f;
		while (accumulatedTime >= frameTime)
		{
			accumulatedTime -= frameTime;
			currentFrame += adder;
			if (currentFrame == 0 && adder < 0)
			{
				exit(game);
				game->currentScene = &game->mainMenu;
				return;
			}
		}
	}
	void OptionsMenu::fixedUpdate(Game* game) 
	{

	}

	void GamePlay::enter(Game* game)
	{
		player.currentSprite = PlayerSprite::brown;
		player.init(game);
	}
	void GamePlay::render(Game * game)
	{
		player.render(game);
	}
	void GamePlay::exit(Game* game)
	{
		player.destroy(game);
	}
	void GamePlay::update(Game* game)
	{
		player.update(game);
	}
	void GamePlay::fixedUpdate(Game* game)
	{

	}

	Game::Game()
		: game("Lethal Prescription: Remake", 1920, 1080, 0)
	
	{
		SDL_SetWindowIcon(game.getWindow()->window, Surface("assets/BackGrounds/icon.bmp").get());
		game.setTicks(50);
		menuMusic.load("assets/Sounds and music/BackGroundMusic.mpga", "music");
		menuMusic.tag("ui");
		menuMusic.playRepeat();
		Sound::setTagGain("music", 0);

		buttonSound.load("assets/Sounds and music/SoundEffectHover.wav", "sound");
		buttonSound.tag("ui");
#if _DEBUG
		DirectoryListener listener(SDL_GetCurrentDirectory());
#endif

		currentScene->enter(this);
;		while (!game.closeRequested())
		{
			game.updateEvents();
			while (game.ticking())
			{
				currentScene->fixedUpdate(this);
			}
#if _DEBUG
			if (listener.changed())
			{
				std::cout << "Reloading Current Scene'\n";
				currentScene->enter(this);
			}
#endif
			currentScene->update(this);

			game.clear();
			currentScene->render(this);
			game.present();
			SDL_Delay(1);
		}
	}

	Game::~Game()
	{

	}
}