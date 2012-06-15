#include "testApp.h"
#include "../../dallasEng/dallasEng.h"
#include "eqConfig.h"

extern ofColor white,black,gray;

string MAIN_TITLE="See how your structure responds to motion";
//--------------------------------------------------------------
void testApp::setup(){
	cfg().setup();

	header().loadFont("fonts/DinC.ttf");
	header().setSize(20);
	header().setMode(OF_FONT_TOP);
	ttlBar.loadFont("fonts/DinC.ttf", 35);
	if(!cfg().noTable)
		table().setup();
	inter.setup();
	background.loadImage("images/background.jpg");
}

//--------------------------------------------------------------
void testApp::update(){
	inter.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255*.2,255*.2,255*.2);
	ofSetColor(white*.1);
	drawHatching(0,0,ofGetWidth(),ofGetHeight(),50,50);
	bool tableRunning=false;
	inter.draw(0,ttlBar.h);
	ttlBar.draw(MAIN_TITLE,0,0);

	table().drawForeground();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	inter.drag(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(inter.clickDown(x,y));
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	inter.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
	table().stopTable();
}