#include "ofMain.h"
#include "ofExtended.h"
#include "../shakeTable/shakeTable.h"

#pragma once

class shakeButton : public ofButton {
public:
	double duration;
	double nameWid,dateWid,magWid,durWid;
	string dur;
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
	double nameWid,dateWid,durWid;
public:
	shakeButton & operator[](int i);
	int size();
	shakeButton & lastPressed();
	void setup(vector<shakeTraj> * trajs);
	void draw(int _x, int _y, int _w, int _h);
	bool clickDown(int _x, int _y);
	bool clickUp();
};

struct slidePack {
	ofButton up;
	ofButton dn;
	ofSlider sld;
	digitDisplay disp;
	void setup(int touchRad=40){
		sld.setup(touchRad,touchRad,0);
		disp.setup(150,4);
		up.setup(touchRad,touchRad,"images/plus.png");
		dn.setup(touchRad,touchRad,"images/minus.png");
	}
	bool clickUp(){
		up.clickUp();
		dn.clickUp();
		sld.clickUp();
		return false;
	}
};

//button for stopping the table

class eqButton :public ofButton {
public:
	void draw(int _x, int _y);
};