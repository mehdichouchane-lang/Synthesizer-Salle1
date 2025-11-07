#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(34, 34, 34);
	
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.0f;
	// phaseAdderTarget 	= 0.0f;
	// std::vector<float> phaseAdderTarget[10] = {1};
	bNoise 				= false;
	lAudio.assign(bufferSize, 0.0);
	rAudio.assign(bufferSize, 0.0);
	soundStream.printDeviceList();
	widthWhiteKey = 100;
	keySpace = 3;
	heightWhiteKey = 220;
	widthBlackKey = widthWhiteKey / 2;
	heightBlackKey = heightWhiteKey / 2;
	xStart = 150;
	yStart = 550;
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
	float scaleFourier = 3.;
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
	current_map = french_map;
	ofSoundStreamSettings settings;
	
		titleFont.load("Welbut.ttf", 48); // font file + size

	mapHarms = {
		{ '0', harm1 },
		{ '1', harm2 },
		{ '2', harm3 },
		{ '3', harm4 },
		{ '4', harm5 },
		{ '5', harm6 },
		{ '6', harm7 },
		{ '7', harm8 },
		{ '8', harm9 },
		{ '9', harm10 }
	};

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
	parameters.setName("Parameters");
	parameters.add(octave.set("octave", 4, 1, 7));
	parameters.add(LaFreq.set("C4 frequency", 440, 430, 450));
	// parameters.add(color.set("color",100,ofColor(0,0),255));
	parameters.add(volumeAudio.set("volume", 2, 0, 30));
	QwertyToggle.setup("Qwerty", false);
	QwertyToggle.addListener(this, &ofApp::onQwertyToggled);
	parameters.setName("Harmonic Parameters");
	parameters.add(harm1.set("First harmonic", 1, 0, 1));
	parameters.add(harm2.set("Second harmonic", 0, 0, 1));
	parameters.add(harm3.set("Third harmonic", 0, 0, 1));
	parameters.add(harm4.set("Fourth harmonic", 0, 0, 1));
	parameters.add(harm5.set("Fifth harmonic", 0, 0, 1));
	parameters.add(harm6.set("Sixth harmonic", 0, 0, 1));
	parameters.add(harm7.set("Seventh harmonic", 0, 0, 1));
	parameters.add(harm8.set("Eighth harmonic", 0, 0, 1));
	parameters.add(harm9.set("Nineth harmonic", 0, 0, 1));
	parameters.add(harm10.set("Tenth harmonic", 0, 0, 1));
	gui.setup(parameters);
	gui.add(&QwertyToggle);
	ofSetBackgroundColor(0);
}


