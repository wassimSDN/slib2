#include <iostream>
#include <string>

#include <slib.h>
#include <SDL3_ttf/SDL_ttf.h>

int main()
{
	slib::App slib2test = { "test window", 1000, 1000, SDL_WINDOW_RESIZABLE };
	slib2test.setTicks(50);



	slib::AnimatedTexture sprite("sprite.png", 4, 4, 2, 175, 102, 145, 2);
	
	slib::Rect c = { {250, 250}, {500, 500} };

	while (!slib2test.closeRequested())
	{
		slib2test.updateEvents();
		while (slib2test.ticking())
		{

		}

		if (slib2test.mouseJustDown(slib::Buttons::left))
		{
			sprite.playOnce();
		}
		else if (slib2test.mouseJustDown(slib::Buttons::right))
		{
			sprite.playRepeat();
		}

	
		
		sprite.update();

		slib2test.clear();


		sprite.render(c);

		slib2test.present();
		//SDL_Delay(16);
	}
}