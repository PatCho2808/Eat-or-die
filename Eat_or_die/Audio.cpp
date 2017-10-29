#include "Audio.h"



Audio::Audio(bool load_everything, bool menu)
{
	if (load_everything)
	{
		loadMusic(menu);
		loadSounds();
	}
}

void Audio::loadMusic(bool menu)
{
	std::string path;
	if (menu)
		path = "../data/sounds/menu.ogg";
	else
		path = "../data/sounds/GamePlay.ogg";

	music.openFromFile(path);
	music.setLoop(true);
}

bool Audio::loadSounds()
{
	std::string name[T_COUNT] = { "crunch.ogg","click.wav","drink.ogg","game_over.wav","level_complete.ogg" };

	for (int i = 0; i < T_COUNT; i++)
	{
		if (!bufer[i].loadFromFile("../data/sounds/" + name[i]))
		{
			return false;
		}
	}

	return true;
}

void Audio::playMusic()
{
	music.play();
}

void Audio::stopMusic()
{
	music.stop();
}

void Audio::playSound(SoundType st, bool loop)
{
	sound.setBuffer(bufer[st]);
	sound.setLoop(loop);
	sound.play();
}


void Audio::setVolume(bool mute)
{
	if (mute)
	{
		music.stop();
		sound.setVolume(0);
	}

	else
	{
		music.play();
		sound.setVolume(100);
	}
}

Audio::~Audio()
{

}
