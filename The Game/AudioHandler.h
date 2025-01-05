#pragma once
#include <portaudio.h>
#include <iostream>
#include "GameState.h"
#include "GameObject.h"
#include "Player.h"
#include <vector>
#include <mutex>


class AudioHandler : public GameObject {
private:
	bool initialized;
	PaStream* stream = nullptr;
	//float pi = std::acos(-1.0f);

	
	
public:
	static std::vector <float> globalAudioBuffer; //buffer gia dedomena hxou
	static std::mutex buffermutex; //mutex for safety 
	std::vector<float> getAndClearAudioBuffer();
	void ShowAudioDevices();
	AudioHandler();   // Constructor
	~AudioHandler(); //katastrofeas
	bool audioInit(); // portaudio initialization
	void startAudio();
	void stopAudio();
	void  ShowDefaultDevices() const;
	//bool AudioRecorder(int durationSeconds, const std::string& filename);
	static int audioCallback(const void* inputbuffer, void* outputbuffer, unsigned long framsPerBuffer, const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflag, void* userData);


};
