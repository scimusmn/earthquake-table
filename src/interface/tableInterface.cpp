#include "tableInterface.h"
#include "../eqConfig.h"
#include "../../../dallasEng/dallasEng.h"

//--------------------- instruction box -----------

extern ofColor white;
extern ofColor black;
extern ofColor gray;
extern ofColor yellow;
extern ofColor blue;
extern ofColor orange;
extern ofColor red;

extern string MAIN_TITLE;

instructionBox inst;

void instructionBox::setup(string folder)
{
	o_rodInst.loadImage("images/"+folder+"/oRods.jpg");
	o_buildInst.loadImage("images/"+folder+"/oBuild.jpg");
	q_rodInst.loadImage("images/"+folder+"/qRods.jpg");
	q_buildInst.loadImage("images/"+folder+"/qBuild.jpg");
	rodSel.setup(300,300,"images/"+folder+"/rodBut.jpg");
	buildSel.setup(300,300,"images/"+folder+"/buildBut.jpg");
	resetBut.setup("Return to experiment selection",15);
}
	
void instructionBox::draw(int x, int y, int w, int h)
{
	if(!rodSel.pressed()&&!buildSel.pressed()&&mode==TABLE_SELECT){
		ofSetColor(white*.1);
		ofRect(x,y,w,h);
		ofSetColor(gray);
		drawHatching(x,y,w,h,50,50);
		ofSetColor(yellow);
		header().setMode(OF_FONT_CENTER);
		header("Choose your structure type",x+w/2,y+h/4);
		header().setMode(OF_FONT_LEFT);
		ofSetColor(white);
		rodSel.draw(x+w/3-rodSel.w/2,y+(h-rodSel.h)/2);
		buildSel.draw(x+2*w/3-buildSel.w/2,y+(h-buildSel.h)/2);
	}
	else if(mode==TABLE_OSC){
		ofSetColor(white);
		if(buildSel.pressed()) o_buildInst.draw(x,y,w,h);
		if(rodSel.pressed()) o_rodInst.draw(x,y,w,h);
		ofSetColor(yellow);
		resetBut.draw(x+w-resetBut.w-20,y+h-resetBut.h-20);
	}
	else if(mode==TABLE_QUAKE){
		ofSetColor(white);
		if(buildSel.pressed()) q_buildInst.draw(x,y,w,h);
		if(rodSel.pressed()) q_rodInst.draw(x,y,w,h);
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
		if(rodSel.clickDown(x,y)||buildSel.clickDown(x,y))
			mode=TABLE_OSC;
		if(rodSel.pressed()) MAIN_TITLE="See how rods respond to vibration";
		if(buildSel.pressed()) MAIN_TITLE="See how buildings respond to vibration";
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
	mode=TABLE_SELECT;
	table().stopTable();
	MAIN_TITLE="See how your structure responds to motion";
}

int instructionBox::getState()
{
	int ret=0;
	if(rodSel.pressed()) ret=1;
	if(buildSel.pressed()) ret=2;
	return ret;
}

//-_-_-_-_-_-_-_-_-_-_ Draw function fo the white box around each group -_-_-_-_-_-_-_-_-_-_

int drawGroupBox(int x, int y, int w, int h, string title)
{
	ofColor k=ofGetStyle().color;

	int ret;

	int strWid=ret=header().stringHeight(title)*1.5;
	 
	ofSetColor(gray);
	ofRect(x+strWid,y,w-strWid,h);
	ofSetColor(white*.1);
	drawHatching(x+strWid,y,w-strWid,h,15,1);

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
	ofTranslate(x+strWid/4,y+h-20);
	ofRotate(-90);
	header().drawString(title,0,0);
	ofPopMatrix();

	return ret;
}

//_-_-_-_-_-_-_-_-_-_-_ manual controls -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void drawSlideGroup(int x, int y, int w, int h,string lbl, slidePack & t){

	//-_-_-_-_-_ draw title -_-_-_-_-_-_
	ofSetColor(white);
	label(lbl,x+w/2,t.disp.y-40);
	ofSetColor(black);
	ofPushStyle();
	ofSetLineWidth(2);
	ofLine(x+10,t.disp.y-20,x+w-10,t.disp.y-20);
	ofPopStyle();

	//-_-_-_-_-_ slider draw -_-_-_-_-_-_
	ofSetColor(blue);
	t.sld.draw(x+(w-t.sld.w)/2-100,y,t.sld.w,15);

	//-_-_-_-_-_ button draw -_-_-_-_-_-_
	ofSetColor(white);
	t.dn.draw(t.sld.x-t.dn.w-30,t.sld.y+(t.sld.h-t.dn.h)/2);
	t.up.draw(t.sld.x+t.sld.w+30,t.sld.y+(t.sld.h-t.up.h)/2);
}

