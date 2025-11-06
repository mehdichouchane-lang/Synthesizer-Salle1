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
	bNoise 				= false;
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	baseFreq = 440; 
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
		{ '�', Key::CSharp },
		{ 'z', Key::D },
		{ '"', Key::DSharp },
		{ 'e', Key::E },
		{ 'r', Key::F },
		{ '(', Key::FSharp },
		{ 't', Key::G },
		{ '-', Key::GSharp },
		{ 'y', Key::A },
		{ '�', Key::ASharp },
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
void ofApp::setupGui() {
	ofBackground(34, 34, 34);
	parameters.setName("parameters");
	parameters.add(octave.set("octave", 4, 1, 7));
	parameters.add(LaFreq.set("La Frequency", 440, 430, 450));
	// parameters.add(color.set("color",100,ofColor(0,0),255));
	parameters.add(volumeAudio.set("volume", 2, 0, 30));
	QwertyToggle.setup("Qwerty", false);
	QwertyToggle.addListener(this, &ofApp::onQwertyToggled);
	gui.setup(parameters);
	gui.add(&QwertyToggle);
	ofSetBackgroundColor(0);
}

//--------------------------------------------------------------
void ofApp::onQwertyToggled(bool & val) {
	qwertyActive = val;
	counterQwerty++;
	if (qwertyActive) {
		current_map = french_map;
		ofLogNotice() << "Qwerty mode ON";
	} else {
		current_map = qwerty_map;
		ofLogNotice() << "Qwerty mode OFF";
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(225);
	// ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	// ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);
	
	ofNoFill();
	// draw keys 
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 50, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Temporal Domain", 4, 18);
		
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
		ofTranslate(32, 250, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Frequency Domain", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			// ofBeginShape();
			// for (unsigned int i = 0; i < rAudio.size(); i++){
			// 	float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
			// 	ofVertex(x, 100 -rAudio[i]*180.0f);
			// }
			// ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
		
	ofSetColor(225);
	string reportString = "volume: ("+ofToString(volumeAudio, 2)+")\npan: ("+ofToString(pan, 2)+")\nsynthesis: ";
	if( !bNoise ){
		reportString += "sine wave (" + ofToString(baseFreq, 2) + " Hz)";
	}else{
		reportString += "noise";	
	}
	ofDrawBitmapString(reportString, 32, 579);

	
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
		}
	// Draw the black keys
		std::string blackKeyLabels;
		if (counterQwerty % 2) {
			blackKeyLabels = "�\"(-�";
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
}
float ofApp::keyFreq(int key, int LaFreq) {
	char charKey = static_cast<char>(key);
	auto it = current_map.find(charKey);
	if (it != current_map.end()) {
		int n = static_cast<int>(it->second);
		return LaFreq * std::pow(2.0, (n-9+(octave-4)*12) / 12.0);
	}
	return 0.0f; // or some default
}

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
void ofApp::drawGui(ofEventArgs & args){
	ofSetColor(115);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	char charKey = static_cast<char>(key);
	if (current_map.find(charKey) != current_map.end()) {
		pan = 0.5;
		targetFrequency = keyFreq(key, LaFreq);
		phaseAdderTarget = (targetFrequency / (float)sampleRate) * glm::two_pi<float>();
		setFlags(key, flags, true);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
	setFlags(key, flags, false);
	phaseAdderTarget = 0.;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
 
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
			lAudio[i] = buffer[i * buffer.getNumChannels()] = ofRandom(0, 1) * (0.05 * volumeAudio) * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = ofRandom(0, 1) * (0.05 * volumeAudio) * rightScale;
		}
	} else {
		phaseAdder = phaseAdderTarget; //0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * (0.05 * volumeAudio) * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * (0.05 * volumeAudio) * rightScale;
		}
	}

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
