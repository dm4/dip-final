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
		// restore frame
		Mat painting = director->getPainting();
		Picture& picture = *director->getPictureAt(0);
		picture.setFrame(painting(originFrame), originFrame);

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
		Picture& picture = *director->getPictureAt(0);

		// save and set frame
		originFrame = picture.getFrame();
		bigFrame.x = 0;
		bigFrame.y = 0;
		bigFrame.width = 300;
		bigFrame.height = 300;
		picture.setFrame(painting(bigFrame), bigFrame);

		// set white color
		picture.setContent(Mat(picture.size(), picture.getType(), Scalar(255, 255, 255)));

		// add text
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(new IplImage(picture), "Sit to Start", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));

		director->setCanRecord(false);
		isInitialized = true;
    }
}
