#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../../dallasEng/dallasEng.h"

struct configuration {
	string eqDir; // directory vars
	double timeout;
	bool verbose;
	int homeCurrent;
	bool scaleToAll;
	bool smoothing;
	bool noTable;
	bool autoTrim;
	int maxDisp;
	double sampleTime;
	void readGeneralConfig(string cfgFile);
	configuration(){
		verbose=false;
		noTable=true;
		ofEnableDataPath();
	}
	void setup(){
		readGeneralConfig("config.txt");
	}
};

configuration & cfg();