//--------------------------------------------------------------
void ofApp::onQwertyToggled(bool & val) {
	qwertyActive = val;
	counterQwerty++;
	if (QwertyToggle) {
		current_map =  qwerty_map;
		ofLogNotice() << "Qwerty mode ON";
	} else {
		current_map = french_map;
		ofLogNotice() << "Qwerty mode OFF";
	}
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
namespace unsigned_types { 
    typedef std::basic_string<unsigned char> string;
}

//--------------------------------------------------------------
void ofApp::draw(){

	ofSetColor(225);
	// ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 32);
	// ofDrawBitmapString("press 's' to unpause the audio\npress 'e' to pause the audio", 31, 92);
	int offsetY = 100;
	ofSetColor(255, 215, 0); // gold
	titleFont.drawString("🎵 Synthesizer 3000 🎵", 100, 100);
	ofNoFill();
	// draw keys 
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, offsetY+50, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Temporal Domain", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
		int scaleSound = 1;
			for (unsigned int i = 0; i < lAudio.size(); i++){
			if (volumeAudio > 15) {
				scaleSound = 1.0f + volumeAudio / 20;
			} else {
				scaleSound = 1;
			}
				float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
				ofVertex(x, 100 -(lAudio[i]*180.0f/scaleSound));
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
		
	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
	ofTranslate(32, offsetY+250, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Frequency Domain", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
		
		// draw the fourier transform
		int halfSize = fourierSpectrum.size() / 2;
		if (!fourierSpectrum.empty()) {
			ofSetColor(0, 255, 0);
			ofNoFill();
			ofBeginShape();
			float logMin = std::log10(1.0f);
			float logMax = std::log10((float)halfSize);
			for (int i = 0; i < halfSize; i++) {
				float linearX = ofMap(i, 0, halfSize, 0, 900, true);
				float logX = std::log10(linearX);
				float x = ofMap(logX, logMin, logMax, 0, 900, true);
				ofVertex(x, (140 - fourierSpectrum[i] * 10.0f / (volumeAudio + 0.01f)));
			}
			ofEndShape(false);
		}
			
		ofPopMatrix();
	ofPopStyle();
	
		ofTranslate(32, offsetY, 0);
	ofSetColor(225);
	string reportString = "volume: ("+ofToString(volumeAudio, 2)+")\npan: ("+ofToString(pan, 2)+")\nsynthesis: ";
	if( !bNoise ){
		reportString += "Fundamental Frequency: " + ofToString(targetFrequency, 2) + " Hz";
	}else{
		reportString += "noise";	
	}
	ofDrawBitmapString(reportString, 32, 475);

	// Draw the white keys
	//ofTrueTypeFont font;
	//font.load("verdana.ttf", 24); // 24 is the font size
	std::string whiteKeyLabels;
	if (QwertyToggle) {
		whiteKeyLabels = "qwertyu";
	} else {
		whiteKeyLabels = "azertyu";
	}
	int whiteKeyLocs[] = { 0, 2, 4, 5, 7, 9, 11 };
	for (int i = 0; i < 7; i++) {	 
			ofSetLineWidth(1);
			int flagIdx = whiteKeyLocs[i];
			if (flags[flagIdx]) {
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
		if (QwertyToggle) {
			//unsigned char u_array[5] = { (char16_t) u'\xE8', '"', '(', '-', (char16_t) u'è' };
			//blackKeyLabels = std::string(u_array, u_array + sizeof u_array / sizeof u_array[0]);
			blackKeyLabels =  "23567";
		} else {
			blackKeyLabels = 'é"(-è';
		}
		
		int blackKeyOffsets[] = { 1, 3, 6, 8, 10 }; // where black keys are placed
		int blackKeyOffsets_key[] = { 0, 1, 3, 4, 5 }; // where black keys are placed
		for (int i = 0; i < 5; ++i) {
			int flagIdx = blackKeyOffsets[i];
			int keyIdx = blackKeyOffsets_key[i];
			ofSetLineWidth(1);
			if (flags[flagIdx]) { 
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

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args){
	ofSetColor(115);
	gui.draw();
}

//--------------------------------------------------------------
float ofApp::keyFreq(int key, int LaFreq) {
	char charKey = static_cast<char>(key);
	auto it = current_map.find(charKey);
	if (it != current_map.end()) {
		int n = static_cast<int>(it->second);
		return LaFreq * std::pow(2.0, (n-49+4+((octave-1)*12)) / 12.0);
	}
	return 0.0f; // or some default
}

//--------------------------------------------------------------
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
	
	char charKey = static_cast<char>(key);
	if (current_map.find(charKey) != current_map.end()) {
		pan = 0.5;
		targetFrequency = keyFreq(key, LaFreq);
		for (int i = 0; i < 10; ++i) {
			phaseAdderTarget.push_back((mapHarms[i] * (targetFrequency * (i + 1) )/ (float)sampleRate) * glm::two_pi<float>());
		}
		setFlags(key, flags, true);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
	setFlags(key, flags, false);
	phaseAdderTarget[0] = 1;
	for (int i = 1; i < 10; ++i) {

		phaseAdderTarget[i] = 0;

	}
	// phaseAdderTarget = 0.;
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
std::vector<float> fourierSpectrum;

void ofApp::computeFourier(const std::vector<float> & signalwave) {
	int N = signalwave.size(); // number of samples

	std::vector<float> F1(N, 0.0f);
	std::vector<float> F2(N, 0.0f);
	std::vector<float> F(N, 0.0f); // Fourier magnitude

	for (int k = 0; k < N; k++) {
		F1[k] = 0.0f;
		F2[k] = 0.0f;

		for (int n = 0; n < N; n++) {
			float phi = (2.0f * PI * k * n) / N;
			F1[k] += signalwave[n] * cos(phi);
			F2[k] -= signalwave[n] * sin(phi);
		}

		// Fourier magnitude calculation
		F[k] = sqrt(F1[k] * F1[k] + F2[k] * F2[k]);
	}

	//Fourier magnitude result for all frequencies
	fourierSpectrum = F;
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
		phaseAdder = phaseAdderTarget[0]; //0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = buffer[i * buffer.getNumChannels()] = sample * (0.05 * volumeAudio) * leftScale;
			rAudio[i] = buffer[i * buffer.getNumChannels() + 1] = sample * (0.05 * volumeAudio) * rightScale;
		}
	}
	// Compute Fourier Transform for the sample
	computeFourier(lAudio);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
