#include "tableInterface.h"

//--------------------- instruction box -----------

extern ofColor white;
extern ofColor black;
extern ofColor gray;
extern ofColor yellow;
extern ofColor blue;
extern ofColor orange;
extern ofColor red;

void instructionBox::setup(string folder)
{
	rodInst.loadImage("images/"+folder+"/rods.jpg");
	buildInst.loadImage("images/"+folder+"/build.jpg");
	rodSel.setup(300,300,"images/"+folder+"/rodBut.jpg");
	buildSel.setup(300,300,"images/"+folder+"/buildBut.jpg");
	resetBut.setup("Return to experiment selection","fonts/Arial.ttf",15);
	resetBut.arial.setMode(OF_FONT_TOP);
}
	
void instructionBox::draw(int x, int y, int w, int h)
{
	if(!rodSel.pressed()&&!buildSel.pressed()){
		rodSel.draw(x,y+(h-rodSel.h)/2);
		buildSel.draw(x+w-buildSel.w,y+(h-buildSel.h)/2);
	}
	else{
		ofSetColor(white);
		if(buildSel.pressed()) buildInst.draw(x,y,w,h);
		if(rodSel.pressed()) rodInst.draw(x,y,w,h);
		ofSetColor(yellow);
		resetBut.draw(x+w-resetBut.w-20,y+h-resetBut.h-20);
	}
}
	
void instructionBox::update()
{

}
	
bool instructionBox::clickDown(int x, int y)
{
	bool ret=0;
	if(!rodSel.pressed()&&!buildSel.pressed()){
		ret=1;
		rodSel.clickDown(x,y);
		buildSel.clickDown(x,y);
	}
	else if(resetBut.clickDown(x,y)){
		ret=1;
		reset();
	}
	return ret;
}
	
bool instructionBox::clickUp()
{
	return resetBut.clickUp();
}
	
void instructionBox::reset()
{
	rodSel.setPressed(false);
	buildSel.setPressed(false);
}

int instructionBox::getState()
{
	int ret=0;
	if(rodSel.pressed()) ret=1;
	if(buildSel.pressed()) ret=2;
	return ret;
}

//-_-_-_-_-_-_-_-_-_-_ Draw function fo the white box around each group -_-_-_-_-_-_-_-_-_-_

void drawGroupBox(int x, int y, int w, int h, string title,ofFont & label)
{
	ofColor k=ofGetStyle().color;

	int strWid=label.stringHeight(title)*1.5;
	 

	//------- white border box;
	ofNoFill();
	ofSetLineWidth(2.0);
	ofSetColor(white.opacity(k.a/255.));
	ofRect(x+strWid,y,w-strWid,h);
	ofBeginShape();
	ofVertex(x+strWid,y+h);
	ofVertex(x,y+h);
	ofVertex(x,y+strWid);
	ofVertex(x+strWid,y);
	ofEndShape();
	ofSetLineWidth(1);
	ofFill();


	//------- rotated label for group
	ofSetColor(yellow.opacity(k.a/255.));
	ofPushMatrix();
	ofTranslate(x+strWid/8,y+h-20);
	ofRotate(-90);
	label.drawString(title,0,0);
	ofPopMatrix();
}

//_-_-_-_-_-_-_-_-_-_-_ manual controls -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void drawSlideGroup(int x, int y, int w, int h,string label, slidePack & t, ofFont & fnt){

	//-_-_-_-_-_ draw title -_-_-_-_-_-_
	ofSetColor(white);
	fnt.drawString(label,x+w/2,t.disp.y-40);
	ofSetColor(black);
	ofPushStyle();
	ofSetLineWidth(2);
	ofLine(x+10,t.disp.y-20,x+w-10,t.disp.y-20);
	ofPopStyle();

	//-_-_-_-_-_ slider draw -_-_-_-_-_-_
	ofSetColor(blue);
	t.sld.draw(x+(w-t.sld.w)/2-100,y,400,15);

	//-_-_-_-_-_ button draw -_-_-_-_-_-_
	ofSetColor(white);
	t.dn.draw(t.sld.x-t.dn.w-30,t.sld.y+(t.sld.h-t.dn.h)/2);
	t.up.draw(t.sld.x+t.sld.w+30,t.sld.y+(t.sld.h-t.up.h)/2);
}

