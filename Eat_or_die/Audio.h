#pragma once
#include <SFML/Audio.hpp>

using namespace sf;


class Audio
{


public:
	Audio(bool load_everything = false, bool menu = false);
	~Audio();

	enum SoundType
	{
		CRUNCH, CLICK, DRINK, GAME_OVER, LEVEL_COMPLETE, T_COUNT
	};


	void loadMusic(bool);
	bool loadSounds();

	void playMusic();
	void stopMusic();
	void playSound(SoundType, bool);
	void setVolume(bool);

private:

	Music music;
	Sound sound;
	SoundBuffer bufer[T_COUNT];
};

