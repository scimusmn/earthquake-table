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
	//if(!tableRunning) ofSetColor(255,255,255),ofRect(100,200,1000,200);
	//traj.draw(200,500);
	//table.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='z'){
		table.home();
	}
	
	if(key=='-') inter.freqDown(.02);
	if(key=='=') inter.freqUp(.02);
	if(key=='+') inter.dTimeDown(1);
	if(key=='_') inter.dTimeUp(1);
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
	testBut.clickUp();
	inter.clickUp();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


//--------------------------------------------------------------
void testApp::exit(){
	table.stopTable();
}