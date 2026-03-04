#if _DEBUG
#include <iostream>
#endif

#include "../include/slibapp.h"
#include "../include/slibaudio.h"

namespace slib
{

	Sound::Sound()
	{
		audio = MIX_CreateSineWaveAudio(App::mixer.mixer, 440, 0.5f, 10);
		if (!audio)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		track = MIX_CreateTrack(App::mixer.mixer);

		if (!track)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}

		if (!MIX_SetTrackAudio(track, audio))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return;
		}
	}
	Sound::Sound(const char* filename)
	{
		create(filename);
	}
	Sound::Sound(const char* filename, const char* tag)
	{
		create(filename, tag);
	}

	Sound::~Sound()
	{
		if(audio)
			MIX_DestroyAudio(audio);

		if(track)
			MIX_DestroyTrack(track);
	}

	void Sound::play()
	{
#if _DEBUG
		if (!MIX_PlayTrack(track, 0))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		MIX_PlayTrack(track, 0);
#endif
	}
	void Sound::play(int loop)
	{
		SDL_PropertiesID options = SDL_CreateProperties();
		SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, loop);
		MIX_PlayTrack(track, options);
	}
	void Sound::playRepeat()
	{
		SDL_PropertiesID options = SDL_CreateProperties();
		SDL_SetNumberProperty(options, MIX_PROP_PLAY_LOOPS_NUMBER, -1);

#if _DEBUG
		if (!MIX_PlayTrack(track, options))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		MIX_PlayTrack(track, options);
#endif
	}
		
	void Sound::pause()
	{
#if _DEBUG
		if (!MIX_PauseTrack(track))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		MIX_PauseTrack(track);
#endif
	}
	
	void Sound::resume()
	{
#if _DEBUG
		if (!MIX_ResumeTrack(track))
		{
			std::cout << SDL_GetError() << '\n';
		}
#else
		MIX_ResumeTrack(track);
#endif
	}
	
	bool Sound::load(const char* filename)
	{
		return create(filename);
	}
	bool Sound::load(const char* filename, const char* tag)
	{
		return create(filename, tag);
	}

	bool Sound::create(const char* filename)
	{
		if (audio)
			MIX_DestroyAudio(audio);
		if (track)
			MIX_DestroyTrack(track);

		audio = MIX_LoadAudio(App::mixer.mixer, filename, true);
		if (!audio)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		track = MIX_CreateTrack(App::mixer.mixer);

		if (!track)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		if (!MIX_SetTrackAudio(track, audio))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		return true;
	}

	bool Sound::create(const char* filename, const char* tag)
	{
		if (audio)
			MIX_DestroyAudio(audio);
		if (track)
			MIX_DestroyTrack(track);

		audio = MIX_LoadAudio(App::mixer.mixer, filename, true);
		if (!audio)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		track = MIX_CreateTrack(App::mixer.mixer);

		if (!track)
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}


		if (!MIX_SetTrackAudio(track, audio))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}


		if (!MIX_TagTrack(track, tag))
		{
#if _DEBUG
			std::cout << SDL_GetError() << '\n';
#endif
			return false;
		}

		return true;
	}
}