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
    Mat red = imread("Pics/red.png");
    Mat blue = imread("Pics/blue.png");
	if (!isInitialized) {
		// 		director->playMusic("Musics/thunder.wav");
		// 		director->takePhoto(photo);

        // set mole
		for (int i = 0; i < 6; ++i) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(molePhoto);
			hasMole[i] = true;
		}

        // set other pic
        for (int i = 7; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(blue);
        }
		srand(time(NULL));
		isInitialized = true;

        // init black for pic 6
        Picture& picture = *director->getPictureAt(6);
        Rect frame = picture.getFrame();
        picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
		black = cvCloneImage(new IplImage(picture));

        // init score
        (new Score())->score = 0;
		Picture& scorePic = *director->getPictureAt(6);
        setScore(scorePic, 0);
	}

    // reborn mole
	if( rand()%10 > 4){
		int mole = rand() % 6;
        Picture& molePic = *director->getPictureAt(mole);
        if (molePic.getAnimation()->animationEnded()) {
            molePic.setContent(molePhoto);
            molePic.setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
            molePic.setEnableAnimationTime(director->getCurrentTime());
            hasMole[mole] = true;
        }
	}

	Picture& picture = *director->getPictureAt(eyePosIndex);
	picture.setFocus(true);
	if (eyePosIndex < 6 && picture.getAnimation()->animationEnded()) {
		if (hasMole[eyePosIndex]) {
			Score s;
			s.score++;
			hasMole[eyePosIndex] = false;
			Picture& scorePic = *director->getPictureAt(6);
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
