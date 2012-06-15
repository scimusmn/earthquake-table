#include "interfaceObjects.h"
#include "../../../dallasEng/dallasEng.h"


// setup the button for an earthquake. sets the width to the width of the location, date and magnitude strings *2.

extern ofColor white,black,gray,yellow,blue,orange,red;

void shakeButton::setup(shakeTraj * shk)
{
	bAvailable=true;
	shake=shk;
	duration=shake->duration();
	dur=ssprintf("%i:%02i", int(duration)/60,int(duration)%60);
	arial.loadFont("fonts/HelveticaCond.otf");
	arial.setSize(20);
	cells.clear();
	addCell(shake->location,"Location");
	addCell(shake->date,"Date");
	addCell(dur,"Duration");
	addCell(shake->magnitude,"Magnitude");
	for(unsigned int i=0; i<cells.size(); i++){
		w+=cells[i].width;
	}
	h=arial.stringHeight("Kjg")+10;
	arial.setMode(OF_FONT_TOP);
}

void shakeButton::addCell(string val, string name)
{
	cells.push_back(cell(max(label().stringWidth(val)+10,label().stringWidth(name)+10),val));
}

//Draw a rectangle to hold the text of the earthquake, color dependent on the state of the trajectory

void shakeButton::draw(int _x, int _y, int _w)
{
	x=_x, y=_y;
	if(w!=_w){
		int incWid=(_w-w)/4;
		w=_w;
		for(unsigned int i=0; i<cells.size(); i++){
			cells[i].width+=incWid;
		}
	}

	ofColor k=ofGetStyle().color;
	double a=1;
	ofSetColor(orange.opacity(.55*a));
	if(shake->isRunning()){
		ofSetColor(red.opacity(.75*a));
	}
	ofRect(x,y,_w,h);
	ofSetColor(white.opacity(a));
	ofNoFill();
	ofRect(x,y,_w,h);
	ofFill();
	int pos=0;
	for(unsigned int i=0; i<cells.size(); i++){
		label(cells[i].val,x+pos+5,y+5);
		pos+=cells[i].width;
		if(i<cells.size()-1) ofRect(x+pos,y,2,h);
	}

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
	lastPress=0;
	shakes=trajs;
	w=0,nameWid=0,dateWid=0,durWid;
	int lblSize= label().getSize();
	labelSize=18;
	label().setSize(labelSize);
	cells.push_back(cell(0,"Location"));
	cells.push_back(cell(0,"Date"));
	cells.push_back(cell(0,"Duration"));
	cells.push_back(cell(0,"Magnitude"));
	for(unsigned int i=0; i<shakes->size(); i++){
		buttons.push_back(shakeButton());
		buttons[i].setup(&shakes->at(i));
		w=max(w,buttons[i].w);
		for(unsigned int j=0; j<cells.size();j++){
			cells[j].width=max(cells[j].width,buttons[i].cells[j].width);
		}
	}
	label().setSize(lblSize);
	for (unsigned int i=0; i<buttons.size(); i++){
		buttons[i].w=w;
		for(unsigned int j=0; j<cells.size();j++){
			buttons[i].cells[j].width=cells[j].width;
		}
	}
}

// draw all of the buttons
	
void shakeButs::draw(int _x, int _y, int _w, int _h)
{
	x=_x;
	y=_y;
	int totH=0;
	if(w!=_w){
		double incWid=(_w-w)/4.;
		w=_w;
		for(unsigned int i=0; i<cells.size(); i++){
			cells[i].width+=incWid;
		}
	}
	ofFont * lbl=&label();

	int fntSz=label().getSize();
	label().setMode(OF_FONT_LEFT);
	label().setMode(OF_FONT_TOP);

	label().setSize(labelSize);
	/*if(buttons.size()){
		lbl = &buttons[0].arial;
	}*/

	//-_-_-_-_-_-_-_-_ Draw black backing -_-_-_-_-_-_-_-_
	ofSetColor(black.opacity(.5));
	ofRect(x,y,_w,h+60);

	//-_-_-_-_-_-_-_-_ draw divs -_-_-_-_-_-_-_-_
	ofSetColor(white.opacity(.85));
	int pos=0;
	//int incWid=(_w-w)/4;
	for(unsigned int i=0; i<cells.size(); i++){
		label(cells[i].val,x+pos+5,y+5);
		pos+=cells[i].width;
		if(i<cells.size()-1) ofRect(x+pos,y,2,h);
	}

	for(unsigned int i=0; i<buttons.size(); i++){
		buttons[i].draw(x,y+50+totH,_w);
		totH+=buttons[i].h+10;
	}

	//-_-_-_-_-_-_-_-_ Draw black border -_-_-_-_-_-_-_-_
	ofSetColor(black);
	ofPushStyle();
	ofNoFill();
	ofSetLineWidth(2.0);
	ofRect(x,y,_w,h+60);
	ofPopStyle();

	label().setSize(fntSz);
	label().setMode(OF_FONT_CENTER);
	label().setMode(OF_FONT_MID);

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
	ofRoundedRect(x,y,w,h,h/4);
	ofSetColor(gray.opacity(k.a/255.));
	ofNoFill();
	ofRoundedRect(x,y,w,h,h/4);
	ofFill();
	arial.drawString(title,x+10,y+5);
}