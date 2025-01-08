#pragma once
#include <portaudio.h>
#include <iostream>
#include "GameState.h"
#include <vector>
#include <mutex>
#include "GameState.h"
#include "GameObject.h"


class AudioHandler : public GameObject {
private:
	bool initialized;
	PaStream* stream = nullptr;

	//bool pb_initialized;
	//PaStream* pb_stream = nullprt;

	int player_id;



public:
	static std::vector <float> globalAudioBuffer; //buffer to send audio
	static std::mutex buffermutex; //mutex for safety 
	//static std::vector<float> playbackBuffer; // buffer to playback
	static std::map<int, std::vector<float>> playbackBuffer;
	static std::mutex playbackMutex; // mutex for playbacksafety
	static  std::vector<float> getAndClearAudioBuffer();
	void ShowAudioDevices();
	AudioHandler();   // Constructor
	~AudioHandler(); //katastrofeas
	bool audioInit(); // portaudio initialization
	void startAudio(); //start audio stream to record
	void startplaybackstream(); //starts audio stream to playback
	void stopAudio();
	void  ShowDefaultDevices() const;
	//bool AudioRecorder(int durationSeconds, const std::string& filename);
	static int audioCallback(const void* inputbuffer, void* outputbuffer, unsigned long framsPerBuffer, const PaStreamCallbackTimeInfo* timeinfo, PaStreamCallbackFlags statusflag, void* userData);
	//void preparedata(const std::vector<float>& buffer);
	void preparedata();
	void setbuffer(int i, const std::vector<float>& buffer);
	void checkAndStopAudio();
	bool isPlaybackFinished;
	bool playbackFinishedFlag;
};