void drawDisp(slidePack & t,string contents,string add){
	//ofRoundShadow(t.disp.x-10,t.disp.y-10,t.disp.w+20,t.disp.h+20,5,1);
	ofSetShadowDarkness(1);
	ofShadowRounded(t.disp.x-5,t.disp.y-5,t.disp.w+10,t.disp.h+10,5,5);
	ofSetShadowDarkness(.3);
	ofSetColor(blue);
	t.disp.draw(contents,t.up.x+t.up.w+20,t.sld.y+(t.sld.h-t.disp.h)/2);

	//-_-_-_-_-_ draw addendum -_-_-_-_-_-_
	label().setSize(25);
	label(add,t.disp.x+t.disp.w+30,t.disp.y+t.disp.h);
	label().setSize(15);
}

void manualMode::setup()
{
	amp.setup();
	freq.setup();
	sinWave.setup(.5,10000,5);
	prevFreqPerc=0;
	prevAmpPerc=1;

	start.setup("Start oscillations",25);
	slideWidth=300;
	amp.sld.w=slideWidth;
	freq.sld.w=slideWidth;
	amp.sld.setPercent(sinWave.amplPercent);
}
	
void manualMode::draw(int x, int y, int w, int h)
{
	//-_-_-_-_-_ draw box around group -_-_-_-_-_-_
	ofSetColor(white);
	drawGroupBox(x,y,w,h,"MANUAL CONTROL");

	header().setSize(24);
	double sideLabelW=header().stringHeight("Kjg")*1.5;
	ofRectangle controls;
	controls.width=w-sideLabelW;
	double boxX=x+sideLabelW;

	//ofSetColor(64,128,200);

	ofPoint ctrlPt(boxX,y+100);

	//-_-_-_-_-_ draw transparent black bg -_-_-_-_-_-_
	ofSetColor(black.opacity(.25));
	ofRect(ctrlPt.x+10,ctrlPt.y,controls.width-20,h-150);
	ofPushStyle();
	ofNoFill();
	ofSetColor(black);
	ofSetLineWidth(2);
	ofRect(ctrlPt.x+10,ctrlPt.y,controls.width-20,h-150);
	ofPopStyle();

	ofSetColor(yellow);
	start.draw(ctrlPt.x+(controls.width-start.w)/2,ctrlPt.y+25);

	//-_-_-_-_-_ draw headline -_-_-_-_-_-_
	string title="FIND THE RESONANCE\nOF YOUR STRUCTURE";
	ofSetColor(yellow);
	header().setSize(24);
	int wid=header().stringWidth(title);
	header(title,x+sideLabelW+10,y+10);
	header().setSize(30);

	//-_-_-_-_-_ draw black line under headline -_-_-_-_-_-_
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofLine(x+sideLabelW+10,y+10+header().stringHeight(title),x+w-10,y+10+header().stringHeight(title));
	ofSetLineWidth(1.0);

	//-_-_-_-_-_ draw frequency slider -_-_-_-_-_-_
	string freqLabel="Frequency slider";
	int nFreq=sinWave.freq;
	drawDisp(freq,ssprintf("%02i.%i",nFreq,int((sinWave.freq+.05)*10-nFreq*10)),"hz");
	drawSlideGroup(ctrlPt.x,ctrlPt.y+175,controls.width,h,freqLabel,freq);

	//-_-_-_-_-_ draw amplitude slider -_-_-_-_-_-_
	string ampLabel="Amplitude slider";
	drawDisp(amp,ssprintf("%03i",int((sinWave.amplPercent)*100)),"%");
	drawSlideGroup(ctrlPt.x,ctrlPt.y+350,controls.width,h,ampLabel,amp);

	//sinWave.auxilliaryDraw(100,100,100,200,*lblFnt);
}

void manualMode::update()
{
	sinWave.update();
}

void averageSliderMotion(double & prev, ofSlider & sld){
	if(sld.pressed()&&abs(prev-sld.getPercent())>.01)
			sld.setPercent((.98*prev+.02*sld.getPercent()));
	prev=sld.getPercent();
}

