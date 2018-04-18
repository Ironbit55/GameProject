#include "SdlMixer.h"
#include <iostream>


SdlMixer::SdlMixer(){
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		initSuccess = false;
	}
	initSuccess = true;
}


SdlMixer::~SdlMixer(){
	clearAllSounds();
	close();
}

bool SdlMixer::loadEffect(int id, std::string path){
	clearEffect(id); //clear if effect at id already exists

	//i have no idea where sdl actually stores the effects
	Mix_Chunk* effect = Mix_LoadWAV(path.c_str());
	if (effect == nullptr) {
		//failed to load effect, path is probably not valid
		std::cout << "Unable to load sound effect " << path << ": " << Mix_GetError() << std::endl;
		return false;
	}

	//put effect in the map
	soundEffects[id] = effect;
	return true;
}

bool SdlMixer::loadMusic(int id, std::string path){
	clearMusic(id);

	Mix_Music* song = Mix_LoadMUS(path.c_str());
	if (song == nullptr) {
		std::cout << "Unable to load music " << path << ": " << Mix_GetError() << std::endl;
		return false;
	}
	music[id] = song;
	return true;
}

bool SdlMixer::playEffect(int effectId){
	if(!effectExists(effectId)){
		return false;
	}

	Mix_PlayChannel(-1, soundEffects[effectId], 0);
	return true;
}

bool SdlMixer::playMusic(int musicId){

	if (!musicExists(musicId)) {
		return false;
	}

	Mix_PlayMusic(music[musicId], -1);
	return  true;
}

void SdlMixer::pauseMusic(){
	if (playingMusic()) {
		Mix_PausedMusic();
	}
}

void SdlMixer::unpauseMusic(){
	if (pausedMusic()) {
		Mix_ResumeMusic();
	}
}

void SdlMixer::stopMusic(){
	if (playingMusic() || pausedMusic()) {
		Mix_HaltMusic();
	}
}

bool SdlMixer::playingMusic(){
	return Mix_PlayingMusic();
}

bool SdlMixer::pausedMusic(){
	return Mix_PausedMusic();
}

void SdlMixer::close(){
	Mix_Quit();
}

void SdlMixer::clearAllSounds(){
	for (auto it = soundEffects.begin(); it != soundEffects.end(); it++) {
		Mix_FreeChunk(it->second);
	}

	soundEffects.clear();
	for (auto it = music.begin(); it != music.end(); it++) {
		Mix_FreeMusic(it->second);
	}
	music.clear();
}

void SdlMixer::clearEffect(int effectId){
	if (effectExists(effectId)) {
		Mix_FreeChunk(soundEffects[effectId]);
	}
	
}

void SdlMixer::clearMusic(int musicId){
	if (musicExists(musicId)) {
		Mix_FreeMusic(music[musicId]);
	}
}

bool SdlMixer::effectExists(int effectId){
	return soundEffects.find(effectId) != soundEffects.end();
}

bool SdlMixer::musicExists(int musicId){
	return music.find(musicId) != music.end();
}