void drawDisp(slidePack & t,string contents,string add, ofFont & fnt){
	ofRoundShadow(t.disp.x-10,t.disp.y-10,t.disp.w+20,t.disp.h+20,5,1);
	ofSetColor(blue);
	t.disp.draw(contents,t.up.x+t.up.w+20,t.sld.y+(t.sld.h-t.disp.h)/2);

	//-_-_-_-_-_ draw addendum -_-_-_-_-_-_
	fnt.setSize(25);
	fnt.drawString(add,t.disp.x+t.disp.w+30,t.disp.y+t.disp.h);
	fnt.setSize(15);
}

void manualMode::setup(motionTable * tbl,ofFont * fnt1, ofFont * fnt2)
{
	amp.setup();
	freq.setup();
	sinWave.setup(.5,10000,5);
	prevFreqPerc=0;
	prevAmpPerc=1;
	fnt=fnt1;
	lblFnt=fnt2;
	table=tbl;

	sine.setup(&sinWave);
	amp.sld.w=400;
	amp.sld.setPercent(sinWave.amplPercent);
	inst.setup("manual");
}
	
void manualMode::draw(int x, int y, int w, int h)
{
	lblFnt->setSize(24);
	double sideLabelW=lblFnt->stringHeight("Kjg")*1.5;
	double boxW=w-sideLabelW;
	double boxX=x+sideLabelW;

	//ofSetColor(64,128,200);

	ofPoint ctrlPt(boxX,y+100);

	//-_-_-_-_-_ draw transparent black bg -_-_-_-_-_-_
	ofSetColor(black.opacity(.25));
	ofRect(ctrlPt.x+10,ctrlPt.y,boxW-20,h-150);
	ofPushStyle();
	ofNoFill();
	ofSetColor(black);
	ofSetLineWidth(2);
	ofRect(ctrlPt.x+10,ctrlPt.y,boxW-20,h-150);
	ofPopStyle();

	ofSetColor(yellow);
	sine.draw(ctrlPt.x+(boxW-sine.w)/2,ctrlPt.y+25);

	//-_-_-_-_-_ draw headline -_-_-_-_-_-_
	string title="FIND THE RESONANCE\n OF YOUR BUILDING";
	ofSetColor(yellow);
	lblFnt->setSize(24);
	int wid=lblFnt->stringWidth(title);
	lblFnt->drawString(title,x+sideLabelW+10,y+10);
	lblFnt->setSize(30);

	//-_-_-_-_-_ draw black line under headline -_-_-_-_-_-_
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofLine(x+sideLabelW+10,y+10+lblFnt->stringHeight(title),x+w-10,y+10+lblFnt->stringHeight(title));
	ofSetLineWidth(1.0);

	//-_-_-_-_-_ draw frequency slider -_-_-_-_-_-_
	string freqLabel="Frequency slider";
	int nFreq=sinWave.freq;
	drawDisp(freq,ssprintf("%02i.%i",nFreq,int((sinWave.freq+.05)*10-nFreq*10)),"hz",*fnt);
	drawSlideGroup(ctrlPt.x,ctrlPt.y+175,boxW,h,freqLabel,freq,*fnt);

	//-_-_-_-_-_ draw amplitude slider -_-_-_-_-_-_
	string ampLabel="Amplitude slider";
	drawDisp(amp,ssprintf("%03i",int((sinWave.amplPercent)*100)),"%",*fnt);
	drawSlideGroup(ctrlPt.x,ctrlPt.y+350,boxW,h,ampLabel,amp,*fnt);

	//-_-_-_-_-_ draw box around group -_-_-_-_-_-_
	ofSetColor(white);
	drawGroupBox(x,y,w,h,"MANUAL CONTROL",*lblFnt);

	//sinWave.auxilliaryDraw(100,100,100,200,*lblFnt);
}