bool manualMode::clickDown(int x, int y)
{
	bool ret=0;
	if(start.toggle(x,y)){
		if(!(sinWave.isRunning()))
			ret=true;
		else table().stopTable(),start.setTitle("Start oscillations");
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
	
	if(ret&&!(sinWave.isRunning())){
		table().handleTrajectory(sinWave);
		start.setTitle("Stop oscillations");
	}
	if(inst.clickDown(x,y)) reset();
	return ret;
}

bool manualMode::clickUp()
{
	bool ret=0;
	amp.clickUp();
	freq.clickUp();
	start.clickUp();
	inst.clickUp();
	return ret;
}

bool manualMode::drag(int x, int y)
{
	bool ret=0;
	freq.sld.drag(x,y);
	if(freq.sld.pressed()){
		averageSliderMotion(prevFreqPerc,freq.sld);
		sinWave.freq=pow(2,freq.sld.getPercent()*(5)-1);
	}
	amp.sld.drag(x,y);
	if(amp.sld.pressed()){
		averageSliderMotion(prevAmpPerc,amp.sld);
		sinWave.amplPercent=.1+.9*amp.sld.getPercent();
	}
	return ret;
}

void manualMode::reset()
{
	table().stopTable(),start.setTitle("Start oscillations");
	freq.sld.setPercent(0),amp.sld.setPercent(100);
	sinWave.reset();
	sinWave.freq=pow(2,freq.sld.getPercent()*(5)-1);
	sinWave.amplPercent=.1+.9*amp.sld.getPercent();
	prevFreqPerc=freq.sld.getPercent();
	prevAmpPerc=freq.sld.getPercent();
}

//_-_-_-_-_-_-_-_-_-_-_ automated earthquake controls -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_

void autoMode::setup()
{
	cout << cfg().eqDir << endl;
	int numAcc=DIR.listDir(cfg().eqDir);
	double maxNet=0;
	for(int i=0; i<numAcc; i++){
		shakes.push_back(shakeTraj());
		shakes[i].setup(DIR.getPath(i));
		maxNet=max(shakes[i].maxNet,maxNet);
	}
	for(unsigned int i=0; i<shakes.size(); i++){
		if(cfg().scaleToAll) shakes[i].updateMaxNet(maxNet);
	}

	buttons.setup(&shakes);
}
	
void autoMode::draw(int x, int y, int w, int h)
{
	//-_-_-_-_-_-_-_-_ draw box -_-_-_-_-_-_-_-_-_-_-_-_
	ofSetColor(white);
	int off=drawGroupBox(x,y,w,h,"EARTHQUAKES");

	//-_-_-_-_-_-_-_-_ draw headline -_-_-_-_-_-_-_-_-_-_-_-_
	string title="SELECT AN EARTHQUAKE\nTO TEST YOUR STRUCTURE";
	ofSetColor(yellow);
	header().setSize(24);
	int wid=header().stringWidth(title);
	header(title,x+header().stringHeight("Kjg")*1.5+10,y+10);
	header().setSize(30);

	//-_-_-_-_-_-_-_-_ draw black line under headline -_-_-_-_-_-_-_-_-_-_-_-_
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofLine(x+header().stringHeight("Kjg")*1.5+10,y+10+header().stringHeight(title),x+w-10,y+10+header().stringHeight(title));
	ofSetLineWidth(1.0);

	//_-_-_-_-_-_-_-_-_ draw buttons -_-_-_-_-_-_-_-_
	ofSetColor(white);
	buttons.draw(x+off+2,y+(h-(buttons.h))/2,w-off-4,h);
}
	
void autoMode::update()
{

}

bool autoMode::clickDown(int x, int y)
{
	bool ret=0;
	if(buttons.clickDown(x,y)){
		ret=true;
		table().handleTrajectory(*(buttons.lastPressed().shake));
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

void tableInterface::setup()
{

	qSelect.setup("TRY AN EARTHQUAKE",30);

	oSelect.setup("NEW USER?",30);

	header().loadFont("fonts/DinC.ttf");
	header().setSize(30);
	header().setMode(OF_FONT_TOP);
	

	label().loadFont("fonts/Arial.ttf");
	label().setSize(15);
	label().setMode(OF_FONT_BOT);
	label().setMode(OF_FONT_CENTER);

	homeTimer.set(1);
	homeTimer.pause();

	oSelect.setPressed(true);

	man.setup();
	quake.setup();
	inst.setup("instructions");
}
	
void tableInterface::draw(int _x, int _y)
{
	ofPoint pad(30,0);
	ofPoint margin(100,40);
	x=_x,y=_y;
	h=ofGetHeight()-(y+pad.y*2);
	w=ofGetWidth()-(x+pad.x*2);
	
	ofRectangle graph;
	graph.x=x+pad.x+margin.x;
	graph.y=y+pad.y+margin.y;
	graph.width=ofGetWidth()-graph.x*2;
	graph.height=200;

	ofRectangle controls;
	controls.y=graph.y+graph.height+margin.y;
	controls.x=x+pad.x+margin.x/2;
	controls.width=ofGetWidth()/2-controls.x-margin.x/2;
	controls.height=ofGetHeight()-controls.y-margin.y;

	//-_-_-_-_- shaded rect over the background -_-_-_-_-
	ofSetColor(gray.opacity(.75));
	ofRect(x+pad.x,y+pad.y,w,h);
	ofSetColor(black);
	ofSetLineWidth(2.0);
	ofNoFill();
	ofRect(x+pad.x,y+pad.y,w,h);
	ofFill();
	ofSetLineWidth(1.0);

	//--------  Background of the graph area
	ofSetColor(white*.5);
	ofRect(graph.x-10,graph.y-10,graph.width+20,graph.height+20);
	ofSetColor(black);
	ofRect(graph.x-2,graph.y-2,graph.width+4,graph.height+4);
	ofSetColor(white*.8);
	ofRect(graph.x,graph.y,graph.width,graph.height);


	//-_-_-_-_-_-_-_-_- draw the running trajectory -_-_-_-_-_-_-_-_-
	if(table().isRunning()){
		table().runningTrajectory().auxilliaryDraw(graph.x-margin.x,graph.y,margin.x,graph.height,header()); //(start pos for the text x, y, width for aux, height of graph)
		table().runningTrajectory().draw(graph.x,graph.y,graph.width,graph.height);
	}
	else if(cfg().noTable&&qSelect.pressed()){
		quake.buttons.lastPressed().shake->draw(graph.x,graph.y,graph.width,graph.height);
	}
	else{
		ofSetColor(gray);
		pushHeaderStyle(40,OF_FONT_CENTER);
		if(qSelect.pressed()) header("Select an earthquake below",graph.x+graph.width/2,graph.y+graph.height/2);
		else if(oSelect.pressed()) header("Control the table motion with the sliders below",graph.x+graph.width/2,graph.y+graph.height/2);
		popHeaderStyle();
	}

	//-_-_-_-_- shade over the traj -_-_-_-_-
	ofShade(x+pad.x+margin.x,y+pad.y+margin.y,20,graph.height,OF_RIGHT);
	ofShade(ofGetWidth()-(x+pad.x+margin.x),y+pad.y+margin.y,20,graph.height,OF_LEFT);

	//-_-_-_-_- if we have the quakes selected, draw the oscillation select and quake options -_-_-_-_-
	if(qSelect.pressed()&&inst.getState()){
		ofSetColor(yellow);
		oSelect.draw(ofGetWidth()/2+margin.x/2+(controls.width-oSelect.w)/2,y+h-(oSelect.h+50));
		quake.draw(controls.x,controls.y,controls.width,controls.height);
		inst.draw(ofGetWidth()/2+margin.x/2,controls.y,controls.width,controls.height-100);
	}

	//-_-_-_-_- if we have the manual mode selected, draw the quake select button and manual controls -_-_-_-_-
	if(oSelect.pressed()&&inst.getState()){
		ofSetColor(yellow);
		if(inst.getState()){
			header().setSize(24);
			header().setMode(OF_FONT_BOT);
			header().drawString("Or",qSelect.x-header().stringWidth("Or")-20, qSelect.y+qSelect.h-10);
			header().setMode(OF_FONT_TOP);
			header().setSize(30);
			qSelect.draw(ofGetWidth()/2+margin.x/2+(controls.width-qSelect.w)/2,y+h-(qSelect.h+50));
		}
		man.draw(controls.x,controls.y,controls.width,controls.height);
		inst.draw(ofGetWidth()/2+margin.x/2,controls.y,controls.width,controls.height-100);
	}

	if(inst.mode==TABLE_SELECT)
		inst.draw(0,_y,ofGetWidth(),ofGetHeight()-_y);
}

void tableInterface::update()
{
	man.update();
	if(table().justStopped()){
	}
	if(homeTimer.getPercent()>.5&&homeTimer.getPercent()<.6){
		table().home();
	}
	if(resetTimer.justExpired()){
		qSelect.setPressed(false);
		oSelect.setPressed(true);
		table().stopTable();
		man.reset();
		inst.reset();
	}
	table().update();
}

bool tableInterface::clickDown(int _x, int _y)
{
	bool ret=0;
	if(!table().isHoming()){
		if(!oSelect.pressed()){
			resetTimer.set(cfg().timeout);
			resetTimer.run();
			if(oSelect.clickDown(_x,_y)){
				ret=true;
				if(!cfg().noTable)
					table().home();
				inst.reset();
				inst.mode=TABLE_SELECT;
				qSelect.setPressed(false);
				man.reset();
			}
			ret=quake.clickDown(_x,_y);
		}
		if(!ret&&!qSelect.pressed()){
			resetTimer.set(cfg().timeout);
			resetTimer.run();
			if(inst.getState()&&qSelect.clickDown(_x,_y)){
				if(!cfg().noTable)
					table().home();
				oSelect.setPressed(false);
				inst.mode=TABLE_QUAKE;
				man.reset();
			}
			ret=man.clickDown(_x,_y);
		}
		if(stop.clickDown(_x,_y)){
			table().stopTable();
			ofGetAppPtr()->draw();
			table().home();
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