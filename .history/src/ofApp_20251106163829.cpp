#include "ofApp.h"
#include <map>
#include <cmath>

//--------------------------------------------------------------
bool bcolor[10];
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
		bcolor[i] = false;
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
int octaveShift = 0; // 0 = normal, +12 = a octave high, -12 = below
void ofApp::draw(){

	//ofSetColor(225);
	ofSetColor(245, 58, 135);
	ofDrawBitmapString("ArTeK 808  Synthesizer", 32, 25);
	//ofDrawBitmapString("press 'x' to unpause the audio\npress 'c' to pause the audio", 31, 80);
	
	ofNoFill();
	// draw keys 
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 50, 0);
			
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
		ofTranslate(32, 170, 0);
			
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

	ofPushStyle();
		ofPushMatrix();
		ofTranslate(32, 290, 0);
			
		ofSetColor(225);
		ofDrawBitmapString("F(f)", 4, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 100);

		ofSetColor(245, 58, 135);
		ofSetLineWidth(3);
					
			ofBeginShape();
			//for (unsigned int i = 0; i < rAudio.size(); i++){
			//	float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
			//	ofVertex(x, 50 -rAudio[i]*180.0f);
		//	}
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
	if (bcolor[i]) ofSetColor(0, 0, 180);
    else ofSetColor(255);

    ofDrawRectangle(x, y, keyWidth, keyHeight);  // draw white key

    ofNoFill();
    ofSetColor(0);                               // black outline
    ofDrawRectangle(x, y, keyWidth, keyHeight);
    ofFill();
}
ofSetColor(0, 180, 255); // light blue color

ofDrawBitmapString("Note active: " + ofToString(targetFrequency, 2) + " Hz", 31, 480);
ofDrawBitmapString("Octave active: " + ofToString(octaveShift / 12), 500, 480);

ofSetColor(255);
ofDrawBitmapString("KeyBoard: Q to P notes C4 to E5", 32, 500);
ofDrawBitmapString(" press a to >> Octive! ", 500, 500);
ofDrawBitmapString(" press b to << Octive! ", 500, 520);



int blackKeyPattern[7] = {1,1,0,1,1,1,0}; // padrão relativo às 7 teclas brancas por oitava

float blackWidth = keyWidth * 0.6;   // largura da tecla preta
float blackHeight = keyHeight * 0.6; // altura da tecla preta

for(int i = 0; i < nKeys; i++){
    // só algumas teclas têm preta
    int patternIndex = i % 7; // ciclo de  teclas  ahhhhh
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

float getFrequencyFromKey(int pianoKeyNumber) {
    // piano varia de 1 (A0) a 88 (C8)
    return 440.0f * pow(2.0f, (pianoKeyNumber + octaveShift - 49) / 12.0f);
}

std::map<int,int> keyToIndex = {
    {'q',0}, {'w',1}, {'e',2}, {'r',3}, {'t',4},
    {'y',5}, {'u',6}, {'i',7}, {'o',8}, {'p',9}
};

std::map<int, int> keyToPiano = {
    {'q', 40}, // C4
    {'w', 42}, // D4
    {'e', 44}, // E4
    {'r', 45}, // F4
    {'t', 47}, // G4
    {'y', 49}, // A4
    {'u', 51}, // B4
    {'i', 52}, // C5   change it after for use black keys
    {'o', 54}, // D5
    {'p', 56}  // E5
};

//Fourier Transfor

void ofApp::computeFourier(const std::vector<float>& sinal) {
    int N = sample.size();  // number of samples
    
    std::vector<float> F1(N, 0.0f);  
    std::vector<float> F2(N, 0.0f);  
    std::vector<float> F(N, 0.0f);   // Fourier magnitude
    
    for (int k = 0; k < N; k++) {
        F1[k] = 0.0f;
        F2[k] = 0.0f;

        for (int n = 0; n < N; n++) {
            float phi = (2.0f * PI * k * n) / N;
            F1[k] += sinal[n] * cos(phi);
            F2[k] -= sinal[n] * sin(phi);
        }

        // magnitude do espectro
        F[k] = sqrt(F1[k] * F1[k] + F2[k] * F2[k]);
    }

    // guarda para desenhar depois
    fourierSpectrum = F;
}



//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	if(keyToIndex.find(key) != keyToIndex.end()){
        int index = keyToIndex[key];
        bcolor[index] = true;
        
		int pianoKey = keyToPiano[key];
        targetFrequency = getFrequencyFromKey(pianoKey);
        phaseAdderTarget = (targetFrequency / (float) sampleRate) * glm::two_pi<float>();    
    }

	if (key == 'b') { // Octave below
    octaveShift -= 12;
	} 
	else if (key == 'a') { // Octave above
    octaveShift += 12;
	}


	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = std::max(volume, 0.f);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = std::min(volume, 1.f);
	}
	/*
	if( key == 'x' ){
		soundStream.start();
	}
	
	if( key == 'c' ){
		soundStream.stop();
	}
	*/
	
}

//--------------------------------------------------------------

void ofApp::keyReleased(int key){
    if(keyToIndex.find(key) != keyToIndex.end()){
        int index = keyToIndex[key];
        bcolor[index] = false;
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
	bNoise = true;

}



//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bNoise = false;
	for (int i = 0; i < 10; i++) {
		bcolor[i] = false;
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