void manualMode::update()
{
	sinWave.update();
}

void averageSliderMotion(double & prev, ofSlider & sld){
	if(sld.pressed()&&abs(prev-sld.getPercent())>.1)
			sld.setPercent((9*prev+sld.getPercent())/10);
	prev=sld.getPercent();
}

bool manualMode::clickDown(int x, int y)
{
	bool ret=0;
	if(sine.over(x,y)){
		if(!(sine.sine->isRunning()))
			ret=true,table->handleTrajectory(*sine.sine);
		else table->stopTable();
		sine.setPressed(true);
	}
	else if(freq.sld.clickDown(x,y)||freq.up.clickDown(x,y)||freq.dn.clickDown(x,y)){
		ret=true;
		double freqPerc=freq.sld.getPercent();
		averageSliderMotion(prevFreqPerc,freq.sld);
		if(freq.up.pressed()&&sinWave.freq<16.) freq.sld.setPercent((log(double(int(10*sinWave.freq+1.5))/10.)/log(2.)+1)/5);
		if(freq.dn.pressed()&&sinWave.freq>.5) freq.sld.setPercent((log(double(int(10*sinWave.freq-.9))/10.)/log(2.)+1)/5);

		sinWave.freq=pow(2,freq.sld.getPercent()*(5)-1);
	}
	
	else if(amp.sld.clickDown(x,y)||amp.up.clickDown(x,y)||amp.dn.clickDown(x,y)){
		ret=true;
		double ampPerc=amp.sld.getPercent();
		averageSliderMotion(prevAmpPerc,amp.sld);
		if(amp.up.pressed()&&ampPerc<=.9) amp.sld.setPercent(double(int(10*ampPerc+1.5))/10.);
		if(amp.dn.pressed()&&ampPerc>=.1) amp.sld.setPercent(double(int(10*ampPerc-.9))/10.);
		sinWave.amplPercent=.1+.9*amp.sld.getPercent();
	}
	
	if(ret&&!(sine.sine->isRunning())){
		table->handleTrajectory(*sine.sine);
	}
	inst.clickDown(x,y);
	return ret;
}

bool manualMode::clickUp()
{
	bool ret=0;
	amp.clickUp();
	freq.clickUp();
	sine.clickUp();
	inst.clickUp();
	return ret;
}

bool manualMode::drag(int x, int y)
{
	bool ret=0;
	if(freq.sld.dragB(x,y)){
		averageSliderMotion(prevFreqPerc,freq.sld);
		sinWave.freq=pow(2,freq.sld.getPercent()*(5)-1);
	}
	if(amp.sld.dragB(x,y)){
		averageSliderMotion(prevAmpPerc,amp.sld);
		sinWave.amplPercent=.1+.9*amp.sld.getPercent();
	}
	return ret;
}

void manualMode::reset()
{

}

//_-_-_-_-_-_-_-_-_-_-_ automated earthquake controls -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void autoMode::setup(motionTable * tbl,ofFont * fnt1, ofFont * fnt2)
{
	fnt=fnt1;
	lblFnt=fnt2;
	table=tbl;

	int numAcc=DIR.listDir("accelerogram/");
	double maxNet=0;
	for(int i=0; i<numAcc; i++){
		shakes.push_back(shakeTraj());
		shakes[i].setup(DIR.getPath(i));
		maxNet=max(shakes[i].maxNet,maxNet);
	}
	for(unsigned int i=0; i<shakes.size(); i++){
		shakes[i].updateMaxNet(maxNet);
	}

	buttons.setup(&shakes);

	inst.setup("quakes");
}
	
