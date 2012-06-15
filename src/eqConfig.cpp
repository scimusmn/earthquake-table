#include "eqConfig.h"

static configuration config;

configuration & cfg(){
	return config;
};

void configuration::readGeneralConfig(string cfgFile){
	ifstream config(ofToDataPath(cfgFile).c_str());
	while (config.peek()!=EOF) {
		string nextLine;
		getline(config, nextLine);
		vector<string> token=ofSplitString(nextLine, "=\r\n");
		if(token.size()){
			if(token[0]=="QUAKE_DIR"){
				eqDir=ofToDataPath(token[1]);
			}
			else if(token[0]=="VERBOSE"){
				verbose=ofToInt(token[1]);
			}
			else if(token[0]=="TIMEOUT"){
				timeout=ofToInt(token[1]);
			}
			else if(token[0]=="HOME_CURRENT"){
				homeCurrent=ofToInt(token[1]);
			}
			else if(token[0]=="SCALE_ALL_QUAKES"){
				scaleToAll=ofToInt(token[1]);
			}
			else if(token[0]=="SMOOTHING"){
				smoothing=ofToInt(token[1]);
			}
			else if(token[0]=="AUTO_TRIM"){
				autoTrim=ofToInt(token[1]);
			}
			else if(token[0]=="NO_TABLE"){
				noTable=ofToInt(token[1]);
			}
			else if(token[0]=="MAX_DISPLACEMENT"){
				maxDisp=ofToInt(token[1]);
			}
			else if(token[0]=="FULLSCREEN"){
				ofSetFullscreen(ofToInt(token[1]));
			}
			else if(token[0]=="SAMPLE_TIME"){
				sampleTime=ofToFloat(token[1]);
			}
		}
	}
	config.close();
}