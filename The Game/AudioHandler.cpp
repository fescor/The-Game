#include "AudioHandler.h"
#include "GameState.h"
#include "Player.h"
#include "Net.h"
#include <fstream>
#include <queue>
#include <cstdint>   // gia int16_t
#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>


using namespace std;
#define _USE_MATH_DEFINES
std::vector<float> AudioHandler::finalboss;
std::vector<float> AudioHandler::globalAudioBuffer;
std::vector <float> AudioHandler::temp_vector;

std::mutex AudioHandler::buffermutex;
//std::map<int, std::vector<float>> AudioHandler::playbackMap;
std::map<int, std::queue<float>> AudioHandler::playbackMap;
std::mutex AudioHandler::playbackMutex;
bool streamcloseflag = false;
bool AudioHandler::dataready = false;
std::condition_variable playbackCondition;



// Constructor implementation
AudioHandler::AudioHandler() {
	AudioHandler::audioInit();

	
}

//katastrofeas
AudioHandler::~AudioHandler() {
	//stop and close stream if is still open 
	if (stream) {
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
	}

	//terminate portaudio if initialized 
	if (initialized) {
		Pa_Terminate();
	}
}

bool AudioHandler::audioInit() {
	if (initialized) {
		return true;
	}
	PaError err;
	err = Pa_Initialize();

	if (err != paNoError) {
		std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
		return false;
	}
	initialized = true; 
	std::cout << "Portaudio Initialized" << std::endl; 
	return true;

}


void AudioHandler::startAudio(){
	
	if (stream && Pa_IsStreamActive(stream)) {
		std::cerr << "Audio stream is already active!" << std::endl;
		return; // ean exei anoiksei mhn ksana anoigeis
	}
	if (!initialized) {
		cerr << "Portaudio not initialized." << endl;
		return ;
	}

	PaError err; 

	PaStreamParameters inputparametr, outputparametr; 

	inputparametr.device = Pa_GetDefaultInputDevice(); //get default mic as input device
	

	if (inputparametr.device == paNoDevice) {
		std::cerr << "Cant find default input device." << std::endl;
		return;
	}
	//PaDeviceIndex deviceIndex = 1; 
	//inputparametr.device = deviceIndex;
	inputparametr.channelCount = 1; //stereo audio
	inputparametr.sampleFormat = paFloat32; // 32-bit floating-point samples  
	inputparametr.suggestedLatency = Pa_GetDeviceInfo(inputparametr.device)->defaultLowInputLatency;
	inputparametr.hostApiSpecificStreamInfo = nullptr;

	

	//open audio stream 
	err = Pa_OpenStream(&stream, &inputparametr, nullptr, 48000, 512, paFloat32, &AudioHandler::audioCallback,nullptr);
	if (err != paNoError) {
		std::cerr << "Fail to open stream: " << Pa_GetErrorText(err) << std::endl;
		return; 
	}
	//start audio stream 
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		std::cerr << "Fail to start stream" << Pa_GetErrorText(err) << std::endl; 
		return;
	}
	else {
		std::cout << "Audio stream just started!" << std::endl;
	}
	
	
	
}


//stop stream and playblack 
void AudioHandler::stopPlaybackAudio() {
	if (outstream) {
		//stop audio stream
		PaError err = Pa_StopStream(outstream);
		if (err != paNoError) {
			std::cerr << "Fail to stop audio stream." << Pa_GetErrorText(err) << std::endl;
			return;
		}
		//close audio stream
		err = Pa_CloseStream(outstream);
		if (err != paNoError) {
			std::cerr << "Fail to close audio stream." << Pa_GetErrorText(err) << std::endl;
			return;
		}
		outstream = nullptr;
		std::cout << "input Audio stream just stopped and closed!" << std::endl;
		//AudioHandler::getAndClearAudioBuffer();
	}
}
void AudioHandler::stopAudio() {
	if (stream) {
		//stop audio stream
		PaError err = Pa_StopStream(stream);
		if (err != paNoError) {
			std::cerr << "Fail to stop audio stream." << Pa_GetErrorText(err) << std::endl;
			return;
		}

		//close audio stream
		err = Pa_CloseStream(stream);
		if (err != paNoError) {
			std::cerr << "Fail to close audio stream." << Pa_GetErrorText(err) << std::endl;
			return;
		}

		stream = nullptr;
		std::cout << "Audio stream just stopped and closed!" << std::endl;
		//AudioHandler::getAndClearAudioBuffer();
		

	}
}



