/*
 *  aAnimPlayer.cpp    mackey DDA Pratt
 *  one or more instances owned by any Node with animations
 */

#include "aAnimPlayer.h" //

aAnimPlayer::aAnimPlayer() {}; //default constructor

aAnimPlayer::aAnimPlayer(sAnimData _theAnimData) {
    theAnimData=_theAnimData;
    
    imgfileprefix = _theAnimData.PREFIX;
	imgfilesuffix = _theAnimData.EXTEN;
    animimgsdirectory = _theAnimData.IMGSDIR;
    //now look into this directory and count the files
    numFrames = getFileCountInDirectory(); //see below
    
	printf("aAnimPlayer for %s starting to load the images \n",animimgsdirectory.c_str());
    
	//load up the vector with ofImage objects
	ofImage img;
	for (int i=0; i<numFrames; i++) {
		img.loadImage(makePath(i));
		sequence.push_back(img);
	}
    frameCount=0;
    
    centered = _theAnimData.ALIGN;
    locXY.set(ofPoint(_theAnimData.X,_theAnimData.Y));
    
    dimensions[0] = _theAnimData.X;
    dimensions[1] = _theAnimData.Y;
    dimensions[2] = sequence[0].width; //use one of the imgs
    dimensions[3] = sequence[0].height;


}

//TODO: probably will need these for all assets~~~~~~~~~~~~~~~~~~~~~~~~~~
//void aAnimPlayer::arrived() { //called from Node via Scene parent when it begins
//    startTimer(); //timer used to advance the frame number
//}
//void aAnimPlayer::departed() { //called from Scene parent when leaving
//    
//}

void aAnimPlayer::update() {
    if (timeHasExpired()) advanceTheFrame();
    //needs work, if you want "frame rate" of anim to be slaved to this timer; probably via an event
}
//TODO: DEBUG not finding the directory?

void aAnimPlayer::draw(ofPoint _nodeLoc) { //from Node draw()
    //use Translate if (centered)
	//sequence[frameCount].draw(locXY);
    sequence[frameCount].draw(_nodeLoc);
    
    /* to be a slave to the draw loop (and thus, the project's real-life frame rate)*/
    frameCount++;
    if (frameCount >= numFrames) frameCount=0; //looping
    
    
}
//----clockwork-------------------------------------------------------
void aAnimPlayer::advanceTheFrame() {
    frameCount++;
    if (frameCount >= numFrames) frameCount=0; //looping
    startTimer(); //reset
}

void aAnimPlayer::startTimer() {
    startTime = ofGetElapsedTimeMillis();
}

bool aAnimPlayer::timeHasExpired() {
	if (ofGetElapsedTimeMillis()+startTime > INTERVAL) {
		return true;
	} else {
		return false;
	}
}
//--------------------------------------------------------------------

//----file work........................................................
string aAnimPlayer::makePath(int i) { //assumes EXTEN includes a period
    filename = "";
	if (i<10) {
		filename = animimgsdirectory+"/"+imgfileprefix + "00" + ofToString(i) + imgfilesuffix;
	} else {
        if (i<100) {
            filename = animimgsdirectory+"/"+imgfileprefix + "0" + ofToString(i) + imgfilesuffix;
        } else {
            filename = animimgsdirectory+"/"+imgfileprefix + ofToString(i) + imgfilesuffix;
        }
    }
	if (i==0) printf("aAnimPlayer::makePath made > %s < \n",filename.c_str() );
	
	return filename.c_str();
}

int aAnimPlayer::getFileCountInDirectory() {
    string absdatapath;
    ofFile where(ofToDataPath(animimgsdirectory));//subdir in data ƒ
    absdatapath = where.getAbsolutePath();
    //absdatapath is complete path on your HD into the data ƒ
    //printf("The entire path to animation imgs fldr = %s\n",absdatapath.c_str());
    
    homedir.listDir(absdatapath); //homedir is an instance of ofDirectory
    //this is an array of all the file names at that path
    int n = (int)homedir.size();
    printf("The number of files in this fldr <%s> is %i\n",absdatapath.c_str(),n);
    return n;
}
//....................................................................


