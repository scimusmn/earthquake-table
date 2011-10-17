#include "shakeTable.h"

motionTable::motionTable()
{
	pos=1;
	canBPS = 1000000;
	canDevice = "CAN0";
	canNodeID = 1;
	tRunning=0;
	bIsRunning=false;

	homeCurrent=350;
	hcfg.method  = CHM_HARDSTOP_NEG;
	hcfg.velFast = 12000;
	hcfg.velSlow = 12000;
	hcfg.accel   = 10000;
	hcfg.current = 300; // 2.0 amps, enough to stop it once both blocks are against the stops. 
	hcfg.offset  = 30500;

	amp.SetCountsPerUnit(1);
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
	amp.Enable();
	bIsRunning=true;
	tRunning=&traj;
	amp.SendTrajectory(traj);
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
	bool ret=false;
	err = amp.GoHome( hcfg );
	handleError( err, "Going home" );

	//ofGetAppPtr()->draw();
	printf( "Waiting for home to finish...\n" );
	err = amp.WaitMoveDone( 20000 ); 
	handleError( err, "waiting on home" );

	if(amp.IsReferenced()){
		ret=true;
		printf("Successfully Homed\n");
		pos=0;
		uunit motPos=0;
		amp.GetPositionActual(motPos);
		cout << motPos << " is the current pos" << endl;
	}
	stopTable();
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

void motionTable::update()
{

}