//call back function for processing audio 
int AudioHandler::audioCallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {

	const float* in = static_cast<const float*>(inputBuffer); //input data 
	
	//bool isPlaybackFinished = false;

	if (inputBuffer) {
		std::vector<float> chunk(in, in + framesPerBuffer); //copy chunk in local vector
		/*
		{
			std::lock_guard<std::mutex> lock(buffermutex);// mutex lock
			globalAudioBuffer.insert(globalAudioBuffer.end(), chunk.begin(), chunk.end());
		}
		if (globalAudioBuffer.size() >= framesPerBuffer) {
			dataready = true;
		}
		*/
		auto gamestate = GameState::getInstance();
		gamestate->getNet()->addaudiodata(*(gamestate)->getPlayer()->geto_id(), chunk.data());
	}
	//std::cout << "audiocallback size " << globalAudioBuffer.size() << std::endl;
	
	return paContinue;
}


 void AudioHandler::preparedata() {
	//std::vector<float> audioData = AudioHandler::getAndClearAudioBuffer();
	
	 
	 while (!dataready) { 
		 std::this_thread::sleep_for(std::chrono::milliseconds(200));
	 }

	 {
		 std::lock_guard<std::mutex> lock(buffermutex);
		 temp_vector = std::move(globalAudioBuffer);
		 globalAudioBuffer.clear();
	 }
	 int playerid = *(m_state)->getPlayer()->geto_id();

	 while (!temp_vector.empty() && Pa_IsStreamActive(stream)) //or na exei ksekinhsei to stream 
	 { 
			 float preparechunk[512] = { 0 };
			 //send fist 512 frames
			 size_t dataToCopy = min(temp_vector.size(), size_t(512));
			 std::copy(temp_vector.begin(), temp_vector.begin() + dataToCopy, preparechunk);
			 
			 m_state->getNet()->addaudiodata(playerid, preparechunk);
			 //erase the data that sended
			 temp_vector.erase(temp_vector.begin(), temp_vector.begin() + 512);
			 
	 }
	 std::cout << "prepare to send data with size: " << globalAudioBuffer.size() << std::endl;
}

 void AudioHandler::setbuffer(int i, const std::vector<float>& chunk) {
	 {
		
		 if (playbackMap.find(i) == playbackMap.end()) {
			 //playbackMap[i] = std::vector<float>(); //create a buffer for each player when he send voice data 
			 playbackMap[i] = std::queue<float>();
		 }
		 
		 std::lock_guard<std::mutex> lock(playbackMutex);
		 auto& playerbuffer = playbackMap[i];
		
		 //insert
		 for (const auto& sample : chunk) {
			 playerbuffer.push(sample); 
		 }
		 
		// dataready = true;
		std::cout << "Buffer for player " << i << " has been updated with chunk of size: " << chunk.size() << std::endl;
	 }
	// playbackCondition.notify_one();

 }


 void AudioHandler::startplaybackstream() {
	 if (outstream && Pa_IsStreamActive(outstream)) {
		 std::cerr << "Audio stream is already active!" << std::endl;
		 return; // ean exei anoiksei mhn ksana anoigeis
	 }
	 if (!initialized) {
		 cerr << "Portaudio not initialized." << endl;
		 return;
	 }
	 PaStreamParameters outputparametr;
	 outputparametr.device = Pa_GetDefaultOutputDevice(); // get default speakers as output device
	 //PaDeviceIndex outputparametr.device = Pa_GetDefaultOutputDevice();
	 if (outputparametr.device == paNoDevice) {
		 std::cerr << "Cant find default output device." << std::endl;
		 return;
	 }
	 outputparametr.channelCount = 1;
	 outputparametr.sampleFormat = paFloat32;
	 outputparametr.suggestedLatency = Pa_GetDeviceInfo(outputparametr.device)->defaultLowInputLatency;
	 outputparametr.hostApiSpecificStreamInfo = nullptr;



	 //paClipOff: disable clipping, paDitherOff: disable dithering
	 PaError err = Pa_OpenStream(&outstream, nullptr, &outputparametr, 48000, 512, paFloat32, &AudioHandler::playbackcallback, nullptr);
	 if (err != paNoError) {
		 std::cerr << "Pa_OpenStream failed: " << Pa_GetErrorText(err) << std::endl;
		 return;
	 }
	 //start stream
	 err = Pa_StartStream(outstream);
	 if (err != paNoError) {
		 std::cerr << "Pa_StartStream failed: " << Pa_GetErrorText(err) << std::endl;
		 return;
	 }
	 else {
		 std::cout << "Output Stream started" << std::endl;
	 }

 }


 int AudioHandler::playbackcallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	 const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
	 float* out = static_cast<float*>(outputBuffer);
	 //std::fill(out, out + framesPerBuffer, 0.0f);
	 float playbackchunk[512] = { 0 };
	 {
		 // std::lock_guard<std::mutex> lock(playbackMutex);
		 for (auto& it : playbackMap) {
			 auto& playerbuffer = it.second;

			 if (!playerbuffer.empty()) {

				 size_t dataToPlayback = min(framesPerBuffer, playerbuffer.size());
				 for (size_t i = 0; i < dataToPlayback; i++) {
					 playbackchunk[i] += playerbuffer.front();
					 playerbuffer.pop();

				 }
				 // playerbuffer.erase(playerbuffer.begin(), playerbuffer.begin() + dataToPlayback);
				  // Reset the flag if buffer is empty after playback	

			 }

		 }
		 for (size_t i = 0; i < framesPerBuffer; i++) {
			 out[i] = playbackchunk[i];

		 }
	 }
	 return paContinue;
 }


