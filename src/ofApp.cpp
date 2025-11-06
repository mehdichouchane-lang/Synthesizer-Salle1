#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(34, 34, 34);
	ofTrueTypeFont font;
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	baseFreq = 220; 
	soundStream.printDeviceList();
	widthWhiteKey = 60;
	keySpace = 3;
	heightWhiteKey = 200;
	widthBlackKey = widthWhiteKey / 2;
	heightBlackKey = heightWhiteKey / 2;
	xStart = 30;
	yStart = 650;
	qwertyButtonStartX = xStart + 7 * (keySpace + widthWhiteKey);
	qwertyButtonStartY = yStart;
	qwertyButtonEndX = qwertyButtonStartX + widthWhiteKey;
	qwertyButtonEndY = qwertyButtonStartY + heightBlackKey;
	counterQwerty = 0;
	for (int i = 0; i < 10; i++) {
		flags[i] = false;
	}
	for (int i = 0; i < 5; i++) {
		flagsFreq[i] = false;
	}
	flagsFreq[0] = true;
	
	qwerty_map = {
		{ 'q', Key::C },
		{ '2', Key::CSharp },
		{ 'w', Key::D },
		{ '3', Key::DSharp },
		{ 'e', Key::E },
		{ 'r', Key::F },
		{ '5', Key::FSharp },
		{ 't', Key::G },
		{ '6', Key::GSharp },
		{ 'y', Key::A },
		{ '7', Key::ASharp },
		{ 'u', Key::B }
	};
	french_map = {
		{ 'a', Key::C },
		{ 'é', Key::CSharp },
		{ 'z', Key::D },
		{ '"', Key::DSharp },
		{ 'e', Key::E },
		{ 'r', Key::F },
		{ '(', Key::FSharp },
		{ 't', Key::G },
		{ '-', Key::GSharp },
		{ 'y', Key::A },
		{ 'è', Key::ASharp },
		{ 'u', Key::B }
	};
	current_map = qwerty_map;
	ofSoundStreamSettings settings;
	

	// if you want to set the device id to be different than the default:
	//
	//	auto devices = soundStream.getDeviceList();
	//	settings.setOutDevice(devices[3]);

	// you can also get devices for an specific api:
	//
	//	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::PULSE);
	//	settings.setOutDevice(devices[0]);

	// or get the default device for an specific api:
	//
	// settings.api = ofSoundDevice::Api::PULSE;

	// or by name:
	//
	//	auto devices = soundStream.getMatchingDevices("default");
	//	if(!devices.empty()){
	//		settings.setOutDevice(devices[0]);
	//	}


	// Latest linux versions default to the HDMI output
	// this usually fixes that. Also check the list of available
	// devices if sound doesn't work

	//settings.setApi(ofSoundDevice::MS_ASIO);
	//settings.setApi(ofSoundDevice::MS_WASAPI);
	//settings.setApi(ofSoundDevice::MS_DS);

	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
	}




	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);

	// on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
	// use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
}


