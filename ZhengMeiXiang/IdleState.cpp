#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>

using namespace std;
using namespace cv;

void IdleState::processTime(Director *director, const int64 &currentTickCount)
{

}

void IdleState::processKeyEvent(Director *director, const int &key) {
    if (key == 'a') {
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
 			director->setAnimationState(new BeforeGameState);
 			director->setStartTickCount();
 			director->getOSCListener()->clearMessageWithType("/interactionState");
 		}
 	}
 }

void IdleState::processAnimation(Director *director)
{
    if (!isInitialized) {
		Picture& picture = *director->getPictureAt(4);
		Rect frame = picture.getFrame();
		picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(new IplImage(picture), "Sit to Start", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
        isInitialized = true;
    }

	director->setCanRecord(false);
}
