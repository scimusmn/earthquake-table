#pragma once

#include "../tableTrajectory.h"

CML_NAMESPACE_USE();

struct pvtCont {
	double pos;
	double vel;
	double time;
	bool newPoint;
	pvtCont(){ pos=vel=time=newPoint=0;}
	pvtCont(double p, double v, double t){
		pos=p, vel=v, time=t;
		newPoint=false;
	}
	void set(double p, double v, double t){
		pos=p, vel=v, time=t;
		newPoint=true;
	}
};

struct waveform {
	double amp;
	double time;
	double freq;
	double aug;
	waveform(double a, double t, double f, double au){
		amp=a;
		time=t;
		freq=f;
		aug=au;
	}
	void set(double a, double t, double f, double au){
		amp=a;
		time=t;
		freq=f;
		aug=au;
	}
};

class sinTraj : public tableTraj {
protected:
	unsigned long lastTime, tDuration;
	long totTime;
	unsigned long startTime;
	long index;
	double tickPos;
	vector<pvtCont> pvts;
	deque<ofPoint> pts;
	deque<waveform> wave;
	double seconds;
	bool change;
	//double posi,velo,tim;
	pvtCont pt;
	double maxAmp;
	double numPoints;
	double sampleRate;
	double numDivs;
	double x,y,w,h;
public:
	long dTime;
	sinTraj();
	double freq,pFreq, ampl, aug,amplPercent;
	void Finish();
	const Error * NextSegment(uunit & pos, uunit & vel, uint8 & time);
	const Error * StartNew();
	void setup(double minFrequency,double amplitude,double numSeconds);
	void draw(int x, int y, int w=500, int h=200);
	void update();
	void changeDTime(int chngTme);
	 bool UseVelocityInfo();
};