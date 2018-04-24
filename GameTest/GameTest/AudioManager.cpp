#include "AudioManager.h"

//this is the lazy version in absense of file io for now
//these get read on startup and loaded in to a dictionary
//which the engine uses during runtime
LookupEffectFile effectLookup[] =
{
	{EFFECT_JUMP, "Jump.wav"},
	{EFFECT_BOUNCE, "placeholder_bounce.wav" },
	{EFFECT_MAX, ""},
};

LookupMusicFile musicLookup[] =
{
	{MUSIC_LEVEL1,		"sjg.mp3" },
	{MUSIC_MAX, ""},
};

AudioManager::AudioManager()
{
}


AudioManager::~AudioManager() {
	deleteAllListeners();
}

void AudioManager::init(){

	
	//typedef void(AudioManager::*audioMsgCallbackType)(Message&);
	//audioMsgCallbackType audioMsgCallbackDummy = &AudioManager::dummy;
	MessageCallback test = std::bind(&AudioManager::receiveEffectMessage, this, std::placeholders::_1);
	MessageCallback testDummy = std::bind(&AudioManager::dummy, this, std::placeholders::_1);

	addListener(MESSAGE_AUDIO_EFFECT);
	addListener(MESSAGE_AUDIO_MUSIC, testDummy);

	Message msg;
	msg.messageType = MESSAGE_AUDIO_EFFECT;
	msg.timeUntillDispatch = 0;
	SoundEffectMsgData msgData;
	msgData.effect = EFFECT_BOUNCE;
	msg.dataPayload = &msgData;
	msg.dataSize = sizeof(msgData);
	

	pushMessage(msg);
	
	////MessagingService::instance().registerListener(MESSAGE_AUDIO_EFFECT, test);
	//Message msg;
	//test(msg);
	//testDummy(msg);

	//std::cout << test.target<void(AudioManager::*)(Message&)>();
	//std::cout << testDummy.target<void(AudioManager::*)(Message&)>();
	//std::cout << MessagingService::instance().getAddress(test);
	//std::cout << MessagingService::instance().getAddress(test);

	//loop through lookup tables and add to map
	//do we actually need the maps? probably not if we always load everything in one go
	//if we wanted to load things in on the fly its usefull to be able to look it up
	//using a map
	for (int i = 0; i < EFFECT_MAX; ++i){
		if(effectLookup[i].effectId == EFFECT_MAX){
			//hit end of lookup
			break;
		}
		effectFileMap.insert(std::make_pair(effectLookup[i].effectId, effectLookup[i].file));
	}

	for (int i = 0; i < EFFECT_MAX; ++i) {
		if (musicLookup[i].musicId == MUSIC_MAX) {
			//hit end of lookup
			break;
		}
		musicFileMap.insert(std::make_pair(musicLookup[i].musicId, musicLookup[i].file));
	}

	loadAllSounds();
}

void AudioManager::loadAllSounds(){
	for (std::map<SoundEffect, std::string>::iterator it = effectFileMap.begin(); it != effectFileMap.end(); ++it){
		mixer.loadEffect(it->first, SOUND_EFFECT_DIR + it->second);
	}

	for (std::map<Music, std::string>::iterator it = musicFileMap.begin(); it != musicFileMap.end(); ++it) {
		mixer.loadMusic(it->first, MUSIC_DIR + it->second);
	}
}

bool AudioManager::playSoundEffect(SoundEffect effect){
	return mixer.playEffect(effect);
}

bool AudioManager::playMusic(Music music){
	return mixer.playMusic(music);
}

void AudioManager::pauseMusic(){
	mixer.pauseMusic();
}

void AudioManager::unpauseMusic(){
	mixer.unpauseMusic();
}

void AudioManager::stopMusic(){
	mixer.stopMusic();
}

bool AudioManager::playingMusic(){
	return mixer.playingMusic();
}

bool AudioManager::pausedMusic(){
	return mixer.pausedMusic();
}

