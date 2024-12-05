#include "AudioHandler.h"
using namespace std;


// Constructor implementation
AudioHandler::AudioHandler() {
	
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