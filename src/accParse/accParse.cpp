#include "accParse.h"
#include "../eqConfig.h"

dataPoint::dataPoint(double a, double deltat)
{
	acc=vel=pos=0;
	dTime=deltat*1000;
	acc=a;
}

dataPoint::dataPoint(double a, double v, double p, double t)
{
	acc=a, vel=v, pos= p, dTime=t;
}

shakeData::shakeData()
{
	index=vel=pos=0;
	//maxDisp=10000;
	bCaptured=false;
}
	
shakeData::~shakeData()
{
	uData.clear();
}
	
void shakeData::getPoint(double acc, double deltat)
{
	uData.push_back(dataPoint(acc,deltat));
}

void highPassFilter(vector<double> & in,double sampFreq,double fCutoff=.1){
	double dt=1/sampFreq;
	double RC=1/(2*M_PI*fCutoff);
	double alpha=RC/(RC+dt);
	vector<double> out=in;
	out[0]=in[0];
	for(unsigned int i=0; i<in.size(); i++){
		out[i]=alpha*out[i-1] + alpha*(in[i]-in[i-1]);
	}
	in=out;
}

void lowPassFilter(vector<double> & in,double sampFreq,double fCutoff=25){
	double dt=1/sampFreq;
	double RC=1/(2*M_PI*fCutoff);
	double alpha=dt/(RC+dt);
	vector<double> out=in;
	out[0]=in[0];
	for(unsigned int i=0; i<in.size(); i++){
		out[i]=alpha*in[i] + (1-alpha)*out[i-1];
	}
	in=out;
}

void bandpassFilter(vector<double> & in, double sampFreq,int order = 1, double lCutoff=.1,double hCutoff=25){
	while(order){
		highPassFilter(in,sampFreq,lCutoff);
		lowPassFilter(in,sampFreq,hCutoff);
		order--;
	}
}

void linearRegression(vector<double> & in, double & a1, double & a0)
{
	ofPoint sum,avg;
	double sumXY=0,sumXX=0;
	for(unsigned int i=0; i<in.size(); i++){
		sum.x+=i;
		sum.y+=in[i];
		sumXY+=i*in[i];
		sumXX+=i*i;
	}

	avg.x=sum.x/in.size();
	avg.y=sum.y/in.size();

	a1=(in.size()*sumXY-sum.x*sum.y)/(in.size()*sumXX-sum.x*sum.x);
	a0=avg.y-a1*avg.x;
}

void shakeData::loadSMC(string filename)
{
	file=filename;
	ifstream input(ofToDataPath(file).c_str());
	string buffer;
	sampFreq=200.;
	bool tagOpen=false;
	int pipeCount=0;
	vector<double> val;
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
				//uData.push_back(dataPoint(ofToFloat(num),1/sampFreq));
				val.push_back(ofToFloat(num));
			}
		}
	}
	input.close();
	for(unsigned int i=0; i<val.size(); i++){
		uData.push_back(dataPoint(val[i],1/sampFreq));
	}
	processData();
}

enum KNETresults {
	NOT_FOUND, FREQ, SCALE_AMOUNT, MAX_ACCEL
};

static string heads[]={"Sampling Freq(Hz)","Scale Factor","Max. Acc. (gal)"};

KNETresults getVal(string in,string & out)
{
	KNETresults ret;
	out="";
	for(int i=0; i<3; i++){
		size_t found=in.find(heads[i]);
		if(found!=string::npos){
			out=in.substr(in.find_first_not_of(' ',found+heads[i].length()));
			ret=KNETresults(i+1);
		}
	}
	return ret;
}

void shakeData::loadKNET(string filename)
{
	file=filename;
	ifstream input(ofToDataPath(file).c_str());
	string buffer;
	int lineCount=0;
	double scale;
	double averg=0;
	vector<double> val;
	while(lineCount<18){
		getline(input, buffer);
		string result;
		KNETresults rslt=getVal(buffer,result);
		switch(rslt){
			case FREQ:
				sampFreq=ofToInt(result);
				break;
			case SCALE_AMOUNT:
				scale=ofToInt(result.substr(0,result.find_first_of("(")));
				scale/=ofToInt(result.substr(result.find_last_of("/")+1));
				break;
			case MAX_ACCEL:
				break;
			default:
				break;
		}
		lineCount++;
	}
	while(!input.eof()){
		getline(input, buffer);
		for(int i=0; i<8; i++){
			if(buffer.length()>(i+1)*9){
				val.push_back(ofToInt(buffer.substr(i*9,9)));
			}
		}
	}
	input.close();
	double a1,a0;
	linearRegression(val,a1,a0);
	for(unsigned int i=0; i<val.size(); i++){
		val[i]-=a0+a1*i;
		val[i]*=scale;
	}
	bandpassFilter(val,sampFreq,3);
	for(unsigned int i=0; i<val.size(); i++){
		uData.push_back(dataPoint(val[i],1/sampFreq));
	}
	processData();
}

