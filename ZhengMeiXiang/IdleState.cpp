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
		// init state
		dao = 0;
		isFadein = true;

		// set to black
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(Mat::zeros(
				picture.getFrame().width,
				picture.getFrame().height,
				picture.getType()
			));
		}

		// Read setting
		ifstream ifs("idle-settings.txt");
		if (ifs.bad()) {
			cerr << "Read the setting file: idle-settings.txt error" << endl;
		}
		Mat painting = director->getPainting();
		director->pictures.clear();
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			Rect rect;
			ifs >> rect.x >> rect.y >> rect.width >> rect.height;
			//printf("test %d: %d, %d, %d, %d\n", i, rect.x, rect.y, rect.width, rect.height);
			//picture.setFrame(painting(rect), rect);
			director->pictures.push_back(Picture(painting(rect), rect));
		}

		// set first dao image
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			char filename[30];
			sprintf(filename, "Words/%d/%d.jpg", dao + 1, i + 1);
			Mat word = imread(filename);
			picture.setContent(word);
		}

		// set first fadein line animation
		for (int i = 0; i < numPhotos; i++) {
			Picture& picture = *director->getPictureAt(i);
			IplImage *start = cvCloneImage(new IplImage(picture));
			picture.setAnimation(
				FadeinAnimationEnum,
				start,
				NULL
			);
			picture.setEnableAnimationTime(director->getCurrentTime());
		}

		director->setCanRecord(false);
		isInitialized = true;
    }

	// check all moles done
    bool isDone = true;
    for (int i = 0; i < numPhotos; i++) {
        Picture& picture = *director->getPictureAt(i);
        if (!picture.getAnimation()->animationEnded()) {
            isDone = false;
            break;
        }
    }
    if (isDone) {
		if (isFadein) {
			// set fadeout animation
			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				IplImage *start = cvCloneImage(new IplImage(picture));
				picture.setAnimation(
					FadeoutAnimationEnum,
					start,
					NULL
				);
				picture.setEnableAnimationTime(director->getCurrentTime());
			}
			isFadein = false;
		}
		else {
			dao++;
			dao %= 6;
			// set dao image
			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				char filename[30];
				sprintf(filename, "Words/%d/%d.jpg", dao + 1, i + 1);
				Mat word = imread(filename);
				picture.setContent(word);
			}

			// set first fadein animation
			for (int i = 0; i < numPhotos; i++) {
				Picture& picture = *director->getPictureAt(i);
				IplImage *start = cvCloneImage(new IplImage(picture));
				picture.setAnimation(
					FadeinAnimationEnum,
					start,
					NULL
				);
				picture.setEnableAnimationTime(director->getCurrentTime());
			}

			isFadein = true;
		}
    }
}
