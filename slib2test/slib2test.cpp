#include <iostream>
#include <string>

#include <slib.h>
#include <SDL3_ttf/SDL_ttf.h>

int main()
{
	slib::App slib2test = { "test window", 1000, 1000, SDL_WINDOW_RESIZABLE };
	slib2test.setTicks(50);

	slib::Rect r = { {100, 100 }, { 200, 200 } };
	slib::Rect b = { {400, 400 }, { 200, 200 } };

	

	slib::Texture txt = { "test.png"};
	int a = 0;

	slib::Vector2 p = { 10, 10 };

	float size = 200;
	slib::Text text("font.ttf", size, "hi hello");

	slib::Sound sound;
	//sound.load("lost.wav");

	slib::AnimatedTexture sprite("sprite.png", 4, 4, 2, 175, 102, 145, 2);
	
	slib::Rect c = { {250, 250}, {500, 500} };

	while (!slib2test.closeRequested())
	{
		slib2test.updateEvents();
		while (slib2test.ticking())
		{
			slib::Vector2 v;
			if (slib2test.keyDown(slib::Keys::a))
			{
				v += {-10, 0};
			}
			if (slib2test.keyDown(slib::Keys::d))
			{
				v += {10, 0};
			}
			if (slib2test.keyDown(slib::Keys::s))
			{
				v += {0, 10};
			}
			if (slib2test.keyDown(slib::Keys::w))
			{
				v += {0, -10};
			}

			r.addPos(v);
			a++;
		}

		sprite.update();

		if (slib2test.scrollu())
		{
			text.setc({ 255, 0, 0, 255 });
			size++;
			text.setfs(size);
		}
		else if (slib2test.scrolld())
		{
			text.setc({ 0, 255, 0, 255 });
			size--;
			text.setfs(size);
		}
	
		if (slib2test.mouseJustDown(slib::Buttons::left))
		{
			sound.play();
		}

		if (slib2test.mouseJustDown(slib::Buttons::right))
		{
			sound.pause();
		}
		
		slib2test.clear();

		//r.setPos({ slib2test.mousex() - 100, slib2test.mousey() - 100 });
		b.renderFill({ 0, 0, 255, 255 });


		//r.renderFill({ 255, 0, 0, 255 });

		txt.render(r, r);
		r.intersection(b).renderFill({ 0, 255, 0, 128 });

		text.render(slib2test.mousex() - text.getw() / 2, slib2test.mousey() - text.geth() / 2);

		sprite.render(c);

		slib2test.present();
		//SDL_Delay(16);
	}
}