void shakeData::loadDAT(string filename)
{
	file=filename;
	ifstream input(ofToDataPath(file).c_str());
	string buffer;
	int lineCount=0;
	getline(input,buffer);
	double lastTime=0;
	while(!input.eof()){
		getline(input, buffer);
		vector<string> spl=ofSplitString(buffer," \t");
		if(spl.size()){
			float time=ofToFloat(spl[0]);
			uData.push_back(dataPoint(ofToFloat(spl[1]),time-lastTime));
			sampFreq=1/(time-lastTime);
			lastTime=time;
		}
	}
	input.close();
	processData();
}

void shakeData::loadCOSMOS(string filename)
{
	file=filename;
	ifstream input(ofToDataPath(file).c_str());
	string buffer;
	vector<double> val;
	int burn=45;
	while(burn--){
		getline(input, buffer);
	}
	getline(input, buffer);
	int numPoints=ofToInt(buffer.substr(0,buffer.find_first_of(' ',4)));
	sampFreq=1/ofToFloat(buffer.substr(47,4));
	string format=buffer.substr(buffer.find_last_of("(")+1,buffer.find_last_of(")")-(buffer.find_last_of("(")+1));
	cout <<numPoints << " of data, at " << sampFreq << " intervals in " << format << endl;
	vector<string> spl=ofSplitString(format,"f.");
	int pointsPerLine=ofToInt(spl[0]);
	int space=ofToInt(spl[1]);
	while(val.size()<numPoints){
		getline(input, buffer);
		for(unsigned int i=0; i<pointsPerLine&&val.size()<numPoints; i++){
			val.push_back(ofToFloat(buffer.substr(i*space,space)));
		}
	}
	for(unsigned int i=0; i<val.size(); i++){
		uData.push_back(dataPoint(val[i],1/sampFreq));
	}
	input.close();
	processData();
	
}
	
void shakeData::loadUnprocessed(string filename)
{
	string ext=filename.substr(filename.find_last_of('.'));
	if(ext==".smc") loadSMC(filename);
	if(ext==".dat") loadDAT(filename);
	if(ext==".V2") loadCOSMOS(filename);
	if(ext==".EW"||ext==".NS") loadKNET(filename);
}

void shakeData::processData()
{
	double netPos=uData[0].pos=0;
	maxNet=netPos;
	double vel=0;
	for(unsigned int i=0; i<uData.size()&&uData.size(); i++){
		double time=1/sampFreq;
		uData[i].time=i*time;
		uData[i].vel=vel+=uData[i].acc*time;
		uData[i].pos=(netPos+=vel*time);
		maxNet=max(abs(netPos),maxNet);
	}
	cout << maxNet << " max displacement in cm" << endl;
	if(cfg().autoTrim){
		vector<dataPoint>::iterator start=uData.begin();
		vector<dataPoint>::iterator end=start;
		while(abs((*end).pos)<maxNet/150) end++;
		uData.erase(start,end);

		//trimming the back side
		//end=uData.end();
		//start=end;
		while(abs(uData.back().pos)<maxNet/100) uData.pop_back();
		//uData.erase(start,end);
	}
	maxNet/=cfg().maxDisp;
	for(unsigned int i=0; i<uData.size(); i++){
		uData[i].pos/=maxNet;
		uData[i].vel/=(maxNet*100);
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
		vel=uData[index].vel;
		pos=uData[index].pos;
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
	if(!bCaptured){
		int inc = uData.size()/(w*2);
		for(unsigned int i=inc; i<uData.size()&&uData.size(); i+=inc){
			double yPrev=ypos;
			ypos=uData[i].pos;
			ofLine(x+(w/uData.size())*(i-inc),y+h/2-(h/2)*yPrev/cfg().maxDisp,x+(w/uData.size())*i,y+h/2-(h/2)*ypos/cfg().maxDisp);
		}
		shot.allocate(w,h,OF_IMAGE_COLOR);
		shot.grabScreen(x,y,w,h);
		bCaptured=true;
	}
	else ofSetColor(255,255,255),shot.draw(x,y,w,h);
}
	
void shakeData::reset()
{
	pos=0;
	vel=0;
	index=0;
}