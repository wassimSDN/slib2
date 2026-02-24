#include <iostream>
#include <string>

#include <slib.h>

int main()
{
	slib::App slib2test = { "test window", 1000, 1000, SDL_WINDOW_RESIZABLE };
	slib2test.setTicks(50);

	slib::Rect r = { {100, 100 }, { 200, 200 } };
	slib::Rect b = { {400, 400 }, { 200, 200 } };

	slib2test.addWindow("test window", 1000, 1000, SDL_WINDOW_RESIZABLE);;
	slib::Texture txt = { "test.png", 0};
	int a = 0;

	slib::Vector2 p = { 10, 10 };

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


		
		slib2test.clear();

		//r.setPos({ slib2test.mousex() - 100, slib2test.mousey() - 100 });
		b.renderFill({ 0, 0, 255, 255 });


		//r.renderFill({ 255, 0, 0, 255 });

		txt.render(r, r);
		r.intersection(b).renderFill({ 0, 255, 0, 128 });

		slib2test.present();
		SDL_Delay(16);
	}
}