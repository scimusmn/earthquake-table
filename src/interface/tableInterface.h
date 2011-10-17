#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../shakeTable/shakeTable.h"
#include "interfaceObjects.h"

//class to interface with the earthquake trajectories

class instructionBox {
	ofImage rodInst;
	ofImage buildInst;
	ofButton rodSel;
	ofButton buildSel;
	eqButton resetBut;
public:
	void setup();
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	void reset();
	int getState();
};

// class to hold all of the interface objects for the table

class tableInterface {
protected:
	ofxDirList DIR;
	sinTraj sinWave;
	vector<shakeTraj> shakes;
	shakeButs buttons;
	sinButton sine;
	motionTable * table;
	eqButton stop;

	///------ manual control

	ofSlider slide;
	ofSlider amp;

	ofButton ampUp;
	ofButton ampDown;

	ofButton freqBUp;
	ofButton freqBDown;

	ofTimer homeTimer;

	digitDisplay ampDisp;
	digitDisplay readout;

	double prevFreqPerc;
	double prevAmpPerc;

	instructionBox inst;

	//------ mode select

	eqButton qSelect;
	eqButton oSelect;

	bool resetting;

	ofFont label;
	ofFont subtext;

	bool bWaving,bQuaking;
public:

	tableInterface();
	~tableInterface();

	void setup(motionTable * tbl);
	void update();
	void draw(int x, int y);

	void drawGroupBox(int x, int y, int w, int h,string title, bool gray);

	void drawFreq(int x, int y, int w, int h);
	void drawQuakes(int x, int y, int w, int h);
	void freqUp(double amt);
	void freqDown(double amt);
	void dTimeUp(double amt);
	void dTimeDown(double amt);
	bool clickDown(int x,int y);
	bool clickUp();
	void drag(int x, int y);
};
