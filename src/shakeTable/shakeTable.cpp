#include "shakeTable.h"
#include "../../../dallasEng/dallasEng.h"
#include "../eqConfig.h"

extern ofColor black,white,red,yellow;

static motionTable tbl;

motionTable & table(){
	return tbl;
}

motionTable::motionTable()
{
	pos=1;
	canBPS = 1000000;
	canDevice = "CAN0";
	canNodeID = 1;
	tRunning=0;
	bIsRunning=false;
	trajToHandle=0;

	amp.SetCountsPerUnit(1);

	bHomed=false;
	bHoming=false;
}
	
motionTable::~motionTable()
{

}

bool motionTable::isRunning()
{ 
	if(tRunning){
		bIsRunning=tRunning->isRunning();
	}
	else bIsRunning=false;
	return bIsRunning;
}

bool motionTable::justStopped()
{ 
	bool ret=0;
	if(bWasRunning&&!isRunning())
		ret=1;
	bWasRunning=bIsRunning;
	return ret;
}
	
void motionTable::handleTrajectory(tableTraj & traj)
{
	amp.GetPositionActual(currentPosition);
	if(abs(currentPosition)<2000){
		amp.Enable();
		bIsRunning=true;
		tRunning=&traj;
		amp.SendTrajectory(traj);
		trajToHandle=0;
	}
	else{
		trajToHandle=&traj;
		home();
	}
}

void motionTable::stopTable()
{
	if(bIsRunning){
		amp.Disable();
		bIsRunning=false;
		tRunning->Finish();

		tRunning=0;
	}
}
	
void motionTable::handleError(const Error *err, string str)
{
   if( err )
   {
      printf( "Error %s: %s\n", str.c_str(), err->toString() );
      //exit(1);
   }
}
	
void motionTable::clearFaults()
{
	amp.ClearFaults();
}
	
bool motionTable::home()
{
	stopTable();
	amp.Enable();
	bHoming=true;
	bool ret=false;
	clearFaults();
	err = amp.GoHome( hcfg );
	handleError( err, "Going home" );

	return ret;
}

	
void motionTable::setup()
{
	hcfg.method  = CHM_HARDSTOP_NEG;
	hcfg.velFast = 12000;
	hcfg.velSlow = 12000;
	hcfg.accel   = 10000;
	homeCurrent=cfg().homeCurrent;
	hcfg.current = homeCurrent; // 1.0 amps, enough to stop it once both blocks are against the stops. 
	hcfg.offset  = 30500;


	cml.SetDebugLevel( LOG_EVERYTHING );

	can.SetBaud( canBPS );
	
	err = canOpen.Open( can );
	handleError( err, "Opening CANopen network" );

	clearFaults();

	printf( "Doing init\n" );
	err = amp.Init( canOpen, canNodeID );
	handleError( err, "Initting amp" );

	home();
}

tableTraj & motionTable::runningTrajectory()
{ 
	if(tRunning)
		return *tRunning;
	return blank;
}

void motionTable::update()
{
	if(bHoming){
		if(amp.IsReferenced()){
			uunit motPos=0;
			amp.GetPositionActual(motPos);
			if(abs(motPos)<100){
				printf("Successfully Homed\n");
				pos=0;
				bHoming=false;
				bHomed=true;
				if(trajToHandle){
					handleTrajectory(*trajToHandle);
					trajToHandle=0;
				}
			}
		}
	}
	if(bIsRunning){
		amp.GetPositionActual(currentPosition);
		//cout << currentPosition << endl;
	}
}

void motionTable::drawForeground(){
	if(isHoming()){
		ofSetColor(black.opacity(.75));
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofSetColor(white);
		spinner.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/4);
		header().setMode(OF_FONT_CENTER);
		header().setSize(70);
		ofSetColor(white);
		header().drawString("Centering table, please wait",ofGetWidth()/2,ofGetHeight()/8);
		header().setSize(30);
		header().setMode(OF_FONT_LEFT);
	}
}