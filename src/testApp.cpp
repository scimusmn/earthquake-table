#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	table.setup();
	inter.setup(&table);
	background.loadImage("images/background.jpg");
}

//--------------------------------------------------------------
void testApp::update(){
	inter.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(255*.2,255*.2,255*.2);
	ofSetColor(0xffffff);
	background.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()/2,-ofGetHeight()/2);
	background.draw(ofGetWidth()/2,ofGetHeight()/2,-ofGetWidth()/2,-ofGetHeight()/2);
	background.draw(ofGetWidth()/2,ofGetHeight()/2,-ofGetWidth()/2,ofGetHeight()/2);
	background.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()/2,ofGetHeight()/2);
	bool tableRunning=false;
	inter.draw(100,100);
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
	table.stopTable();
}