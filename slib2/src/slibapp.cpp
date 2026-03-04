#if _DEBUG
#include <iostream>
#endif

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "../include/slibapp.h"
#include "../include/slibmisc.h"

namespace slib
{

	bool State::nowFalse() const { return !current; }
	bool State::nowJustFalse() const { return !current && previous; }
	bool State::nowTrue() const { return current; }
	bool State::nowJustTrue() const { return current && !previous; }

	bool Window::createWindow(const char* title, int width, int height, int flags)
	{
		if (!SDL_CreateWindowAndRenderer(title, width, height, flags, &window, &renderer))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}
#if _DEBUG		
		if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND))
		{

			std::cout << SDL_GetError() << '\n';
		}

		textEngine = TTF_CreateRendererTextEngine(renderer);
		if (!textEngine)
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		textEngine = TTF_CreateRendererTextEngine(renderer);
#endif
		
		ID = SDL_GetWindowID(window);
		
		return true;
	}

	Dimensions Window::size() 
	{
		int w = 0, h = 0;

#if _DEBUG
		if (!SDL_GetWindowSize(window, &w, &h))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		SDL_GetWindowSize(window, &w, &h);
#endif	

		return { static_cast<float>(w), static_cast<float>(h) };
	}

	void Window::destroyWindow()
	{
		if (window)
			SDL_DestroyWindow(window);

		if (renderer)
			SDL_DestroyRenderer(renderer);
	}

	void Window::clear() 
	{ 
#if _DEBUG
		if (!SDL_RenderClear(renderer))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		SDL_RenderClear(renderer); 
#endif	
	}

	void Window::present() { SDL_RenderPresent(renderer); }

	Color Window::getRenderColor()
	{
		Color color;
		SDL_GetRenderDrawColor(renderer, &color.r, &color.g, &color.b, &color.a);

		return color;
	}

	void Window::setRenderColor(Color color)
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	}

	bool Mixer::init()
	{
		if (!MIX_Init())
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
		if (!mixer)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif

			return false;
		}

		return true;
	}

	void Mixer::destroyMixer()
	{
		if(mixer)
			MIX_DestroyMixer(mixer);
	}

	void TimeManager::update()
	{
		const long double currentTime = static_cast<long double>(SDL_GetPerformanceCounter()) / counterFrequency;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		accumulatedTime += deltaTime;
	}

	bool TimeManager::ticking()
	{
		if (accumulatedTime < timeStep)
			return false;

		accumulatedTime -= timeStep;

		return true;
	}

	void TimeManager::setTicks(int ticks)
	{
		if (ticks > 0)
		{
			timeStep = 1.0f / ticks;
			return;
		}

		timeStep = 1.0f / 20;
	}

	App::App(const char* title, int width, int height, int flags) { init(title, width, height, flags); }

	App::~App()
	{
		mainWindow.destroyWindow();
	
		for (Window& it : secondaryWindows)
		{
			it.destroyWindow();
		}

		SDL_Quit();
		TTF_Quit();
		mixer.destroyMixer();
	}

	void App::init(const char* title, int width, int height, int flags)
	{
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
		{
			mainWindow.closeRequested = true;
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		if (!mainWindow.createWindow(title, width, height, flags))
		{
			mainWindow.closeRequested = true;
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		if (!TTF_Init())
		{
			mainWindow.closeRequested = true;
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		if (!mixer.init())
		{
			mainWindow.closeRequested = true;
			return;
		}

		time.counterFrequency = static_cast<long double>(SDL_GetPerformanceFrequency());
		time.timeStep = 1.0f / 20;
		time.accumulatedTime = 0;
		time.lastTime = static_cast<long double>(SDL_GetPerformanceCounter()) / time.counterFrequency;
	}

	void App::updateEvents()
	{
		mainWindow.hasFocus.previous = mainWindow.hasFocus.current;
		mainWindow.hasMouse.previous = mainWindow.hasMouse.current;
		mainWindow.mouse.second = false;
		mainWindow.wheelu.previous = mainWindow.wheelu.current;
		mainWindow.wheeld.previous = mainWindow.wheeld.current;
		mainWindow.wheelr.previous = mainWindow.wheelr.current;
		mainWindow.wheell.previous = mainWindow.wheell.current;
		mainWindow.wheelu.current = false;
		mainWindow.wheeld.current = false;
		mainWindow.wheelr.current = false;
		mainWindow.wheell.current = false;
		for (State& it : mainWindow.keyStates)
		{
			it.previous = it.current;
		}
		for (State& it : mainWindow.buttonStates)
		{
			it.previous = it.current;
		}

		for (Window& it : secondaryWindows)
		{
			it.hasFocus.previous = it.hasFocus.current;
			it.hasMouse.previous = it.hasMouse.current;
			it.mouse.second = false;
			it.wheelu.previous = it.wheelu.current;
			it.wheeld.previous = it.wheeld.current;
			it.wheell.previous = it.wheell.current;
			it.wheelr.previous = it.wheelr.current;
			it.wheelu.current = false;
			it.wheeld.current = false;
			it.wheell.current = false;
			it.wheelr.current = false;
			for (State& it2 : it.keyStates)
			{
				it2.previous = it2.current;
			}
			for (State& it2 : mainWindow.buttonStates)
			{
				it2.previous = it2.current;
			}
		}

		time.update();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_EVENT_MOUSE_MOTION:
				if (e.motion.windowID == mainWindow.ID)
				{
					mainWindow.mouse = { {e.motion.x, e.motion.y}, true };
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.motion.windowID)
						{
							it.mouse = { {e.motion.x, e.motion.y }, true };
							break;
						}
					}
				}
				break;

			case SDL_EVENT_KEY_DOWN:
				if (e.key.windowID == mainWindow.ID)
				{
					mainWindow.keyStates[e.key.scancode].current = true;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.key.windowID)
						{
							it.keyStates[e.key.scancode].current = true;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_KEY_UP:
				if (e.key.windowID == mainWindow.ID)
				{
					mainWindow.keyStates[e.key.scancode].current = false;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.key.windowID)
						{
							it.keyStates[e.key.scancode].current = false;
							break;
						}
					}
				}
				break;
			  
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				if (e.button.windowID == mainWindow.ID)
				{
					mainWindow.buttonStates[e.button.button].current = true;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.button.windowID)
						{
							it.buttonStates[e.button.button].current = true;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_MOUSE_BUTTON_UP:
				if (e.button.windowID == mainWindow.ID)
				{
					mainWindow.buttonStates[e.button.button].current = false;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.button.windowID)
						{
							it.buttonStates[e.button.button].current = false;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_MOUSE_WHEEL:
				if (e.button.windowID == mainWindow.ID)
				{
					mainWindow.wheelu.current = e.wheel.y > 0;
					mainWindow.wheeld.current = e.wheel.y < 0;
					mainWindow.wheelr.current = e.wheel.x > 0;
					mainWindow.wheell.current = e.wheel.x < 0;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.button.windowID)
						{
							it.wheelu.current = e.wheel.y > 0;
							it.wheeld.current = e.wheel.y < 0;
							it.wheelr.current = e.wheel.x > 0;
							it.wheell.current = e.wheel.x < 0;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_WINDOW_FOCUS_GAINED:
				if (e.window.windowID == mainWindow.ID)
				{
					mainWindow.hasFocus.current = true;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.window.windowID)
						{
							it.hasFocus.current = true;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_WINDOW_FOCUS_LOST:
				if (e.window.windowID == mainWindow.ID)
				{
					mainWindow.hasFocus.current = false;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.window.windowID)
						{
							it.hasFocus.current = false;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_WINDOW_MOUSE_ENTER:
				if (e.window.windowID == mainWindow.ID)
				{
					mainWindow.hasMouse.current = true;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.window.windowID)
						{
							it.hasMouse.current = true;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_WINDOW_MOUSE_LEAVE:
				if (e.window.windowID == mainWindow.ID)
				{
					mainWindow.hasMouse.current = false;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.window.windowID)
						{
							it.hasMouse.current = false;
							break;
						}
					}
				}
				break;

			case SDL_EVENT_QUIT:
				mainWindow.closeRequested = true;
				break;
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				if (e.window.windowID == mainWindow.ID)
				{
					mainWindow.closeRequested = true;
				}
				else
				{
					for (Window& it : secondaryWindows)
					{
						if (it.ID == e.window.windowID)
						{
							it.closeRequested = true;
							break;
						}
					}
				}
				break;
			}	
		}	
	}

	bool App::running() const { return !mainWindow.closeRequested; }

	bool App::addWindow(const char* title, int width, int height, int flags)
	{
		Window window;
		if (!window.createWindow(title, width, height, flags))
			return false;

		secondaryWindows.push_back(window);

		return true;
	}

	void App::popWindow()
	{
		secondaryWindows[secondaryWindows.size() - 1].destroyWindow();
		secondaryWindows.pop_back();
	}

	Color App::getRenderColor() const { return mainWindow.getRenderColor(); }
	void App::setRenderColor(Color color)
	{
		mainWindow.setRenderColor(color);
	}

	Color App::getRenderColor(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return { 0, 0, 0, 0 };

		return secondaryWindows[windowIndex].getRenderColor();
	}
	void App::setRenderColor(int windowIndex, Color color)
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return;
		
		secondaryWindows[windowIndex].setRenderColor(color);
	}

	float App::mousex() { return mainWindow.mouse.first.x; }
	float App::mousey() { return mainWindow.mouse.first.y; }

	float App::mousex(int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return -1;
		
		return secondaryWindows[windowIndex].mouse.first.x;
	}
	float App::mousey(int windowIndex)
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return -1;

		return secondaryWindows[windowIndex].mouse.first.y;
	}

	void App::clear()
	{
		mainWindow.clear();

		for (Window& it : secondaryWindows)
		{
			it.clear();
		}
	}

	void App::present()
	{
		mainWindow.present();

		for(Window& it : secondaryWindows)
		{
			it.present();
		}
	}

	Window App::getWindow() const { return mainWindow; }

	bool App::hasFocus() const { return mainWindow.hasFocus.nowTrue(); }
	bool App::justHasFocus() const { return mainWindow.hasFocus.nowJustTrue(); }

	bool App::hasFocus(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].hasFocus.nowTrue();;
	}
	bool App::justHasFocus(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].hasFocus.nowJustTrue();;
	}

	bool App::closeRequested() const { return mainWindow.closeRequested; }
	
	bool App::closeRequested(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].closeRequested;
	}

	bool App::oneCloseRequested() const
	{
		if (mainWindow.closeRequested)
			return true;

		for (Window& it : secondaryWindows)
		{
			if (it.closeRequested)
				return true;
		}

		return false;
	}

	bool App::keyDown(Keys key) const { return mainWindow.keyStates[(int)key].nowTrue(); }
	bool App::keyJustDown(Keys key) const { return	mainWindow.keyStates[(int)key].nowJustTrue(); }
	bool App::keyUp(Keys key) const { return mainWindow.keyStates[(int)key].nowFalse(); }
	bool App::keyJustUp(Keys key) const { return mainWindow.keyStates[(int)key].nowJustFalse(); }

	bool App::keyDown(Keys key, int windowIndex) const 
	{ 
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].keyStates[(int)key].nowTrue(); 
	}
	bool App::keyJustDown(Keys key, int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return	secondaryWindows[windowIndex].keyStates[(int)key].nowJustTrue();
	}
	bool App::keyUp(Keys key, int windowIndex) const 
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false; 
	
		return secondaryWindows[windowIndex].keyStates[(int)key].nowFalse();
	}
	bool App::keyJustUp(Keys key, int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].keyStates[(int)key].nowJustFalse();
	}

	bool App::mouseDown(Buttons button) const { return	mainWindow.buttonStates[(int)button].nowTrue(); }
	bool App::mouseJustDown(Buttons button) const { return	mainWindow.buttonStates[(int)button].nowJustTrue(); }
	bool App::mouseUp(Buttons button) const { return mainWindow.buttonStates[(int)button].current; }
	bool App::mouseJustUp(Buttons button) const { return	mainWindow.buttonStates[(int)button].nowJustFalse(); }

	bool App::mouseDown(Buttons button, int windowIndex) const 
	{ 
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].buttonStates[(int)button].nowTrue();
	}
	bool App::mouseJustDown(Buttons button, int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size() || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].buttonStates[(int)button].nowJustTrue();
	}
	bool App::mouseUp(Buttons button, int windowIndex) const 
	{ 
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].buttonStates[(int)button].nowFalse();
	}
	bool App::mouseJustUp(Buttons button, int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return 	secondaryWindows[windowIndex].buttonStates[(int)button].nowJustFalse();
	}

	bool App::scrollu() const
	{
		return mainWindow.wheelu.nowTrue();
	}
	bool App::scrolld() const
	{
		return mainWindow.wheeld.nowTrue();
	}
	bool App::scrolll() const
	{
		return mainWindow.wheell.nowTrue();
	}
	bool App::scrollr() const
	{
		return mainWindow.wheelr.nowTrue();
	}

	bool App::scrollJustu() const
	{
		return mainWindow.wheelu.nowJustTrue();
	}
	bool App::scrollJustd() const
	{
		return mainWindow.wheeld.nowJustTrue();
	}
	bool App::scrollJustl() const
	{
		return mainWindow.wheell.nowJustTrue();
	}
	bool App::scrollJustr() const
	{
		return mainWindow.wheelr.nowJustTrue();
	}


		 
	bool App::scrollu(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;
		
		return secondaryWindows[windowIndex].wheelu.nowTrue();
	}
	bool App::scrolld(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].wheeld.nowTrue();
	}
	bool App::scrolll(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;
		
		return secondaryWindows[windowIndex].wheell.nowTrue();
	}
	bool App::scrollr(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;
	
		return secondaryWindows[windowIndex].wheelr.nowTrue();
	}

	bool App::scrollJustu(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].wheelu.nowJustTrue();
	}
	bool App::scrollJustd(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].wheeld.nowJustTrue();
	}
	bool App::scrollJustl(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].wheell.nowJustTrue();
	}
	bool App::scrollJustr(int windowIndex) const
	{
		if (windowIndex < 0 || windowIndex >= secondaryWindows.size())
			return false;

		return secondaryWindows[windowIndex].wheelr.nowJustTrue();
	}


	void App::setTicks(int ticks)
	{
		time.setTicks(ticks);
	}

	bool App::ticking()
	{
		return time.ticking();
	}

	long double App::getdt()
	{
		return time.deltaTime;
	}


	Window App::mainWindow;
	std::vector<Window> App::secondaryWindows;
	TimeManager App::time;
	Mixer App::mixer;
}