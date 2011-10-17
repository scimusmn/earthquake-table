#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "shakeTable.h"

//class to interface with the earthquake trajectories

class shakeButton : public ofButton {
public:
	double nameWid,dateWid;
	shakeTraj * shake;
	void setup(shakeTraj * shake);
	void draw(int _x, int _y);
	friend class shakeButs;
};

//class to interface with the sin trajectories

class sinButton : public ofButton {
public:
	sinTraj * sine;
	sinButton(){sine=0;}
	void setup(sinTraj * sin);
	void draw(int _x, int _y);
};

//class for the group of earthquake buttons

class shakeButs : public ofInterGroup {
	vector<shakeTraj> * shakes;
	vector<shakeButton> buttons;
	int lastPress;
public:
	shakeButton & operator[](int i);
	int size();
	shakeButton & lastPressed();
	void setup(vector<shakeTraj> * trajs);
	void draw(int _x, int _y);
	bool clickDown(int _x, int _y);
	bool clickUp();
};

//button for stopping the table

class eqButton :public ofButton {
public:
	void draw(int _x, int _y);
};

class instructionBox {
	ofImage rodInst;
	ofImage buildInst;
	ofButton rodSel;
	ofButton buildSel;
public:
	void setup();
	void draw(int x, int y, int w, int h);
	void update();
	bool clickDown(int x, int y);
	bool clickUp();
	void reset();
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

	ofTimer qTimer;
	ofTimer oTimer;

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
