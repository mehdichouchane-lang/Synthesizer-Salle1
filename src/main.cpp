#include "ofApp.h"
#include "ofAppGLFWWindow.h"
#include "ofMain.h"

int main() {

	//Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	ofGLFWWindowSettings settings;
	settings.setSize(1000, 900);
	settings.setPosition(glm::vec2(300, 50));
	settings.resizable = true;
	auto mainWindow = ofCreateWindow(settings);

	settings.setSize(250, 150);
	settings.setPosition(glm::vec2(50, 50));
	settings.resizable = true;
	// uncomment next line to share main's OpenGL resources with gui
	//settings.shareContextWith = mainWindow;
	auto guiWindow = ofCreateWindow(settings);
	guiWindow->setVerticalSync(false);

	auto mainApp = std::make_shared<ofApp>();
	mainApp->setupGui();
	ofAddListener(guiWindow->events().draw, mainApp.get(), &ofApp::drawGui);
	
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
