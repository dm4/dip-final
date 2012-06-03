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
        director->setAnimationState(new FadeinState);
        director->setStartTickCount();
    }
}

void IdleState::processMouseEvent(Director *director, const Point &mousePos)
{

}

// void IdleState::processOSC(Director *director, 
// 	const map<string, osc::ReceivedMessage *> &messageMap)
// {
// 	map<string, osc::ReceivedMessage *>::const_iterator 
// 		messageMapIter = messageMap.find("/interactionState");
// 	if (messageMapIter != messageMap.end()) {
// 		osc::ReceivedMessage *oscMessage = messageMapIter->second;
// 		osc::ReceivedMessageArgumentStream oscArgStream = oscMessage->ArgumentStream();
// 		bool interactionState = false;
// 		oscArgStream >>  interactionState >> osc::EndMessage;
// 		if (interactionState) {
// 			director->setAnimationState(new MirrorState);
// 			director->setStartTickCount();
// 			director->getOSCListener()->clearMessageWithType("/interactionState");
// 		}
// 	}
// }

void IdleState::processAnimation(Director *director)
{
    if (!isInitialized) {
		Picture& picture = *director->getPictureAt(0);
		Rect frame = picture.getFrame();
		picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(new IplImage(picture), "IdleState", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
        isInitialized = true;
    }

	director->setCanRecord(false);
}
