#pragma once

#include <SDL3_mixer/SDL_mixer.h>

namespace slib
{

	class Sound
	{
	public:
		Sound();
		Sound(const char* filename);
		Sound(const char* filename, const char* tag);
		~Sound();

		void play();
		void play(int loop);
		void playRepeat();

		void pause();
		void resume();

		bool load(const char* filename);
		bool load(const char* filename, const char* tag);

	private:
		bool create(const char* filename);
		bool create(const char* filename, const char *tag);

		MIX_Audio *audio = nullptr;
		MIX_Track* track = nullptr;
	};
}