void autoMode::draw(int x, int y, int w, int h)
{
	//-_-_-_-_-_-_-_-_ draw headline -_-_-_-_-_-_-_-_-_-_-_-_
	string title="SELECT AN EARTHQUAKE\nTO TEST YOUR STRUCTURE";
	ofSetColor(yellow);
	lblFnt->setSize(24);
	int wid=lblFnt->stringWidth(title);
	lblFnt->drawString(title,x+lblFnt->stringHeight("Kjg")*1.5+10,y+10);
	lblFnt->setSize(30);

	//-_-_-_-_-_-_-_-_ draw black line under headline -_-_-_-_-_-_-_-_-_-_-_-_
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofLine(x+lblFnt->stringHeight("Kjg")*1.5+10,y+10+lblFnt->stringHeight(title),x+w-10,y+10+lblFnt->stringHeight(title));
	ofSetLineWidth(1.0);

	//_-_-_-_-_-_-_-_-_ draw buttons -_-_-_-_-_-_-_-_
	ofSetColor(white);
	buttons.draw(x+w-buttons.w,y+h-(buttons.h+200),w,h);

	//-_-_-_-_-_-_-_-_ draw box -_-_-_-_-_-_-_-_-_-_-_-_
	ofSetColor(white);
	drawGroupBox(x,y,w,h,"EARTHQUAKES",*lblFnt);
}
	
void autoMode::update()
{

}

bool autoMode::clickDown(int x, int y)
{
	bool ret=0;
	if(buttons.clickDown(x,y)){
		ret=true;
		table->handleTrajectory(*(buttons.lastPressed().shake));
	}
	inst.clickDown(x,y);
	return ret;
}

bool autoMode::clickUp()
{
	buttons.clickUp();
	inst.clickUp();
	return false;
}

bool autoMode::drag(int x, int y)
{
	return false;
}

void autoMode::reset()
{

}

//-_-_-_-_-_-_-_-_-_-_-_-_ interface -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

// no special needs in creating the interface

tableInterface::tableInterface()
{

}

tableInterface::~tableInterface()
{

}

void tableInterface::setup(motionTable * tbl)
{
	table=tbl;

	qSelect.setup("TRY AN EARTHQUAKE","fonts/DinC.ttf",30);
	qSelect.arial.setMode(OF_FONT_TOP);

	oSelect.setup("NEW USER?","fonts/DinC.ttf",50);
	oSelect.arial.setMode(OF_FONT_TOP);

	label.loadFont("fonts/DinC.ttf");
	label.setSize(30);
	label.setMode(OF_FONT_TOP);
	

	subtext.loadFont("fonts/Arial.ttf");
	subtext.setSize(15);
	subtext.setMode(OF_FONT_BOT);
	subtext.setMode(OF_FONT_CENTER);

	homeTimer.set(1);
	homeTimer.pause();

	oSelect.setPressed(true);

	man.setup(table,&subtext,&label);
	quake.setup(table,&subtext,&label);
}
	
