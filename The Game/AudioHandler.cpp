#include "AudioHandler.h"
#include <fstream>
#include <cstdint>   // gia int16_t
#include <iostream>

using namespace std;



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

//initialize audioport logika tha kaleite otan ksekinaei to game
bool AudioHandler::audioInit() {
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


void AudioHandler::startAudio(){
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

	inputparametr.channelCount = 2; //stereo audio
	inputparametr.sampleFormat = paFloat32; // 32-bit floating-point samples
	inputparametr.suggestedLatency = Pa_GetDeviceInfo(inputparametr.device)->defaultLowInputLatency;
	inputparametr.hostApiSpecificStreamInfo = nullptr;

	outputparametr.device = Pa_GetDefaultOutputDevice(); // get default speakers as output device
	//PaDeviceIndex outputparametr.device = Pa_GetDefaultOutputDevice();
	if (outputparametr.device == paNoDevice) {
		std::cerr << "Cant find default output device." << std::endl;
		return;
	}
	outputparametr.channelCount = 2;
	outputparametr.sampleFormat = paFloat32;
	outputparametr.suggestedLatency = Pa_GetDeviceInfo(outputparametr.device)->defaultLowInputLatency;
	outputparametr.hostApiSpecificStreamInfo = nullptr; 

	//open audio stream 
	err = Pa_OpenStream(&stream, &inputparametr, &outputparametr, 44100, 256, paClipOff, audioCallback, nullptr);
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
		PaError err =  Pa_StopStream(stream);
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

	}
}

//call back function for processing audio 
int AudioHandler::audioCallback(const void* inputBuffer, void* outputBuffer,unsigned long framesPerBuffer,const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags,void* userData) {
	const float* in = static_cast<const float*>(inputBuffer); //input data 
	float* out = static_cast<float*>(outputBuffer); //output data
	if (inputBuffer) {
		//if input buffer is available copy it directly 
		for (unsigned long i = 0; i < framesPerBuffer; i++) {
			out[i] = in[i];// real time playback for mic 
		}
	}
	else {
		//if no input fill output with silence 
		for (unsigned long i = 0; i < framesPerBuffer; i++) {
			out[i] = 0.0f;
		}
	}

	return paContinue; // continue the audiostream
}
/*
//prospatheia gia record apo to default input kai save se arxeio 

bool AudioHandler::AudioRecorder(int durationSeconds,const std::string& filename) {
	if (!initialized) {
		cerr << "Portaudio not initialized." << endl;
		return true;
	}

	//stream parameters
	PaStreamParameters inputpar; 
	memset(&inputpar, 0, sizeof(inputpar));

	//getdefault dev
	inputpar.device = Pa_GetDefaultInputDevice();
	if (inputpar.device == paNoDevice) {
		cerr << "No default input device found." << endl;
		return false;
	}
	const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(inputpar.device);
	inputpar.channelCount = 1; // Mono recording
	inputpar.sampleFormat = paInt16; // 16-bit integer format
	inputpar.suggestedLatency = deviceInfo->defaultLowInputLatency; 
	inputpar.hostApiSpecificStreamInfo = nullptr;

	const int sampleRate = 44100; //standar sample rate
	const unsigned long framesPerBuffer = 512; //buffer size

	//open stream 
	cout << "trying to openstream" << endl;
	PaError err = Pa_OpenStream(&stream, &inputpar, nullptr, sampleRate, framesPerBuffer, paClipOff, nullptr, nullptr);
	if (err != paNoError) {
		std::cerr << "Failed to open audio stream: " << Pa_GetErrorText(err) << std::endl;
		Pa_CloseStream(stream); //kleinw an ginei mlkia
		return false;
	}
	else
	{
		cout << "stream just open" << endl;
	}

	//start stream
	err = Pa_StartStream(stream);
	cout << "trying to start stream" << endl;
	if (err != paNoError) {
		std::cerr << "Failed to start audio stream: " << Pa_GetErrorText(err) << std::endl;
		Pa_CloseStream(stream);
		stream = nullptr;
		return false;
	}
	else
	{
		cout << "stream just start" << endl;
	}
	std::cout << "Recording audio for " << durationSeconds << " seconds..." << std::endl;

	//prepare file for saving audio data
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile.is_open()) {
		std::cerr << "Failed to open file for writing:" << filename << std::endl;
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
		stream = nullptr;
		return false;
	}

	//recording loop 
	int16_t* buffer = new int16_t[framesPerBuffer];
	int totalFrames = static_cast<int>(durationSeconds * sampleRate);

	for (int i = 0; i < totalFrames; i += framesPerBuffer) {
		err = Pa_ReadStream(stream, buffer, framesPerBuffer);
		if (err != paNoError) {
			std::cerr << "Error reading audio stream: " << Pa_GetErrorText(err) << std::endl;
			break;
		}
		//Write the audio data buffer to the output file
		outFile.write(reinterpret_cast<char*>(buffer), framesPerBuffer * sizeof(int16_t));
	}

	//cleanup
	delete[] buffer;
	outFile.close();

	err = Pa_StopStream(stream);
	if (err != paNoError) {
		std::cerr << "Error stopping stream : " << Pa_GetErrorText(err) << std::endl;
		return true;
	}
}
*/