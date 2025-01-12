#pragma once
#include <portaudio.h>
#include <iostream>
#include "GameState.h"
#include <vector>
#include <mutex>
#include <queue>
#include "GameState.h"
#include "GameObject.h"


class AudioHandler : public GameObject {
private:
	bool initialized;
	PaStream* stream = nullptr;
	PaStream* outstream = nullptr;

	//bool pb_initialized;
	//PaStream* pb_stream = nullprt;

	int player_id;
	


public:
	static std::vector<float> finalboss;
	static std::vector <float> globalAudioBuffer; //buffer to send audio
	static std::vector <float> temp_vector;//temp vector to copy globalaudiobuffer
	static std::mutex buffermutex; //mutex for safety 
	//static std::vector<float> playbackBuffer; // buffer to playback
	//static std::map<int, std::vector<float>> playbackMap;
	static std::map<int, std::queue<float>> playbackMap;	//map to store the audio data of each player
	static std::mutex playbackMutex; // mutex for playbacksafety
	//std::vector <float> temp_buffer; //buffer that when is empty playback should finish 
	
	//static  std::vector<float> getAndClearAudioBuffer();
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
	//void preparedata(const std::vector<floatconst void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	static int playbackcallback (const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) ;
	void preparedata();
	void setbuffer(int i, const std::vector<float>& buffer);
	bool closecall();
	static bool dataready ;
	//static bool streamcloseflag;
};
