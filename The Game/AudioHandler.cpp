#include "AudioHandler.h"
#include <fstream>
#include <cstdint>   // gia int16_t
#include <iostream>

using namespace std;


// Constructor implementation
AudioHandler::AudioHandler() {
	
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
//epistrefei ton arithmo twn suskeuwn hxou 
int AudioHandler::countAudioDevices() const {
	if (!initialized) {
		std:std::cerr << "Portaudio not initialized" << std::endl;
		return -1;
	}
	int devc = Pa_GetDeviceCount();
	if (devc < 0) {
		std::cerr << "Error retrieving device count: " << Pa_GetErrorText(devc) << std::endl;
		return -1;
	}
	std::cout << "You have " << devc << "devices" << std::endl;
	return devc;
}

void AudioHandler::listAudioDevices() const {
	int devc = Pa_GetDeviceCount();
	
	if (!initialized) {
	std:std::cerr << "Portaudio not initialized" << std::endl;
		return ;
	}

	if (devc <= 0) {
		std::cerr << "No audio devices" << std::endl;
	}
	for (int i = 0; i < devc; i++) {

		const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
		if (deviceInfo) {
			std::cout << "Device " << i << ": " << deviceInfo->name << std::endl;
		}
	}
}

//apo edw briskei swsta tis default suskeues pou xrisimopoiei (toulaxiston sto diko mou) 
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