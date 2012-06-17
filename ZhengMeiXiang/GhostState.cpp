#include "AllAnimationStates.h"
#include "Director.h"
#include "Score.h"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace cv;

void GhostState::processTime(Director *director, const int64 &currentTickCount) {
}

void GhostState::processKeyEvent(Director *director, const int &key) {
}

void GhostState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

 void GhostState::processOSC(Director *director, const map<string, osc::ReceivedMessage *> &messageMap)
 {
 #if SIMULATOR == 0
 	Point2f eyePos(0, 0);
 	eyePos = director->getEyePosWithOSCMessage(messageMap);
 	eyePosIndex = director->get1DEyePos(eyePos);
 #endif
 }

void GhostState::processAnimation(Director *director)
{
	Mat molePhoto = imread("Pics/ghost_7.jpg");
    Mat red = imread("user_ghost.jpg");
    Mat blue = imread("Pics/blue.png");
	if (!isInitialized) {
		// 		director->playMusic("Musics/thunder.wav");
		// 		director->takePhoto(photo);

        // set mole
		for (int i = 0; i < 6; ++i) {
			//Picture& picture = *director->getPictureAt(i);
			//picture.setContent(molePhoto);
			hasMole[i] = 0;
		}

        // set other pic
        for (int i = 7; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			Rect frame = picture.getFrame();
            picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
        }
		srand((unsigned int)time(NULL));
		isInitialized = true;

        // init black for pic 6
        Picture& picture = *director->getPictureAt(6);
        Rect frame = picture.getFrame();
        picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
		black = cvCloneImage(new IplImage(picture));

        // init score
        Score::score = 5;
		Picture& scorePic = *director->getPictureAt(6);
        setScore(scorePic, Score::score);
	}

    // reborn mole
	if(rand() % 10 > 4){
		int mole = rand() % 6;
		bool canHit = (rand() % 100 > 50) ? true : false;
        Picture& molePic = *director->getPictureAt(mole);
        if (hasMole[mole] == 0 && molePic.getAnimation()->animationEnded()) {
			if (canHit) {
				molePic.setContent(molePhoto);
				hasMole[mole] = 1;
			}
			else {
				molePic.setContent(red);
				hasMole[mole] = 2;
			}
            molePic.setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
            molePic.setEnableAnimationTime(director->getCurrentTime());
        }
	}

	Picture& picture = *director->getPictureAt(eyePosIndex);
	picture.setFocus(true);
	if (eyePosIndex < 6 && picture.getAnimation()->animationEnded()) {
		if (hasMole[eyePosIndex]) {
			if (hasMole[eyePosIndex] == 1) {
				Score::score++;
			}
			else {
				Score::score--;
			}

			hasMole[eyePosIndex] = 0;
			Picture& scorePic = *director->getPictureAt(6);
            setScore(scorePic, Score::score);

			// next state
			if (Score::score <= 0) {
				director->setAnimationState(new EndGameState);
				director->setStartTickCount();
				return;
			}
			else if (Score::score >= 10) {
				director->setAnimationState(new HumanState);
				director->setStartTickCount();
				return;
			}
		}
		picture.setAnimation(
				FadeoutAnimationEnum,
				picture,
				photo
				);
		picture.setEnableAnimationTime(director->getCurrentTime());
	}
}

void GhostState::setScore(Picture &picture, int score) {
    IplImage *scoreImg = new IplImage(picture);
    cvCopy(black, scoreImg, NULL);
    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0, 0, 1, CV_AA);
    char scoreStr[10];
    sprintf(scoreStr, "%d", score);
    cvPutText(scoreImg, scoreStr, cvPoint(10, 130), &font, cvScalar(255, 255, 255, 0));
}
