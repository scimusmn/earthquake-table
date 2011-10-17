#pragma once

#include "ofMain.h"
#include "ofExtended.h"

#include "../shakeTrajectory/shakeTrajectory.h"
#include "../sinTrajectory/sinTrajectory.h"
#include "ofxDirList.h"

class motionTable {
protected:
	long pos;

	int canBPS;
	const char * canDevice;
	int canNodeID;

	KvaserCAN can;
	CanOpen canOpen;
	Amp amp;
	HomeConfig hcfg;
	const Error *err;
	CML::int16 homeCurrent;

	bool bIsRunning;
	bool bWasRunning;

	bool bHoming;
	bool bHomed;

	tableTraj * tRunning;
public:
	motionTable();
	~motionTable();

	bool isRunning();
	bool justStopped();

	void handleTrajectory(tableTraj & traj);
	void stopTable();
	void handleError(const Error *err, string str);
	void clearFaults();
	bool home();
	bool isHoming(){return bHoming;}
	bool isHome(){return bHomed;}

	void setup();
	void update();
};