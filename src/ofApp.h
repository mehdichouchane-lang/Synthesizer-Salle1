#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void setupGui();
		void drawGui(ofEventArgs & args);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void computeFourier(const vector<float> & input);
		vector<float> fourierSpectrum; 
		void audioOut(ofSoundBuffer & buffer);
		float keyFreq(int key, int baseFreq);
		void setFlags(int key, bool flags[],bool val);
		ofSoundStream soundStream;
		float scaleFourier;
		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;
		bool flags[12];
		bool flagsFreq[5];
		int baseFreq;
		ofTrueTypeFont titleFont;
		enum class Key {
			C, // C natural
			CSharp, // C# (all sharps after naturals)
			D,
			DSharp, // D#
			E,
			F,
			FSharp, // F#
			G,
			GSharp, // G#
			A,
			ASharp, // A#
			B,
			KeyCount // Helper to count number of keys
		};
		std::unordered_map<char, Key> qwerty_map;
		std::unordered_map<char, Key> french_map;
		std::unordered_map<char, Key> current_map;
		float widthWhiteKey;
		float keySpace ;
		float heightWhiteKey;
		float widthBlackKey;
		float heightBlackKey ;
		float xStart;
		float yStart;
		float qwertyButtonStartX;
		float qwertyButtonStartY;
		float qwertyButtonEndX;
		float qwertyButtonEndY;
		int counterQwerty;
		vector<float> lAudio;
		vector <float> rAudio;
		
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;
		ofParameterGroup parameters;
		ofParameter<int> octave;
		ofParameter<int> volumeAudio;
		ofParameter<int> LaFreq;
		// ofParameter<ofColor> color;
		ofxPanel gui;
		ofxToggle QwertyToggle;
		bool qwertyActive = false;
		void onQwertyToggled(bool & val);
};
