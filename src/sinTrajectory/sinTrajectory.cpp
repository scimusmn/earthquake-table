#include "sinTrajectory.h"

//-- Init the starting variables for the sin Trajectory

extern ofColor yellow;
extern ofColor red;
extern ofColor blue;
extern ofColor gray;

sinTraj::sinTraj(){
	index=tickPos=dTime=totTime=startTime=freq=pFreq=ampl=aug=0;
	bRunning=false;
	seconds=5;
	numDivs=seconds;
	numPoints=500;
	pvts.reserve(numPoints+1);
	//pts.reserve(numPoints+1);
	amplPercent=1;
}

// Define the function which the table calls once it has finished running (which happens when next segment returns 0 for the time)

void sinTraj::Finish(){
	bRunning=false;
	pvts.clear();
	cout << "I quit!\n";
}

// define the NextSegment function, which is called by the table when it needs additional points.

const Error * sinTraj::NextSegment(uunit & pos, uunit & vel, uint8 & time){

	//if the sinTraj is not running return 0 for time pos and vel, and throw an error
	if(!bRunning){
		time=pos=vel=0;
		return &TrjError::NoneAvailable;
	}
	//if the traj is running
	else{
		//dTime=20/freq;
		//Set the timespan for the next event
		time=dTime;
		totTime+=dTime; // increment the current time
		//set the amplitude according to the current frequency
		double scale=amplPercent/(freq*1.6);
		double amp=ampl*scale;
		double ind=float(totTime)/1000.; //derive an index position from the current run time
		
		//if the previous frequency doesn't equal the current freq
		if(pFreq!=freq){
			double ind2=ind;
			//change the augment value to line up the previous sin wave with the current
			aug=2*M_PI*(freq-pFreq)*ind2+aug;
			pFreq=freq; // set the previous freq to the current freq
		}

		ind*=2*M_PI*freq; // multiply the index time by 2*pi*freq
		double sind=sin(ind-aug);
		pos=sind*amp;  // set teh position equal to the sin of the index minus the augmentation value
		pt.set(pos,vel,time);

		/*pts.push_front(ofPoint(ofGetElapsedTimef(),y+h/2+sind*h/2));
		while(pts.size()&&pts[pts.size()-1].x+seconds<ofGetElapsedTimef()) pts.pop_back();*/
	}
	return 0;
}

// draw function for the points which have been plotted.

void sinTraj::draw(int _x, int _y, int _w, int _h)
{
	x=_x,y=_y;
	w=_w,h=_h;
	ofSetLineWidth(1.0);
	int fullAmp=h*(.8)/(freq*1.6);
	ofSetColor(109,202,208);
	for(int i=0; i<numDivs; i++){
		ofLine(tickPos+i*w/seconds+x,y,tickPos+i*w/seconds+x,y+h);
	}
	int ampPos=(1-(.8)/(freq*1.6))*h*.5;
	for(int i=0; i<5;i++){
		if((i>0&&i<4)||ampPos) ofLine(x,y+ampPos+i*fullAmp/4,x+w,y+ampPos+i*fullAmp/4);
	}
	ofSetColor(0,0,0);
	//ofSetLineWidth(2.0);
	/*for(unsigned int i=0; i<pts.size()-1&&pts.size()>1; i++){
		ofLine(x+w-(w/seconds)*(ofGetElapsedTimef()-pts[i].x),pts[i].y,x+w-(w/seconds)*(ofGetElapsedTimef()-pts[i+1].x),pts[i+1].y);
	}*/

	for(unsigned int i=0; i<pts.size()-1&&pts.size()>1; i++){
		ofLine(pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y);
	}
	
	//ofSetLineWidth(1);
}

void sinTraj::auxilliaryDraw(int x, int y, int w, int h,ofFont & lbl)
{
	ofSetColor(yellow);
	double factor=double(h)/lbl.stringWidth("AMPLITUDE");
	double stringW=factor*lbl.stringHeight("AMPLITUDE");
	ofPushMatrix();
	ofTranslate(x-stringW,y+h);
	ofScale(factor,factor);
	ofRotate(-90);
	lbl.drawString("AMPLITUDE",0,0);
	ofPopMatrix();
	string perc=ofToString(int(amplPercent*100))+"%";
	factor=double(w-stringW-20)/lbl.stringWidth(perc);
	lbl.setMode(OF_FONT_BOT);
	ofPushMatrix();
	int ampPos=(1-amplPercent*(.8)/(freq*1.6))*h*.5;
	ofTranslate(x+stringW,y+ampPos-5);
	ofScale(factor,factor);
	lbl.drawString(perc,0,0);
	ofPopMatrix();
	ofSetColor(blue);
	ofSetLineWidth(2.);
	ofLine(x+stringW,y+ampPos,x+w-3,y+ampPos);
	ofLine(x+stringW,y+h-ampPos,x+w-3,y+h-ampPos);
	ofSetLineWidth(2.);
	lbl.setMode(OF_FONT_TOP);
}

