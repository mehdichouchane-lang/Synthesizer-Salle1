#include "ofApp.h"
#include <map>
#include <cmath>

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
	ofDrawBitmapString("AUDIO OUTPUT EXAMPLE", 32, 25);
	ofDrawBitmapString("press 'x' to unpause the audio\npress 'c' to pause the audio", 31, 80);
	
	ofNoFill();
	// draw keys 
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 120, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Left Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 100);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < lAudio.size(); i++){
				float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
				ofVertex(x, 50 -lAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 300, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("Right Channel", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 100);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < rAudio.size(); i++){
				float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
				ofVertex(x, 50 -rAudio[i]*180.0f);
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
	ofDrawBitmapString(reportString, 32, 430);
// Drow piano keyboard

ofSetColor(255);                // white color for keys
int nKeys = 10;                  // nomber of keys
float keyWidth = 80;             // width keys
float keyHeight = 220;           // high keys

float xOffset = 100;              // deslocamento horizontal da borda esquerda
float yOffset = ofGetHeight() - keyHeight - 20; // 20px de margem inferior

for(int i = 0; i < nKeys; i++){
    float x = xOffset + i * keyWidth;
    float y = yOffset;

    ofFill(); 
	if (flags[i]) {
			ofSetColor(0, 0, 180);
		} else {
			ofSetColor(255);
		}


    ofDrawRectangle(x, y, keyWidth, keyHeight);  // draw white key

    ofNoFill();
    ofSetColor(0);                               // black outline
    ofDrawRectangle(x, y, keyWidth, keyHeight);
    ofFill();

    // reset to white for next key
    ofSetColor(255);
ofDrawBitmapString("Clavier: Q W E R T Y U I O P -> 10 notes (C4 à E5)", 32, 490);
ofDrawBitmapString("Note active: " + ofToString(targetFrequency, 2) + " Hz", 32, 510);
ofDrawBitmapString(" press a to >> Octive! ", 32, 520);
ofDrawBitmapString(" press b to  Octive! ", 32, 520);
ofDrawBitmapString("Note active: " + ofToString(targetFrequency, 2) + " Hz", 32, 510);

}

int blackKeyPattern[7] = {1,1,0,1,1,1,0}; // padrão relativo às 7 teclas brancas por oitava

float blackWidth = keyWidth * 0.6;   // largura da tecla preta
float blackHeight = keyHeight * 0.6; // altura da tecla preta

for(int i = 0; i < nKeys; i++){
    // só algumas teclas têm preta
    int patternIndex = i % 7; // ciclo de 7 teclas
    if(blackKeyPattern[patternIndex]){
        float bx = xOffset + i * keyWidth + keyWidth - blackWidth/2;
        float by = yOffset;

        ofSetColor(0);                 // preto
        ofDrawRectangle(bx, by, blackWidth, blackHeight);
    }
}
// título do piano

ofSetColor(0, 180, 255); // light blue color for title
ofDrawBitmapString("Piano", xOffset, yOffset - 10); // título logo acima do teclado

}
int octaveShift = 0; // 0 = normal, +12 = uma oitava acima, -12 = abaixo
float getFrequencyFromMidi(int midiNote) {
    return 440.0f * pow(2.0f, (midiNote + octaveShift - 69) / 12.0f);
}

std::map<int,int> keyToIndex = {
    {'q',0}, {'w',1}, {'e',2}, {'r',3}, {'t',4},
    {'y',5}, {'u',6}, {'i',7}, {'o',8}, {'p',9}
};

std::map<int,int> noteMapMidi = {
    {'q',60}, {'w',62}, {'e',64}, {'r',65}, {'t',67},
    {'y',69}, {'u',71}, {'i',72}, {'o',74}, {'p',76}
};


void ofApp::setFlags(int key, bool flags[],bool val) {
	std::unordered_map<char, int> qwerty_map = {
		{'q', 60}, // C4
    	{'w', 62}, // D4
    	{'e', 64}, // E4
    	{'r', 65}, // F4
    	{'t', 67}, // G4
    	{'y', 69}, // A4
    	{'u', 71}, // B4
    	{'i', 72}, // C5
    	{'o', 74}, // D5
    	{'p', 76}  // E5
	};
	auto it = qwerty_map.find(key);
	if (it != qwerty_map.end()) {
		int index = it->second; // index from map (1-based)
		flags[index - 1] = val; // set to false, adjusting to 0-based array index
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if(keyToIndex.find(key) != keyToIndex.end()){
        int index = keyToIndex[key];
        flags[index] = true;

        int midiNote = noteMapMidi[key];
        targetFrequency = getFrequencyFromMidi(midiNote);
        phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();
        bNoise = false;
    }

	if (key == 'b') { // Octive below
    octaveShift -= 12;
    ofLogNotice() << "Octive: " << (octaveShift / 12);
	} 
	else if (key == 'a') { // Octive above
    octaveShift += 12;
    ofLogNotice() << "Octive: " << (octaveShift / 12);
	}


	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = std::min(volume, 1.f);
	}
	
	if( key == 'x' ){
		soundStream.start();
	}
	
	if( key == 'c' ){
		soundStream.stop();
	}
	
	
}

//--------------------------------------------------------------

void ofApp::keyReleased(int key){
    if(keyToIndex.find(key) != keyToIndex.end()){
        int index = keyToIndex[key];
        flags[index] = false;
        phaseAdderTarget = 0.;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	//float height = (float)ofGetHeight();
	//float heightPct = ((height-y) / height);
	//targetFrequency = 2000.0f * heightPct;
	//phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();
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
			//float sample = sin(phase);
			float sample = sin(phase) + 0.5f * sin(phase*2) + 0.25f * sin(phase*4);
            //float sample = (phase < PI ? 1.0 : -1.0); // square
			//float sample = fabs(fmod(phase / PI, 2.0) - 1.0) * 2 - 1; // triangle

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
