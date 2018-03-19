/*
* Class: AudioManager
* Author:Ed Curran (b5021040)
* Description: The audio subsystem. Uses SdlMixer to play music and sound effects
*
* 
*/

#pragma once
#include <string>
#include <map>
#include "../sdlgl/SdlMixer.h"

enum SoundEffect {
	EFFECT_JUMP,
	EFFECT_MAX,
};

enum Music {
	MUSIC_LEVEL1,
	MUSIC_MAX,
};

typedef struct LookupEffectFile {
	SoundEffect effectId;
	std::string file;
} LookupEffectFile;

typedef struct LookupMusicFile {
	Music musicId;
	std::string file;
} LookupMusicFile;


const std::string MUSIC_DIR = "../../Audio/Music/";
const std::string SOUND_EFFECT_DIR = "../../Audio/Effects/";
class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	//ideally populate the contents of the maps by reading in
	//a file
	void init();

	void init(std::string path);

	void loadAllSounds();

	bool playSoundEffect(SoundEffect effect);
	bool playMusic(Music music);

	//I could have inherited and i wouldn't have to wrap these but
	//inhertence felt wrong in this case
	//helper functions to control playing music

	void pauseMusic();

	void unpauseMusic();

	void stopMusic();

	bool playingMusic();

	bool pausedMusic();

private:
	std::map<SoundEffect, std::string> effectFileMap;
	std::map<Music, std::string> musicFileMap;

	SdlMixer mixer;
};

