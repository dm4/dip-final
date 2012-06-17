#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void BeforeGameState::processTime(Director *director, const int64 &currentTickCount)
{

}

void BeforeGameState::processKeyEvent(Director *director, const int &key)
{
}

void BeforeGameState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

 void BeforeGameState::processOSC(Director *director, const map<string, osc::ReceivedMessage *> &messageMap)
 {
 #if SIMULATOR == 0
 	Point2f eyePos(0, 0);
 	eyePos = director->getEyePosWithOSCMessage(messageMap);
 	eyePosIndex = director->get1DEyePos(eyePos);
 #endif
 }

void BeforeGameState::processAnimation(Director *director) {
    if (!isInitialized) {
        Mat sky = imread("Words/1/17.jpg");
        for (int i = 0; i < 6; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(sky);
			hasMole[i] = true;
        }
        for (int i = 6; i < 14; i++) {
            Picture& picture = *director->getPictureAt(i);
            Rect frame = picture.getFrame();
            picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
        }
		Picture& picture = *director->getPictureAt(6);
        CvFont font;
        cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
        cvPutText(new IplImage(picture), "Start", cvPoint(0, 60), &font, cvScalar(255, 255, 255, 0));
        cvPutText(new IplImage(picture), "  ->", cvPoint(0, 90), &font, cvScalar(255, 255, 255, 0));
        cvPutText(new IplImage(picture), "  ->", cvPoint(0, 120), &font, cvScalar(255, 255, 255, 0));
        isInitialized = true;
    }

    // whack moles to start the game
	if (eyePosIndex < 6 && hasMole[eyePosIndex]) {
        Picture& picture = *director->getPictureAt(eyePosIndex);
        picture.setFocus(true);
        hasMole[eyePosIndex] = false;
		picture.setAnimation(
				FadeoutAnimationEnum,
				picture,
                NULL
                );
// 		FadeoutAnimation *ani = static_cast<FadeoutAnimation *>(picture.getAnimation());
// 		ani->setCanReverse(true);
		picture.setEnableAnimationTime(director->getCurrentTime());
	}

    // check all moles done
    bool isDone = true;
    for (int i = 0; i < 6; i++) {
        Picture& picture = *director->getPictureAt(i);
        if (hasMole[i] || !picture.getAnimation()->animationEnded()) {
            isDone = false;
            break;
        }
    }
    if (isDone) {
        director->setAnimationState(new HumanState);
        director->setStartTickCount();
    }
}
