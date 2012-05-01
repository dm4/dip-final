/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include "Score.h"
#include <stdio.h>

using namespace std;
using namespace cv;

void FadeinState::processTime(Director *director, const int64 &currentTickCount)
{

}

void FadeinState::processKeyEvent(Director *director, const int &key)
{
}

void FadeinState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

// void FadeinState::processOSC(Director *director, 
// 	const map<string, osc::ReceivedMessage *> &messageMap)
// {
// #if SIMULATOR == 0
// 	Point2f eyePos(0, 0);
// 	eyePos = director->getEyePosWithOSCMessage(messageMap);
// 	eyePosIndex = director->get1DEyePos(eyePos);
// #endif
// }

void FadeinState::processAnimation(Director *director)
{
	Mat molePhoto = imread("Pics/Diglett.png");
	if (!isInitialized) {
		// 		director->playMusic("Musics/thunder.wav");
		// 		director->takePhoto(photo);

		for (int i = 0; i < numPhotos; ++i) {
			if (i % 11 == 0 || i % 11 == 10)
				continue;
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(molePhoto);
			hasMole[i] = true;
		}
		srand(time(NULL));
		isInitialized = true;

		Picture& picture = *director->getPictureAt(11);
		black = cvCloneImage(new IplImage(picture));
        (new Score())->score = 0;

		Picture& scorePic = *director->getPictureAt(0);
        setScore(scorePic, 0);
	}

	if( rand()%10 > 5){
		int mole = rand()%44;
		if (mole % 11 != 0 && mole % 11 != 10) {
			Picture& molePic = *director->getPictureAt(mole);
			if (molePic.getAnimation()->animationEnded()) {
				molePic.setContent(molePhoto);
				molePic.setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
				molePic.setEnableAnimationTime(director->getCurrentTime());
				hasMole[mole] = true;
			}
		}
	}

	Picture& picture = *director->getPictureAt(eyePosIndex);
	picture.setFocus(true);
	if (eyePosIndex % 11 != 0 && eyePosIndex % 11 != 10 && picture.getAnimation()->animationEnded()) {
		if (hasMole[eyePosIndex]) {
			Score s;
			s.score++;
			hasMole[eyePosIndex] = false;
			Picture& scorePic = *director->getPictureAt(0);
            setScore(scorePic, s.score);
// 			scorePic.setContent(scoreImg);
		}
		picture.setAnimation(
				FadeoutAnimationEnum,
				picture,
				photo
				);
		FadeoutAnimation *ani = static_cast<FadeoutAnimation *>(picture.getAnimation());
		ani->setCanReverse(true);
		picture.setEnableAnimationTime(director->getCurrentTime());
	}
}

void FadeinState::setScore(Picture &picture, int score) {
    IplImage *scoreImg = new IplImage(picture);
    cvCopy(black, scoreImg, NULL);
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
    char scoreStr[10];
    sprintf(scoreStr, "%d", score);
    cvPutText(scoreImg, scoreStr, cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
}
