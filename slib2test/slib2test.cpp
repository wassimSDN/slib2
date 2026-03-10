#include <iostream>
#include <string>

#include <slib.h>
#include <SDL3_ttf/SDL_ttf.h>

int main()
{
	slib::App slib2test = { "test window", 1000, 1000, SDL_WINDOW_RESIZABLE };
	slib2test.setTicks(50);

	for(int i = 0; i < SDL_GetNumRenderDrivers(); i++)
		std::cout << SDL_GetRenderDriver(i) << '\n';
	
	slib::AnimatedTexture sprite("sprite.png", 4, 4, 2, 175, 102, 145, 2);
	
	slib::Rect c = { {250, 250}, {500, 500} };


	int hx = 0;
	int hy = 0;
	IMG_Animation *animation = IMG_LoadAnimation("cursor.gif");
	if (!animation)
		std::cout << SDL_GetError() << '\n';
	else
	{
		hx = animation->frames[0]->w / 2;
		hy = animation->frames[0]->h / 2;
	}
	SDL_Cursor *cursor = IMG_CreateAnimatedCursor(animation, hx, hy);

	if (!cursor)
		std::cout << SDL_GetError() << '\n';
	//if (!SDL_SetCursor(cursor))
		//std::cout << SDL_GetError() << '\n';
	
	
	IMG_FreeAnimation(animation);	
	slib2test.setRenderColor({ 128, 128, 128, 255 });

	while (!slib2test.closeRequested())
	{
		slib2test.updateEvents();
		while (slib2test.ticking())
		{

		}

		if (slib2test.mouseJustDown(slib::Buttons::left))
		{
			sprite.playOnce();
			sprite.reset();
		}
		else if (slib2test.mouseJustDown(slib::Buttons::right))
		{
			sprite.playRepeat();
		}

		
		slib::Vector2 v = { slib2test.mousex(), slib2test.mousey() };
		slib::Vector2 w = { v.x - c.getx() - c.getw() / 2.0f, v.y - c.gety() - c.geth() / 2.0f };
		
		c.addPos(w * slib2test.getdt());
		
		sprite.update();

		slib2test.clear();

		sprite.render(c);

		slib2test.present();
		SDL_Delay(16);
	}

	SDL_DestroyCursor(cursor);
}