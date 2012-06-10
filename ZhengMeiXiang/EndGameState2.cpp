#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void EndGameState2::processTime(Director *director, const int64 &currentTickCount)
{
}

void EndGameState2::processKeyEvent(Director *director, const int &key)
{
}

void EndGameState2::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

 void EndGameState2::processOSC(Director *director, const map<string, osc::ReceivedMessage *> &messageMap)
 {
 #if SIMULATOR == 0
 	Point2f eyePos(0, 0);
 	eyePos = director->getEyePosWithOSCMessage(messageMap);
 	eyePosIndex = director->get1DEyePos(eyePos);
 #endif
 }

void EndGameState2::processAnimation(Director *director) {
	if (!isInitialized) {
		showText(*director->getPictureAt(4));
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setAnimation(
				FadeoutAnimationEnum,
				picture,
				NULL
				);
			picture.setEnableAnimationTime(director->getCurrentTime());
		}
        isInitialized = true;
		return;
    }

	// check all fade out
    bool isDone = true;
	for (int i = 0; i < numPhotos; i++) {
        Picture& picture = *director->getPictureAt(i);
        if (!picture.getAnimation()->animationEnded()) {
            isDone = false;
            break;
        }
    }
    if (isDone) {
		director->setAnimationState(new IdleState);
        director->setStartTickCount();
    }
}

void EndGameState2::showText(Picture& picture) {
	Rect frame = picture.getFrame();
	picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
	cvPutText(new IplImage(picture), "GameOver", cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
}