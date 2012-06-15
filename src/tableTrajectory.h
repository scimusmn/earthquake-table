#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "CML.h"
#include "can\can_kvaser.h"
#include "ofxThread.h"

CML_NAMESPACE_USE();

class tableTraj : public Trajectory, public ofxThread {
protected:
	bool bRunning;
public:
	bool isRunning(){ return bRunning; }
	virtual void draw(int x, int y, int w, int h){};
	virtual void auxilliaryDraw(int x, int y, int w, int h,ofFont & lbl){}
	//virtual void threadedFunction(){}
};