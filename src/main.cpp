#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"


//========================================================================
int main( ){

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setSize(1000, 900);
	settings.setPosition(glm::vec2(305,0));
	settings.resizable = true;
	auto mainWindow = ofCreateWindow(settings);

	settings.setSize(300, 100);
	settings.setPosition(glm::vec2(0,0));
	settings.resizable = false;
	// uncomment next line to share main's OpenGL resources with gui
	//settings.shareContextWith = mainWindow;
	auto guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);

	auto mainApp = std::make_shared<ofApp>();
	mainApp->setupGui();
	ofAddListener(guiWindow->events().draw,mainApp.get(),&ofApp::drawGui);

	ofRunApp(mainWindow, mainApp);
	// ofRunApp(window2, std::make_shared<ofApp>());
	ofRunMainLoop();

}

