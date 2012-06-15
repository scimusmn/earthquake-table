#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "../tableTrajectory.h"
#include "../sinTrajectory/sinTrajectory.h"
#include "../accParse/accParse.h"

CML_NAMESPACE_USE();

class shakeTraj : public tableTraj{
	unsigned long lastTime, tDuration;
	long dTime;
	unsigned long startTime;
	shakeData data;
	ofFont label;
	vector<pvtCont> pvts;
public:
	double maxNet;
	string location, date, magnitude;
	double samplesPerSecond;
	shakeTraj();
	void Finish();
	const Error * NextSegment(uunit & pos, uunit & vel, uint8 & time);
	const Error * StartNew();
	void setup(string filename);
	void draw(int x, int y,int w=500,int h=200);
	void auxilliaryDraw(int x, int y, int w, int h,ofFont & lbl);
	void updateMaxNet(double newMax);

	double duration();

	bool UseVelocityInfo();
};