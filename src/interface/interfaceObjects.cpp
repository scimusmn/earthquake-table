#include "interfaceObjects.h"


// setup the button for an earthquake. sets the width to the width of the location, date and magnitude strings *2.

double wM=1.65;

ofColor white(255,255,255);
ofColor black(0,0,0);
ofColor gray(0x333333);
ofColor yellow(229,225,15);
ofColor blue(109,202,208);
ofColor orange(251,176,23);
ofColor red(209,35,42);

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
	double a=1;
	ofSetColor(orange.opacity(.55*a));
	if(shake->isRunning()){
		ofSetColor(red.opacity(.75*a));
	}
	ofRect(x,y,w,h);
	ofSetColor(white.opacity(a));
	ofNoFill();
	ofRect(x,y,w,h);
	ofFill();
	//for(int i=1; i<3; i++){
	ofRect(x+nameWid*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid)*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid+durWid)*wM*.85,y,2,h);
	//}
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
	ofSetColor(gray.opacity(k.a/255.));
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
	w=0,nameWid=0,dateWid=0,durWid;
	for(unsigned int i=0; i<shakes->size(); i++){
		buttons.push_back(shakeButton());
		buttons[i].setup(&shakes->at(i));
		w=max(w,buttons[i].w);
		nameWid=max(nameWid,buttons[i].nameWid);
		dateWid=max(dateWid,buttons[i].dateWid);
		durWid=max(durWid,buttons[i].durWid);
	}
	for (unsigned int i=0; i<buttons.size(); i++){
		buttons[i].w=w;
		buttons[i].nameWid=nameWid;
		buttons[i].dateWid=dateWid;
		buttons[i].durWid=durWid;
	}
}

// draw all of the buttons
	
void shakeButs::draw(int _x, int _y, int _w, int _h)
{
	x=_x;
	y=_y;
	int totH=0;
	ofFont * lbl;
	if(buttons.size()){
		lbl = &buttons[0].arial;
	}

	//-_-_-_-_-_-_-_-_ Draw black backing -_-_-_-_-_-_-_-_
	ofSetColor(black.opacity(.5));
	ofRect(x-20,y,w+20,h+60);

	//-_-_-_-_-_-_-_-_ draw divs -_-_-_-_-_-_-_-_
	ofSetColor(white.opacity(.85));
	ofRect(x+nameWid*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid)*(wM*.7),y,2,h);
	ofRect(x+(nameWid+dateWid+durWid)*wM*.85,y,2,h);

	//-_-_-_-_-_-_-_-_ draw labels for quakes -_-_-_-_-_-_-_-_
	lbl->drawString("Location",x+10,y+5);
	lbl->drawString("Date",x+nameWid*(wM*.7)+10,y+5);
	lbl->drawString("Duration",x+(nameWid+dateWid)*(wM*.7)+10,y+5);
	lbl->drawString("Magnitude",x+(nameWid+dateWid+durWid)*wM*.85+10,y+5);
	for(unsigned int i=0; i<buttons.size(); i++){
		buttons[i].draw(x,y+50+totH);
		totH+=buttons[i].h+10;
	}

	//-_-_-_-_-_-_-_-_ Draw black border -_-_-_-_-_-_-_-_
	ofSetColor(black);
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2.0);
	ofRect(x-20,y,w+20,h+60);
	ofPopStyle();

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
	ofSetColor(gray.opacity(k.a/255.));
	ofRoundShape(x,y,w,h,h/4,false);
	arial.drawString(title,x+10,y+5);
}