#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(34, 34, 34);
	
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	
	soundStream.printDeviceList();
	for (int i = 0; i < 10; i++) {
		flags[i] = false;
	}
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
		reportString += "sine wave (" + ofToString(targetFrequency, 2) + "hz) modify with mouse y";
	}else{
		reportString += "noise";	
	}
	ofDrawBitmapString(reportString, 32, 579);
	for (int i = 0; i < 12; i++) {
		ofSetLineWidth(1);
		if (flags[i]) {
			ofSetColor(255, 0, 0);
		} else {
			ofSetColor(255);
		}
		
		ofFill();
		ofDrawRectangle(30+i*(5+10), 650, 10, 50);
	}

//	ofSetColor(255);                // white color for keys
//    int nKeys = 10; // número de teclas que você quer
	float keyWidth = 60; // largura de cada tecla
	float keyHeight = 200; // altura de cada tecla

    for(int i = 0; i < nKeys; i++){
        float x = i * keyWidth;
        float y = ofGetHeight() / 2 - keyHeight / 2;

        ofDrawRectangle(x, y, keyWidth, keyHeight);  // draw white key
        ofNoFill();
        ofSetColor(0);              // black outline
        ofDrawRectangle(x, y, keyWidth, keyHeight);
        ofFill();

        // reset to white for next key
        ofSetColor(255);
    }

    // title
    ofSetColor(220);
    ofDrawBitmapString("Simple Clavier (Piano) — 10 white keys", 20, 30);
}
float ofApp::keyFreq(int key) {
	std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	int n = 0; 
	n = qwerty_map[key]; 
	return 400 * std::pow(2.0, n / 12.0);
}
void ofApp::setFlags(int key, bool flags[],bool val) {
	std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	auto it = qwerty_map.find(key);
	if (it != qwerty_map.end()) {
		int index = it->second; // index from map (1-based)
		flags[index - 1] = val; // set to false, adjusting to 0-based array index
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	std::unordered_map<char, int> qwerty_map = {
		{ 'q', 1 }, { 'w', 2 }, { 'e', 3 }, { 'r', 4 }, { 't', 5 }, { 'y', 6 },
		{ 'u', 7 }, { 'i', 8 }, { 'o', 9 }, { 'p', 10 }, { 'a', 11 }, { 's', 12 }
	};
	
	char charKey = static_cast<char>(key);
	if (qwerty_map.find(charKey) != qwerty_map.end()) {
		pan = 0.5;
		targetFrequency = keyFreq(key);
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




	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = std::min(volume, 1.f);
	}
	/*
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
	}
	*/
	
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
	setFlags(key, flags, false);
	phaseAdderTarget = 0.;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
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
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
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
