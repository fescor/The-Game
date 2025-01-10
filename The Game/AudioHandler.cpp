#include "AudioHandler.h"
#include "GameState.h"
#include "Player.h"
#include "Net.h"
#include <fstream>
#include <cstdint>   // gia int16_t
#include <iostream>
#include <vector>
#include <mutex>


using namespace std;
#define _USE_MATH_DEFINES

using namespace std;
std::vector<float> AudioHandler::finalboss;
std::vector<float> AudioHandler::globalAudioBuffer;
std::mutex AudioHandler::buffermutex;
std::map<int, std::vector<float>> AudioHandler::playbackMap;
std::mutex AudioHandler::playbackMutex;


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


void AudioHandler::ShowAudioDevices() {
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		std::cerr << "PortAudio initialization failed: " << Pa_GetErrorText(err) << std::endl;
		return;
	}

	int numDevices = Pa_GetDeviceCount();
	if (numDevices < 0) {
		std::cerr << "ERROR: Pa_GetDeviceCount returned " << numDevices << std::endl;
		return;
	}

	std::cout << "Available audio devices:\n";

	for (int i = 0; i < numDevices; i++) {
		const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
		if (!deviceInfo) continue;

		std::cout << "Device " << i << ": " << deviceInfo->name << "\n";
		std::cout << "  Max input channels: " << deviceInfo->maxInputChannels << "\n";
		std::cout << "  Max output channels: " << deviceInfo->maxOutputChannels << "\n";
		std::cout << "  Default sample rate: " << deviceInfo->defaultSampleRate << "\n";
		std::cout << "  Host API: " << Pa_GetHostApiInfo(deviceInfo->hostApi)->name << "\n";

		if (i == Pa_GetDefaultInputDevice()) {
			std::cout << "  [Default Input Device]\n";
		}
		if (i == Pa_GetDefaultOutputDevice()) {
			std::cout << "  [Default Output Device]\n";
		}

		std::cout << "---------------------------------\n";
	}

	Pa_Terminate();
}


//TODO: O KATHE PAIXTHS NA MPOREI NA EPILEKSEI THN SISKEUH POU THELEI GIA EISODO KAI EKSODO HXOU ANTI NA PICKARW TA DEFAULT 
void AudioHandler::ShowDefaultDevices() const {
	if (!initialized) {
		cerr << "Portaudio not initialized." << endl;
		return;
	}
	//get input default device
	PaDeviceIndex definputdev = Pa_GetDefaultInputDevice();
	if (definputdev == paNoDevice) {
		cerr << "No default input device found." << endl;
	}
	else {
		const PaDeviceInfo* inputinfo = Pa_GetDeviceInfo(definputdev);
		if (inputinfo) {
			cout << "Default input device: " << inputinfo->name << endl;
		}
	}
	//get output default device
	PaDeviceIndex defoutputdev = Pa_GetDefaultOutputDevice();
	if (defoutputdev == paNoDevice) {
		cerr << "No default output device found." << endl;
	}
	else {
		const PaDeviceInfo* outputinfo = Pa_GetDeviceInfo(defoutputdev);
		if (outputinfo) {
			cout << "Default output device: " << outputinfo->name << endl;
		}
	}
}

void AudioHandler::startplaybackstream(){
	if (stream && Pa_IsStreamActive(stream)) {
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

	PaError err = Pa_OpenStream(&stream, nullptr, &outputparametr, 48000, 512, paClipOff, playbackcallback, nullptr);
	if (err != paNoError) {
		std::cerr << "Pa_OpenStream failed: " << Pa_GetErrorText(err) << std::endl; 
		return;
	}
	//start stream
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		std::cerr << "Pa_StartStream failed: " << Pa_GetErrorText(err) << std::endl;
		return; 
	}
	else {
		std::cout << "Stream started" << std::endl;
	}
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

	//open audio stream 
	err = Pa_OpenStream(&stream, &inputparametr, &outputparametr, 48000, 512, paClipOff, audioCallback,nullptr);
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
	float* out = static_cast<float*>(outputBuffer); //output data
	//bool isPlaybackFinished = false;

	if (inputBuffer) {
		std::vector<float> chunk(in, in + framesPerBuffer); //copy chunk in local vector

		{
			std::lock_guard<std::mutex> lock(buffermutex);// mutex lock
			globalAudioBuffer.insert(globalAudioBuffer.end(), chunk.begin(), chunk.end());
		}
	}
	else {
		std::cout << "No input detected, outputting silence." << std::endl;
		for (unsigned long i = 0; i < framesPerBuffer; i++) {
			out[i] = 0.0f; // Silence
			}
		}
	
	//std::cout << "audiocallback size " << globalAudioBuffer.size() << std::endl;
	return paContinue;
}

