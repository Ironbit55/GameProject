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
#include <iostream>
#include "MessageReceiver.h"


enum SoundEffect {
	EFFECT_JUMP,
	EFFECT_BOUNCE,
	EFFECT_MAX,
};

enum Music {
	MUSIC_LEVEL1,
	MUSIC_MAX,
};

enum MusicCommand{
	COMMAND_PLAY,
	COMMAND_PAUSE,
	COMMAND_STOP,
};

struct SoundEffectMsgData{
	SoundEffect effect;
};

struct MusicMsgData {
	Music music;
	MusicCommand command;
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
class AudioManager : MessageReceiver
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

	//I could have inherited from SdlMixer and i wouldn't have to wrap these but
	//inhertence felt wrong in this case


	//helper functions to control playing music

	void pauseMusic();

	void unpauseMusic();

	void stopMusic();

	bool playingMusic();

	bool pausedMusic();

protected: 

	void receiveEffectMessage(Message& msg) { 
		//this better be a MESSAGE_AUDIO_EFFECT
		SoundEffectMsgData* data = static_cast<SoundEffectMsgData*>(msg.dataPayload);
		this->playSoundEffect(data->effect);
	};

	void receiveMusicMessage(Message& msg) {
		//this better be a MESSAGE_AUDIO_MUSIC
		MusicMsgData* data = static_cast<MusicMsgData*>(msg.dataPayload);
		switch (data->command)
		{
		case COMMAND_PLAY:
			this->playMusic(data->music);
			break;
		case COMMAND_PAUSE:
			this->pauseMusic();
			break;
		case COMMAND_STOP:
			this->stopMusic();
			break;
		}
		
	};

	
	void onReceiveMessage(Message& msg) {
		switch (msg.messageType) 
		{
			case MESSAGE_AUDIO_EFFECT: 
				receiveEffectMessage(msg);
				break;
			case MESSAGE_AUDIO_MUSIC:
				receiveMusicMessage(msg);
				break;
			default: 
				break;
		}
	};



private:
	std::map<SoundEffect, std::string> effectFileMap;
	std::map<Music, std::string> musicFileMap;

	SdlMixer mixer;
};

