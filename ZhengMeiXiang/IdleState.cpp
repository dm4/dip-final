#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void IdleState::processTime(Director *director, const int64 &currentTickCount)
{
}

void IdleState::processKeyEvent(Director *director, const int &key) {
    if (key == 'a') {
		// read origin setting
		Mat painting = director->getPainting();
		ifstream ifs("my-settings.txt");
		if (ifs.bad()) {
			cerr << "Read the setting file: my-settings.txt error" << endl;
		}
		int numRead = 0;
		while(ifs.good() && numRead < numPhotos) {
			Picture& picture = *director->getPictureAt(numRead);
			Rect rect;
			ifs >> rect.x >> rect.y >> rect.width >> rect.height;
			picture.setFrame(painting(rect), rect);
			++numRead;
		}

		// next state
        director->setAnimationState(new BeforeGameState);
        director->setStartTickCount();
    }
}

void IdleState::processMouseEvent(Director *director, const Point &mousePos)
{
}

 void IdleState::processOSC(Director *director, 
 	const map<string, osc::ReceivedMessage *> &messageMap)
 {
 	map<string, osc::ReceivedMessage *>::const_iterator 
 		messageMapIter = messageMap.find("/interactionState");
 	if (messageMapIter != messageMap.end()) {
 		osc::ReceivedMessage *oscMessage = messageMapIter->second;
 		osc::ReceivedMessageArgumentStream oscArgStream = oscMessage->ArgumentStream();
 		bool interactionState = false;
 		oscArgStream >>  interactionState >> osc::EndMessage;
 		if (interactionState) {
			// restore frame
			Mat painting = director->getPainting();
			Picture& picture = *director->getPictureAt(0);
			picture.setFrame(painting(originFrame), originFrame);

			// next state
			director->setAnimationState(new BeforeGameState);
 			director->setStartTickCount();
 			director->getOSCListener()->clearMessageWithType("/interactionState");
 		}
 	}
 }

void IdleState::processAnimation(Director *director)
{
    if (!isInitialized) {
		Mat painting = director->getPainting();

		// Read setting
		ifstream ifs("idle-settings.txt");
		if (ifs.bad()) {
			cerr << "Read the setting file: idle-settings.txt error" << endl;
		}
		int numRead = 0;
		//while(ifs.good() && numRead < numPhotos) {
		director->pictures.clear();
		for (int i = 0; i < 20; i++) {
			Picture& picture = *director->getPictureAt(i);
			Rect rect;
			ifs >> rect.x >> rect.y >> rect.width >> rect.height;
			printf("test %d: %d, %d, %d, %d\n", i, rect.x, rect.y, rect.width, rect.height);
			//picture.setFrame(painting(rect), rect);
			director->pictures.push_back(Picture(painting(rect), rect));
			++numRead;
		}

		for (int i = 0; i < 20; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(Mat(picture.size(), picture.getType(), Scalar(255, 0, 0, 0)));
		}

		director->setCanRecord(false);
		isInitialized = true;
    }
}
