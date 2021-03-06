#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void EndGameState::processTime(Director *director, const int64 &currentTickCount)
{
}

void EndGameState::processKeyEvent(Director *director, const int &key)
{
}

void EndGameState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

 void EndGameState::processOSC(Director *director, const map<string, osc::ReceivedMessage *> &messageMap)
 {
 #if SIMULATOR == 0
 	Point2f eyePos(0, 0);
 	eyePos = director->getEyePosWithOSCMessage(messageMap);
 	eyePosIndex = director->get1DEyePos(eyePos);
 #endif
 }

void EndGameState::processAnimation(Director *director) {
	if (!isInitialized) {
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setFocus(false);
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
		director->setAnimationState(new EndGameState2);
        director->setStartTickCount();
    }
}