//fix this
std::vector<float> AudioHandler::getAndClearAudioBuffer() {
	std::lock_guard<std::mutex> lock(buffermutex);// mutex lock
	std::vector<float> data = std::move(globalAudioBuffer);	//move buffer to data (local)
	globalAudioBuffer.clear();// katharismos buffer
	return data;
}

 void AudioHandler::preparedata() {
	//std::vector<float> audioData = AudioHandler::getAndClearAudioBuffer();
	 std::cout << "prepare to send data with size: " << globalAudioBuffer.size() << std::endl;

	 int playerid = *(m_state)->getPlayer()->geto_id();
	 while (!globalAudioBuffer.empty())
	 { //an exei arketa dedomena
			 float preparechunk[512] = { 0 };
			 //send fist 512 frames
			 size_t dataToCopy = min(globalAudioBuffer.size(), size_t(512));
			 std::copy(globalAudioBuffer.begin(), globalAudioBuffer.begin() + 512, preparechunk);
			 
			 m_state->getNet()->addaudiodata(playerid, preparechunk);
			 //erase the data that sended
			 //globalAudioBuffer.erase(globalAudioBuffer.begin(), globalAudioBuffer.begin() + 512);
			 
		 
	 }

}

 void AudioHandler::setbuffer(int i, const std::vector<float>& chunk) {
	 
	 if (playbackMap.find(i) == playbackMap.end()) {
		 playbackMap[i] = std::vector<float>(); //create a buffer for each player when he send voice data 
	}
	 //take players buffer and insert the data
	 auto& playerbuffer = playbackMap[i];
	 playerbuffer.insert(playerbuffer.end(), chunk.begin(), chunk.end());
	 const size_t maxBuffersize = 1024 * 5; 
	//an perasei to max size sbhse ta prwta pou esteile 
	 if (playerbuffer.size() > maxBuffersize) {
		 playerbuffer.erase(playerbuffer.begin(), playerbuffer.begin() + (playerbuffer.size() - maxBuffersize));

	 }
	 //std::cout << "Added audio chunk for player " << i << ". Buffer size: " << buffer.size() << " samples.\n";
	 //an den uparxei stream anoikse gia to playback
	 if (!stream) {
		 AudioHandler::startplaybackstream();
		 /*
		PaError err = Pa_WriteStream(stream, playbackBuffer[i].data(), playbackBuffer[i].size());
		 if (err != paNoError) {
			 std::cerr << "Error writing to stream: " << Pa_GetErrorText(err) << std::endl;
		 }
		 */
		 if (playbackMap.empty()) {
			 //when playback is finished 
			 stopAudio();
		 }
	 }

 }

 int AudioHandler::playbackcallback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	 const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
 {
	 /*
	 int i = 0;
	 std::vector<float> temp_vector = playbackBuffer.find(id_toupaixti);
	 for (auto iter = temp_vector.begin(); iter != temp_vector.end(); iter++) {
		 out[i] = iter;
		 i++
	 }
	 float* out = static_cast<float*>(outputBuffer);
	 for (auto& playerbuffer : playbackBuffer) {
		 int playerID = playerbuffer.first;
	 }
	// std::cout << "playbackcallback has to play chunk with size : " << globalAudioBuffer.size() << std::endl;
	
	 {
		 //thread safety 
		 std::lock_guard<std::mutex> lock(playbackMutex);// mutex lock
		 for (auto& a : playbackBuffer) {
			 out.push_back(std::)
		 }
		 for (auto it = playbackBuffer.begin(); it != playbackBuffer.end();) {
			 size_t dataToPlayback = min(playbackBuffer.begin()->second.size(), framesPerBuffer);

			 for (size_t i = 0; i < dataToPlayback; i++) {
				 out[i] = it->second;
				 

			 }

		 }
		
		
		 for (auto& playbackBuffer : playbackBuffer) {
			 //finalboss.push_back(std::move(playbackBuffer.second)); 
			 finalboss.insert(finalboss.end(), playbackBuffer.second.begin(), playbackBuffer.second.end());
		 }


		 // store playback size
		 
		 
		 //
		 //std::copy(playbackBuffer.begin()->second.begin(), playbackBuffer.begin()->second.begin() + dataToPlayback,out);

		 // delete what you have copy 
		 //playbackBuffer.begin()->second.erase(playbackBuffer.begin()->second.begin(),playbackBuffer.begin()->second.begin() + dataToPlayback);
		 finalboss.erase(finalboss.begin(), finalboss.begin() + dataToPlayback);
	
		 if (finalboss.empty()) {
			 playbackBuffer.erase(playbackBuffer.begin());
		 }
	 }

*/
	 float* out = static_cast<float*>(outputBuffer);
	 {
		 std::lock_guard<std::mutex> lock(playbackMutex);
		 for (auto& it : playbackMap) {
			 auto& playerbuffer = it.second; 

			 if (!playerbuffer.empty()) { 
				 //how many samples to play
				 size_t dataToPlayback;
				 if (framesPerBuffer >= playerbuffer.size()) {
					  dataToPlayback = framesPerBuffer;
				 }
				 else {
					 dataToPlayback = playerbuffer.size();

				 }
				 //add audio to output
				 for (size_t i = 0; i < dataToPlayback; i++) {
					 out[i] = playerbuffer[i];
				 }
			
				//erase what you have played 
				 playerbuffer.erase(playerbuffer.begin(), playerbuffer.begin() + dataToPlayback);
			 }

		 }
	 }

	
	 return paContinue;  
 }