//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(225);
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);
	
	ofNoFill();
	// draw keys 
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 150, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < lAudio.size(); i++){
				float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
				ofVertex(x, 100 -lAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 350, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < rAudio.size(); i++){
				float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
				ofVertex(x, 100 -rAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
		
	ofSetColor(225);
	string reportString = "volume: ("+ofToString(volume, 2)+") modify with -/+ keys\npan: ("+ofToString(pan, 2)+") modify with mouse x\nsynthesis: ";
	if( !bNoise ){
		reportString += "sine wave (" + ofToString(baseFreq, 2) + "hz) modify with mouse y";
	}else{
		reportString += "noise";	
	}
	ofDrawBitmapString(reportString, 32, 579);
	/*
	int whiteCounter = 0;
	if (i == 1 || i == 3 || i == 6 || i == 8 || i == 10) {
		ofSetLineWidth(1);
		if (flags[i]) {
			ofSetColor(255, 0, 0);
		} else {
			ofSetColor(0, 255, 0);
		}
		ofFill();
		ofDrawRectangle(30 + whiteCounter * (5 + 10), 650, 5, 25);
	}*/
	
	// Draw the white keys
	//ofTrueTypeFont font;
	//font.load("verdana.ttf", 24); // 24 is the font size
	std::string whiteKeyLabels;
	if (counterQwerty % 2) {
		whiteKeyLabels = "azertyu";
	} else {
		whiteKeyLabels = "qwertyu";
	}
	for (int i = 0; i < 7; i++) {	 
			ofSetLineWidth(1);
			if (flags[i]) {
				ofSetColor(255, 0, 144);
			} else {
				ofSetColor(255);
			}		
			ofFill();
			float x = xStart + i * (keySpace + widthWhiteKey);
			ofDrawRectangle(x, yStart, widthWhiteKey, heightWhiteKey);
			ofSetColor(0);
			float textX = x + widthWhiteKey/ 2 - 4; // adjust -4 for approx center text horizontally
			float textY = yStart + heightWhiteKey - 15; // slightly above bottom to be visible
			ofDrawBitmapString(std::string(1, whiteKeyLabels[i]), textX, textY);
			//font.drawString(std::string(1, whiteKeyLabels[i]), textX, textY);
		}
	// Draw the black keys
		std::string blackKeyLabels;
		if (counterQwerty % 2) {
			blackKeyLabels = "é\"(-è";
		} else {
			blackKeyLabels = "23567";
		}
		
		int blackKeyOffsets[] = { 0, 1, 3, 4, 5 }; // where black keys are placed
		for (int i = 0; i < 5; ++i) {
			int keyIdx = blackKeyOffsets[i];
			ofSetLineWidth(1);
			if (flags[7 + i]) { // assuming black key flags start at flags[7]
				ofSetColor(255, 0, 144);
			} else {
				ofSetColor(0); // black
			}
			ofFill();
			// Centered between the white keys
			float x = xStart + (keyIdx + 1) * widthWhiteKey + keyIdx * keySpace - widthBlackKey / 2;
			ofDrawRectangle(x, yStart, widthBlackKey, heightBlackKey);
			ofSetColor(255);
			float textX = x + widthBlackKey / 2 - 4;
			float textY = yStart + heightBlackKey - 15;
			ofDrawBitmapString(std::string(1, blackKeyLabels[i]), textX, textY);
		}
		ofFill();
		// qwerty/azerty button
		
		ofFill();
		ofDrawRectangle(qwertyButtonStartX, qwertyButtonStartY, 100, 100);
	for (int i = 0; i < 5; i++) {
		ofSetLineWidth(1);
		if (flagsFreq[i]) {
			ofSetColor(220);
			ofFill();
		} else {

			ofSetColor(255);
			ofFill();
		}

		
		ofDrawRectangle(700, 600 + i * (20), 50, 20);
		ofSetColor(10, 10, 10); 
		ofDrawBitmapString(ofToString(220 + i * 110), 708, 596 + (i+1) * (20));
	}
}
float ofApp::keyFreq(int key, int baseFreq) {
	char charKey = static_cast<char>(key);
	auto it = current_map.find(charKey);
	if (it != current_map.end()) {
		int n = static_cast<int>(it->second);
		return baseFreq * std::pow(2.0, n / 12.0);
	}
	return 0.0f; // or some default
}
/*
float ofApp::keyFreq(int key,int baseFreq) {
	std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	int n = 0; 
	n = qwerty_map[key]; 
	return baseFreq * std::pow(2.0, n / 12.0);
}
*/
/* void ofApp::setFlags(int key, bool flags[], bool val) {
	 std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	auto it = qwerty_map.find(key);
	if (it != qwerty_map.end()) {
		int index = it->second; // index from map (1-based)
		flags[index - 1] = val; // set to false, adjusting to 0-based array index
	}
}*/
void ofApp::setFlags(int key, bool flags[], bool val) {
	char charKey = static_cast<char>(key);
	auto it = current_map.find(charKey);
	if (it != current_map.end()) {
		// Convert enum Key to int index safely
		int index = static_cast<int>(it->second);
		if (index >= 0 && index < 12) {
			flags[index] = val;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	/* std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	*/
	char charKey = static_cast<char>(key);
	if (current_map.find(charKey) != current_map.end()) {
		pan = 0.5;
		targetFrequency = keyFreq(key,baseFreq);
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
		setFlags(key, flags, true);
	}
	/*
	if (key == 'q') {
		pan = 0.5;
		targetFrequency = keyFreq(key);
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
		setFlags(key,flags,true);
	}
	if (key == 'w') {
		pan = 0.5;
		targetFrequency = keyFreq(key);
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
		setFlags(key, flags, true);
	}
	*/
	if (key == '-' || key == '_') {
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=') {
		volume += 0.05;
		volume = std::min(volume, 1.f);
	}



	
}


//--------------------------------------------------------------
/*void ofApp::keyPressed  (int key){
	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = std::min(volume, 1.f);
	}
	
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
	

	
}*/

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
	setFlags(key, flags, false);
	phaseAdderTarget = 0.;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	/*int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();*/ 
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if ((x <= qwertyButtonEndX && x >= qwertyButtonStartX) && (y <= qwertyButtonEndY && y >= qwertyButtonStartY)) {
		counterQwerty++;
		if (counterQwerty % 2) {
			current_map = french_map;
		} else {
			current_map = qwerty_map;
		}
	}
	if ((x <= 750 && x >= 700) && (y <= 700 && y >= 600)) {
		
			int r = (y-600) % 20;
		int pos = (y - 600 - r) / 20;
			for (int i = 0; i < 5; i++) {
				flagsFreq[i] = false;
			}
			flagsFreq[pos] = true;
			baseFreq = 220 + pos * 110; 

	}

	if ((x < 40 && x > 30) && (y < 700 && y >= 650)) {
		bNoise = true;
		pan = 0;
		flags[0] = true;

	}
	else if ((x < 55 && x > 45) && (y < 700 && y >= 650)) {
		bNoise = true;
		pan = 1;
		flags[1] = true;

	}
	else if ((x < 70 && x > 60) && (y < 700 && y >= 650)) {
		bNoise = false;
		pan = 0.5;
		float height = (float)ofGetHeight();
		float heightPct = ((height - 550) / height);
		targetFrequency = 2000.0f * heightPct;
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
		flags[2] = true;

	}
	//bNoise = true;
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bNoise = false;
	for (int i = 0; i < 10; i++) {
		flags[i] = false;
	}

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	// sin (n) seems to have trouble when n is very large, so we
	// keep phase in the range of 0-glm::two_pi<float>() like this:
	while (phase > glm::two_pi<float>()){
		phase -= glm::two_pi<float>();
	}

	if ( bNoise == true){
		// ---------------------- noise --------------
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	} else {
		phaseAdder = phaseAdderTarget; //0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume * rightScale;
		}
	}

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
