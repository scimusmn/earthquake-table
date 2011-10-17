#include "accParse.h"

dataPoint::dataPoint(double a, double deltat)
{
	acc=vel=pos=0;
	dTime=deltat*1000;
	acc=a;
	vel=a*deltat;
	pos=a*deltat*deltat*.5;
}

dataPoint::dataPoint(double a, double v, double p, double t)
{
	acc=a, vel=v, pos= p, dTime=t;
}

shakeData::shakeData()
{
	index=vel=pos=0;
	maxDisp=10000;
}
	
shakeData::~shakeData()
{
	uData.clear();
}
	
void shakeData::getPoint(double acc, double deltat)
{
	uData.push_back(dataPoint(acc,deltat));
}
	
void shakeData::loadUnprocessed(string filename)
{
	file=filename;
	ifstream input(ofToDataPath(file).c_str());
	string buffer;
	bool tagOpen=false;
	int pipeCount=0;
	getline(input, buffer);
	while(buffer.length()&&(buffer[0]=='|'||pipeCount<4)){
		if(buffer.length()&&buffer[0]=='|') pipeCount++;
		getline(input, buffer);
	}
	while (input.peek()!=EOF) {
		getline(input, buffer);
		if(buffer.length()%10==0){
			for(int i=0; i<buffer.length()/10; i++){
				string num(buffer.begin()+10*i,buffer.begin()+10*i+10);
				uData.push_back(dataPoint(ofToFloat(num),0.005));
			}
		}
	}
	input.close();
	//vector<dataPoint> d2=uData;
	/*for(unsigned int i=1; i<uData.size()&&uData.size(); i++){
		//uData[i].pos=uData[i-1].pos+.1*(uData[i].pos-uData[i-1].pos);
		uData[i].acc=.5*(uData[i-1].acc + d2[i].acc - d2[i-1].acc);
		uData[i].vel=uData[i].acc*uData[i].dTime;
		uData[i].pos=(uData[i].acc/2)*uData[i].dTime*uData[i].dTime;
	}*/
	double netPos=uData[0].pos;
	maxNet=netPos;
	for(unsigned int i=1; i<uData.size()&&uData.size(); i++){
		uData[i].pos=uData[i-1].pos+.1*(uData[i].pos-uData[i-1].pos);
		//uData[i].pos=.5*(uData[i-1].pos + d2[i].pos - d2[i-1].pos);
		netPos+=uData[i].pos;
		maxNet=max(abs(netPos),maxNet);
	}
	cout << maxNet << " max displacement in cm" << endl;
	maxNet/=maxDisp;

	for(unsigned int i=0; i<uData.size(); i++){
		uData[i].pos/=maxNet;
		uData[i].vel/=(maxNet*100);
		//uData[i].dTime*=2;
	}
}

void shakeData::changeMaxNet(double newMax){
	double change=sqrt(maxNet/newMax);
	for(unsigned int i=0; i<uData.size(); i++){
		uData[i].pos*=change;
		uData[i].vel*=change;
	}
	maxNet=newMax;
}
	
void shakeData::loadProcessed(string filename)
{

}

void shakeData::saveProcessed(string filename)
{
	ofXML temp;
	temp.setCurrentTag(";");
	temp.newCurrentTag("points");
	for(unsigned int i=0; i<uData.size(); i++){
		temp.setCurrentTag(";points");
		temp.newCurrentTag("point");
		temp.addAttribute("pos",ofToString(uData[i].pos));
		temp.addAttribute("time",ofToString(uData[i].dTime,0));
		cout << "Running: " << i << endl;
	}
	temp.writeFile(file+"_p.xml");
}
	
void shakeData::load(string filename)
{

}
	
dataPoint & shakeData::operator[](int i)
{
	return uData[i];
}

int shakeData::size()
{
	return uData.size();
}

bool shakeData::nextSegment( CML::uunit & p, CML::uunit & v, CML::uint8 & t)
{
	bool ret=false;
	if(index<uData.size()){
		pos+=uData[index].pos;
		vel+=uData[index].vel;
		t=char(uData[index].dTime);
		index++;
		p=pos;
		v=vel;
		ret=true;
	}
	return ret;
}

void shakeData::draw(int x, int y, double w, double h)
{
	double ypos=0;
	for(unsigned int i=1; i<uData.size()&&uData.size(); i++){
		double yPrev=ypos;
		ypos+=uData[i].pos;
		ofLine(x+(w/uData.size())*(i-1),y+h*yPrev/maxDisp,x+(w/uData.size())*i,y+h*ypos/maxDisp);
	}
}
	
void shakeData::reset()
{
	pos=0;
	vel=0;
	index=0;
}