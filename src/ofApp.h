#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

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
		
		void audioOut(ofSoundBuffer & buffer);
		float keyFreq(int key,int baseFreq);
		void setFlags(int key, bool flags[],bool val);
		ofSoundStream soundStream;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;
		bool flags[12];
		bool flagsFreq[5];
		int baseFreq;
		enum class Key {
			C, // C natural
			D,
			E,
			F,
			G,
			A,
			B,
			CSharp, // C# (all sharps after naturals)
			DSharp, // D#
			FSharp, // F#
			GSharp, // G#
			ASharp, // A#
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
};
