#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLWindowSettings settings;
	settings.setSize(1100, 1100);
	settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	auto window = ofCreateWindow(settings);
	// auto window2 = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	// ofRunApp(window2, std::make_shared<ofApp>());
	ofRunMainLoop();

}
