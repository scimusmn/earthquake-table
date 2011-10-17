#include "interfaceObjects.h"


// setup the button for an earthquake. sets the width to the width of the location, date and magnitude strings *2.

double wM=1.7;

void shakeButton::setup(shakeTraj * shk)
{
	bAvailable=true;
	shake=shk;
	duration=shake->duration();
	dur=ssprintf("%i:%i", int(duration)/60,int(duration)%60);
	arial.loadFont("fonts/Arial.ttf");
	arial.setSize(20);
	nameWid=arial.stringWidth(shake->location);
	dateWid=arial.stringWidth(shake->date);
	durWid=arial.stringWidth(dur);
	w=wM*(nameWid+dateWid+durWid+arial.stringWidth(shake->magnitude));
	h=arial.stringHeight("Kjg")+10;
	arial.setMode(OF_FONT_TOP);
}

//Draw a rectangle to hold the text of the earthquake, color dependent on the state of the trajectory

void shakeButton::draw(int _x, int _y)
{
	x=_x, y=_y;
	ofColor k=ofGetStyle().color;
	double a=k.a/255.;
	ofSetColor(251,176,23,128*a);
	if(shake->isRunning()){
		//ofSetColor(k-.2*255);
		ofSetColor(209,35,42,196*a);
	}
	ofRect(x,y,w,h);
	ofSetColor(255,255,255,k.a);
	ofNoFill();
	ofRect(x,y,w,h);
	ofFill();
	k=ofGetStyle().color;
	ofSetColor(k+.2*255);
	//for(int i=1; i<3; i++){
	ofRect(x+nameWid*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid)*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid+durWid)*wM*.85,y,2,h);
	//}
	ofSetColor(255,255,255,k.a);
	arial.drawString(shake->location,x+10,y+5);
	arial.drawString(shake->date,x+nameWid*(wM*.7)+10,y+5);
	arial.drawString(dur,x+(nameWid+dateWid)*(wM*.7)+10,y+5);
	arial.drawString(shake->magnitude,x+(nameWid+dateWid+durWid)*wM*.85+10,y+5);
}

// bind the trajectory to the button
	
void sinButton::setup(sinTraj * sin)
{
	sine=sin;
	arial.loadFont("fonts/Arial.ttf");
	arial.setSize(25);
	title="start oscillations";
	w=20+arial.stringWidth(title);
	h=arial.stringHeight("Kjg")+10;
	arial.setMode(OF_FONT_TOP);
}

// draw teh button, with text and color dependent on if the trajectory is running

void sinButton::draw(int _x, int _y)
{
	x=_x, y=_y;
	ofColor k=ofGetStyle().color;

	// if the traj is bound to the button and is running change the color.
	if(sine&&sine->isRunning()) title="Stop oscillations", ofSetColor(k-.2*255);
	else title="Start oscillations";
	ofRoundShape(x,y,w,h,h/4,true);
	ofSetColor(255*.2,255*.2,255*.2,k.a);
	arial.drawString(title,x+10,y+5);
}

// return the ith member of the buttons vector

shakeButton & shakeButs::operator[](int i)
{
	return buttons[i];
}
	
// return the size of the buttons vector

int shakeButs::size()
{
	return buttons.size();
}

// create and setup the buttons using the trajs vector *; set a standard width

void shakeButs::setup(vector<shakeTraj> * trajs)
{
	shakes=trajs;
	double wid=0,nW=0,dW=0,durW;
	for(unsigned int i=0; i<shakes->size(); i++){
		buttons.push_back(shakeButton());
		buttons[i].setup(&shakes->at(i));
		wid=max(wid,buttons[i].w);
		nW=max(nW,buttons[i].nameWid);
		dW=max(dW,buttons[i].dateWid);
		durW=max(durW,buttons[i].durWid);
	}
	for (unsigned int i=0; i<buttons.size(); i++){
		buttons[i].w=wid;
		buttons[i].nameWid=nW;
		buttons[i].dateWid=dW;
		buttons[i].durWid=durW;
	}
	w=wid;
}

// draw all of the buttons
	
void shakeButs::draw(int _x, int _y)
{
	int totH=0;
	ofColor k=ofGetStyle().color;
	for(unsigned int i=0; i<buttons.size(); i++){
		ofSetColor(64,128,200,k.a);
		buttons[i].draw(_x,_y+totH);
		totH+=buttons[i].h+10;
	}
	h=totH;
}

// check to see if we are over each button; if so set lastPress to i;
	
bool shakeButs::clickDown(int _x, int _y)
{
	bool ret=0;
	for(unsigned int i=0; i<buttons.size()&&!ret; i++){
		if(buttons[i].clickDown(_x, _y)){
			lastPress=i, ret=true;
		}
	}
	return ret;
}

// return the last button pressed

shakeButton & shakeButs::lastPressed()
{
	return buttons[lastPress];
}
	
// clickUp each of the buttons

bool shakeButs::clickUp()
{
	bool ret=0;
	for(unsigned int i=0; i<buttons.size(); i++){
		if(buttons[i].clickUp()){
			ret=true;
		}
	}
	return ret;
}

//draw a fancy rectangle and text for the stop button

void eqButton::draw(int _x, int _y)
{
	x=_x, y=_y;
	w=20+arial.stringWidth(title);
	h=arial.stringHeight("Kjg")+10;
	ofColor k=ofGetStyle().color;
	if(bPressed) ofSetColor(k-.2*255);
	//ofRect(x,y,w,h);
	ofRoundShape(x,y,w,h,h/4,true);
	ofSetColor(255*.2,255*.2,255*.2);
	ofRoundShape(x,y,w,h,h/4,false);
	arial.drawString(title,x+10,y+5);
}