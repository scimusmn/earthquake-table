#include "shakeTrajectory.h"

extern ofColor yellow;
extern ofColor red;
extern ofColor blue;
extern ofColor gray;

shakeTraj::shakeTraj(){
	label.loadFont("fonts/Arial.ttf");
	bRunning=false;
}

void shakeTraj::Finish(){
	data.reset();
	bRunning=false;
}

const Error * shakeTraj::NextSegment(uunit & pos, uunit & vel, uint8 & time){
	if(!data.nextSegment(pos,vel,time)){
		time=pos=vel=0;
		return &TrjError::NoneAvailable;
	}
	pvts.push_back(pvtCont(pos/100,vel*10,(500./double(data.size()))*data.index));
	return 0;
}

void shakeTraj::draw(int x, int y, int w, int h)
{
	ofSetColor(255,255,255);
	ofRect(x,y,w,h);
	int n=data.size()/200;
	ofSetColor(109,202,208);
	for(int i=1; i<n;i++){
		ofLine(x+i*w/n,y,x+i*w/n,y+h);
	}
	n=4;
	for(int i=0; i<n;i++){
		ofLine(x,y+i*h/n,x+w,y+i*h/n);
	}
	ofSetColor(0,0,0);
	ofSetLineWidth(2);
	data.draw(x,y+h/2,w,h/2);
	//ofDisableSmoothing();
	//label.drawString(location,x,y-100);
	ofSetColor(255,0,0);
	ofSetLineWidth(3);
	ofLine((w/double(data.size()))*data.index+x,y,(w/double(data.size()))*data.index+x,y+h);
	ofSetLineWidth(1);
}

void shakeTraj::auxilliaryDraw(int x, int y, int w, int h,ofFont & lbl)
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
	/*string perc=ofToString(int(amplPercent*100))+"%";
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
	lbl.setMode(OF_FONT_TOP);*/
}

const Error * shakeTraj::StartNew(){
	pvts.clear();
	printf("Starting trajectory");
	bRunning=true;
	return 0;
}

void shakeTraj::setup(string filename){
	label.loadFont("fonts/Arial.ttf");
	vector<string> titleSpl = ofSplitString(filename,"()");
	if(titleSpl.size()>1){
		string file=titleSpl[1];
		titleSpl = ofSplitString(file,"_");
		if(titleSpl.size()>=3){
			location=titleSpl[0];
			date=titleSpl[1];
			magnitude=ofSplitString(titleSpl[2],"=")[1];
		}
	}
	cout << "Location: " << location << " Date: " << date << " Magnitude: " << magnitude << endl;
	data.loadUnprocessed(filename);
	maxNet=data.maxNet;
}

void shakeTraj::updateMaxNet(double newMax){
	data.changeMaxNet(newMax);
	maxNet=data.maxNet;
}

double shakeTraj::duration()
{
	cout << double(data.size())/200. <<endl;
	return float(data.size())/200.;
}

 bool shakeTraj::UseVelocityInfo(){
	 return false;
 }