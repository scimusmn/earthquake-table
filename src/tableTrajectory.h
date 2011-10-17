#pragma once

#include "ofMain.h"
#include "ofExtended.h"
#include "CML.h"
#include "can\can_kvaser.h"

CML_NAMESPACE_USE();

class tableTraj : public Trajectory {
protected:
	bool bRunning;
public:
	bool isRunning(){ return bRunning; }
};