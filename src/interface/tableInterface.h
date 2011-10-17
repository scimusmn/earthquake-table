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
	void setup(string folder);
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	void reset();
	int getState();
};

// class to contain all of the manual mode controls

class manualMode {
	motionTable * table;
	ofFont * fnt;
	ofFont * lblFnt;

	sinTraj sinWave;
	sinButton sine;

	slidePack amp;
	slidePack freq;

	double prevFreqPerc;
	double prevAmpPerc;

public:
	instructionBox inst;
	void setup(motionTable * table,ofFont * fnt1, ofFont * fnt2);
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	bool drag(int x, int y);
	void reset();
};

// class to contain all of the automated controls for the earthquakes

class autoMode {
	motionTable * table;
	ofFont * fnt;
	ofFont * lblFnt;

	ofxDirList DIR;
	vector<shakeTraj> shakes;
	shakeButs buttons;

	double prevFreqPerc;
	double prevAmpPerc;

public:
	instructionBox inst;
	void setup(motionTable * table,ofFont * fnt1, ofFont * fnt2);
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	bool drag(int x, int y);
	void reset();
};

// class to hold all of the interface objects for the table

class tableInterface {
protected:
	motionTable * table;
	eqButton stop;

	///------ manual control
	manualMode man;

	//------- auto mode control
	autoMode quake;

	ofTimer homeTimer;

	//------ mode select

	eqButton qSelect;
	eqButton oSelect;

	ofProgressSpinner homing;

	ofFont label;
	ofFont subtext;
public:

	tableInterface();
	~tableInterface();

	void setup(motionTable * tbl);
	void update();
	void draw(int x, int y);
	bool clickDown(int x,int y);
	bool clickUp();
	void drag(int x, int y);
};
