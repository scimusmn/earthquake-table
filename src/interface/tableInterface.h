#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../shakeTable/shakeTable.h"
#include "../../../dallasEng/dallasEng.h"
#include "interfaceObjects.h"

//class to interface with the earthquake trajectories

enum tableMode {
	TABLE_SELECT, TABLE_OSC, TABLE_QUAKE
};

class instructionBox {
	ofImage o_rodInst;
	ofImage o_buildInst;
	ofImage q_rodInst;
	ofImage q_buildInst;
	ofButton rodSel;
	ofButton buildSel;
	dallasButton resetBut;
public:
	tableMode mode;
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

	sinTraj sinWave;
	dallasButton start;

	slidePack amp;
	slidePack freq;

	int slideWidth;

	double prevFreqPerc;
	double prevAmpPerc;

public:
	//instructionBox inst;
	void setup();
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	bool drag(int x, int y);
	void reset();
};

// class to contain all of the automated controls for the earthquakes

class autoMode {

	ofxDirList DIR;
	vector<shakeTraj> shakes;

public:
	shakeButs buttons;
	//instructionBox inst;
	void setup();
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
	double x,y,w,h;
	eqButton stop;

	///------ manual control
	manualMode man;

	//------- auto mode control
	autoMode quake;

	ofTimer homeTimer;
	ofTimer resetTimer;

	//------ mode select

	dallasButton qSelect;
	dallasButton oSelect;
public:

	tableInterface();
	~tableInterface();

	void setup();
	void update();
	void draw(int x, int y);
	bool clickDown(int x,int y);
	bool clickUp();
	void drag(int x, int y);
};