// Define the function which is called by the table when the trajectory is started.

const Error * sinTraj::StartNew(){
	startTime=ofGetElapsedTimeMillis();
	pts.clear();
	totTime=0;
	aug=0;
	printf("Starting trajectory");
	bRunning=true;
	return 0;
}

// function used to set teh frequency values for the sinwave

void sinTraj::setup(double minFrequency,double amplitude, double numSeconds){
	seconds=numSeconds;
	numDivs=seconds;
	freq=pFreq=minFrequency;
	dTime=4;
	ampl=amplitude;
	sampleRate=10;
	maxAmp=amplitude/(minFrequency*1.6);
}

// shouldn't really be used, but allows one to change the time between points

void sinTraj::changeDTime(int chngTme)
{
	dTime=chngTme;
}

// used by the table to ascertain whether or not it shoudl generate it's own velocity data.

 bool sinTraj::UseVelocityInfo(){
	 return false;
 }

 double timeMove=0;
 double lastTime=0;

void sinTraj::update(){
	if(isRunning()){
		if(timeMove<ofGetElapsedTimef()){
			double secondPos=((ofGetElapsedTimeMillis()-startTime)%1000)/1000.;
			double delay=.005;
			double ti=(ofGetElapsedTimef()-(timeMove-delay));
			timeMove=ofGetElapsedTimef()+delay;
			double yPos=pt.pos/maxAmp*h/2+y+h/2;
			//if(!index){
			pts.push_front(ofPoint(x+w,yPos));
			//}
			while(pts.size()&&pts[pts.size()-1].x<x) pts.pop_back();
			for(unsigned int i=0; i<pts.size(); i++){
				//if(abs(pts[i].y-pts[i+1].y)>7) pts[i].y=(pts[i].y+pts[i+1].y)/2;
				pts[i].x-=ti*(w/seconds);
			}
			tickPos-=ti*(w/seconds);
			if(tickPos<=0) tickPos+=w/numDivs;
			//index++;
			//if(index==4) index=0;
		}
		/*if(timeMove<ofGetElapsedTimef()){
			timeMove=ofGetElapsedTimef()+1/sampleRate;
			wave.push_front(waveform(amplPercent,ofGetElapsedTimef(),freq,aug));
			while(wave.size()>=seconds*sampleRate) wave.pop_back();
			index-=(1/sampleRate)*(w/seconds);
			if(index<=0) index+=w/numDivs;
			double prevF=0;
			double augs=0;
			//if(wave.size()) augs=wave[0].aug;
			pts.clear();
			for(unsigned int i=0; i<wave.size(); i++){
				double d=(w/seconds)/sampleRate;
				for(double j=d*i; j<d*(i+1); j+=4){
					double indp=j/(w/seconds)+double((ofGetElapsedTimeMillis()-startTime)%1000)/1000./wave[i].freq;
					if(prevF!=wave[i].freq){
						double ind2=indp;
						//change the augment value to line up the previous sin wave with the current
						augs=2*M_PI*(wave[i].freq-prevF)*ind2+augs;
						prevF=wave[i].freq; // set the previous freq to the current freq
					}
					indp*=2*M_PI*wave[i].freq;
					int pos=sin(indp)*h/2*wave[i].amp;
					if(!j) cout << pos << " " << double((ofGetElapsedTimeMillis()-startTime)%1000)/1000. << endl;
					pts.push_front(ofPoint(x+j,y+h/2+pos));
					//if(pts.size()&&pts[pts.size()-1].x<x) pts.pop_back();
				}
			}
			lastTime=ofGetElapsedTimef();
		}*/
		/*if(timeMove<ofGetElapsedTimef()){
			timeMove=ofGetElapsedTimef()+1/sampleRate;
			double ind=double((ofGetElapsedTimeMillis()-int(1000/sampleRate)-startTime)%1000)/1000.;
			for(double i=0; i<(w/seconds)/sampleRate; i+=2){
				double ind2=ind+i/(w/seconds);
				ind2*=2*M_PI*freq; // multiply the index time by 2*pi*freq
				double pos=sin(ind2-aug)*h/2;
				pts.push_front(ofPoint(x+w-(w/seconds)/sampleRate+i,y+h/2+pos));
				if(pts.size()&&pts[pts.size()-1].x<x) pts.pop_back();
			}
		}*/
	}
 }