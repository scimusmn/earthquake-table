#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "CML.h"

struct dataPoint {
	unsigned long dTime;
	double acc,vel,pos,time;
	dataPoint(double acc, double deltat);
	dataPoint(double a, double v, double p, double t);
};

class shakeData {
protected:
	vector<dataPoint> uData;
	string file;
	double maxDisp;

	ofImage shot;
	bool bCaptured;
public:
	double sampFreq;
	double maxNet;
	long index;
	double pos,vel;
	shakeData();
	~shakeData();
	void getPoint(double acc, double deltat);
	void loadSMC(string filename);
	void loadKNET(string filename);
	void loadCOSMOS(string filename);
	void loadDAT(string filename);
	void processData();
	void loadUnprocessed(string filename);
	void loadProcessed(string filename);
	void saveProcessed(string filename);
	void load(string filename);
	void changeMaxNet(double newMax);
	dataPoint & operator[](int i);
	int size();
	void draw(int x, int y, double w, double h);

	bool nextSegment( CML::uunit & p, CML::uunit & v, CML::uint8 & t);
	void reset();
};