#include "shakeTable.h"

motionTable::motionTable()
{
	pos=1;
	canBPS = 1000000;
	canDevice = "CAN0";
	canNodeID = 1;
	tRunning=0;
	bIsRunning=false;

	homeCurrent=300;
	hcfg.method  = CHM_HARDSTOP_NEG;
	hcfg.velFast = 12000;
	hcfg.velSlow = 12000;
	hcfg.accel   = 10000;
	hcfg.current = homeCurrent; // 3.0 amps, enough to stop it once both blocks are against the stops. 
	hcfg.offset  = 30500;

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
			cout << motPos << " is the current pos" << endl;
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
		cout << currentPosition << endl;
	}
}