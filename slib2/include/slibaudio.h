#pragma once

#include <SDL3_mixer/SDL_mixer.h>

namespace slib
{

	class Sound
	{
	public:
		Sound() = default;
		Sound(const char* filename);
		Sound(const char* filename, const char* tag);
		~Sound();

		static void setTagGain(const char* tag, float gain);
		static float getTagGain(const char* tag);

		void play();
		void play(int loop);
		void playRepeat();

		void pause();
		void resume();

		void tag(const char* tag);

		bool load(const char* filename);
		bool load(const char* filename, const char* tag);

	private:
		bool create(const char* filename);
		bool create(const char* filename, const char *tag);

		MIX_Audio *audio = nullptr;
		MIX_Track* track = nullptr;
	};
}