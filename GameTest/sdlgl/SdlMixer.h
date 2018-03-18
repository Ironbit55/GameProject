/*
* Class: SdlMixer
* Author:Ed Curran (b5021040)
* Description: OO wrapper around SDL2_Mixer library. This allows us to play audio:
* a music track and any number of sound effects.
* basically acts as our audio subsystem. Although possibly audio subsystem would be
* another layer that makes use of this.
*
* This is very much similar to: https://github.com/LiquidityC/flat/blob/master/src/Mixer.h
*/
#pragma once
#include <SDL_mixer.h>
#include <string>
#include <map>

class SdlMixer
{
public:
	SdlMixer();
	~SdlMixer();

	/*
	 * load in effect at given path
	 * will overite any effect at the given id
	 */
	bool loadEffect(int id, std::string path);

	/*
	* load in music at given path
	* will overite any music at the given id
	*/
	bool loadMusic(int id, std::string path);

	bool playEffect(int effectId);

	bool playMusic(int musicId);

	//helper functions to control playing music

	void pauseMusic();

	void unpauseMusic();

	void stopMusic();

	bool playingMusic();

	bool pausedMusic();


	/**
	 * close the underlying SDL2_Mixer
	 */
	void close();

	/**
	 * unload all music and sounds
	 */
	void clearAllSounds();
	void clearEffect(int effectId);
	void clearMusic(int musicId);
protected:
	bool effectExists(int effectId);
	bool musicExists(int musicId);
private:
	bool initSuccess;

	//should think about doing some memory management for these.
	//its not a huge deal because pretty sure we will just be loading these
	//all in on startup
	std::map<int, Mix_Chunk*> soundEffects;
	std::map<int, Mix_Music*> music;
};