void tableInterface::draw(int x, int y)
{
	int boxOff=100;
	int h=ofGetHeight()-(y-60)*2;
	int w=ofGetWidth()-(x-60)*2;
	int boxW=ofGetWidth()-x*2-boxOff;
	int boxH=x*2;

	//-_-_-_-_- draw the rounded background -_-_-_-_-
	ofSetColor(gray.opacity(.75));
	ofRoundShape(x-60,y-60,w,h,20,true);
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofRoundShape(x-60,y-60,w,h,20,false);
	ofSetLineWidth(1.0);

	//--------  Background of the graph area
	ofSetColor(black+150);
	ofRect(x-10+boxOff,y-10,boxW+20,boxH+20);
	ofSetColor(black);
	ofRect(x-2+boxOff,y-2,boxW+4,boxH+4);
	ofSetColor(white);
	ofRect(x+boxOff,y,boxW,boxH);


	//-_-_-_-_-_-_-_-_- draw the running trajectory -_-_-_-_-_-_-_-_-
	if(table->isRunning()){
		table->runningTrajectory().auxilliaryDraw(x,y,boxOff,boxH,label);
		table->runningTrajectory().draw(x+boxOff,y,boxW,boxH);
	}

	//-_-_-_-_- shade over the traj -_-_-_-_-
	ofShade(x+boxOff,y,20,boxH,OF_RIGHT,.7);
	ofShade(ofGetWidth()-x,y,20,boxH,OF_LEFT,.7);

	//-_-_-_-_- if we have the quakes selected, draw the oscillation select and quake options -_-_-_-_-
	if(qSelect.pressed()){
		ofSetColor(yellow);
		oSelect.draw((ofGetWidth()-oSelect.w)/2,y+h-(oSelect.h*1.75+50));
		quake.draw(x-40,y+boxH+50,(boxW+boxOff)/2,h-(boxH+250));
		quake.inst.draw(ofGetWidth()/2+40,y+boxH+50,(boxW+boxOff)/2,h-(boxH+250));
	}

	//-_-_-_-_- if we have the manual mode selected, draw the quake select button and manual controls -_-_-_-_-
	if(oSelect.pressed()){
		ofSetColor(yellow);
		if(man.inst.getState()){
			label.setSize(24);
			label.setMode(OF_FONT_BOT);
			label.drawString("Or",qSelect.x-label.stringWidth("Or")-20, qSelect.y+qSelect.h-10);
			label.setMode(OF_FONT_TOP);
			label.setSize(30);
			qSelect.draw(ofGetWidth()/2+40+(boxW/2-qSelect.w)/2,y+h-(qSelect.h*2+50));
		}
		man.draw(x-40,y+boxH+50,(boxOff+boxW)/2,h-(boxH+250));
		man.inst.draw(ofGetWidth()/2+40,y+boxH+50,(boxW+boxOff)/2,h-(boxH+250));
	}

	//-_-_-_-_- if the table is homing, gray out everything -_-_-_-_-
	if(table->isHoming()){
		ofSetColor(black.opacity(.75));
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofSetColor(white);
		homing.draw(ofGetWidth()/2,ofGetHeight()/2,ofGetHeight()/4);
		label.setMode(OF_FONT_CENTER);
		label.setSize(70);
		ofSetColor(white);
		label.drawString("Centering table, please wait",ofGetWidth()/2,ofGetHeight()/8);
		label.setSize(30);
		label.setMode(OF_FONT_LEFT);
	}
}

void tableInterface::update()
{
	man.update();
	if(table->justStopped()){
	}
	if(homeTimer.getPercent()>.5&&homeTimer.getPercent()<.6){
		//table->home();
	}
	table->update();
}

bool tableInterface::clickDown(int _x, int _y)
{
	bool ret=0;
	if(!table->isHoming()){
		if(!oSelect.pressed()){
			if(oSelect.clickDown(_x,_y)){
				ret=true;
				table->home();
				man.inst.reset();
				qSelect.setPressed(false);
			}
			ret=quake.clickDown(_x,_y);
		}
		if(!ret&&!qSelect.pressed()){
			if(man.inst.getState()&&qSelect.clickDown(_x,_y)){
				table->home();
				oSelect.setPressed(false);
			}
			ret=man.clickDown(_x,_y);
		}
		if(stop.clickDown(_x,_y)){
			table->stopTable();
			ofGetAppPtr()->draw();
			table->home();
		}
	}
	return ret;
}

bool tableInterface::clickUp()
{
	bool ret=0;
	ret|=man.clickUp();
	ret|=quake.clickUp();
	ret|=stop.clickUp();
	return ret;
}

void tableInterface::drag(int x, int y){
	man.drag(x,y);
}