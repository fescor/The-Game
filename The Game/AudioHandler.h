#pragma once
#include <portaudio.h>
#include <iostream>
#include "GameState.h"

class AudioHandler {
private:
	bool initialized;
	
public:
	
	AudioHandler();   // Constructor
	bool audioInit(); // portaudio initialization
	int countAudioDevices() const;    // Get the number of audio devices
	void listAudioDevices() const;	  // List audio devices 
	void ShowDefaultDevices() const;
};
