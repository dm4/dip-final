/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>

using namespace std;
using namespace cv;

void InitState::processTime(Director *director, const int64 &currentTickCount)
{

}

void InitState::processKeyEvent(Director *director, const int &key)
{

}

void InitState::processMouseEvent(Director *director, const Point &mousePos)
{

}

void InitState::processOSC(Director *director, 
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
			director->setAnimationState(new MirrorState);
			director->setStartTickCount();
			director->getOSCListener()->clearMessageWithType("/interactionState");
		}
	}
}

void InitState::processAnimation(Director *director)
{
	if (needToPlayDefaultMusic) {
		director->playMusic("Musics/Music.wav");
		needToPlayDefaultMusic = false;
	}
	for (int i = 0; i < numPhotos; ++i) {
		Picture& picture = *director->getPictureAt(i);
		Rect frame = picture.getFrame();
		picture.setContent(Mat::zeros(frame.width, frame.height, picture.getType()));
	}
	
	director->setCanRecord(false);

#if SIMULATOR == 1
	director->setAnimationState(new MirrorState);
#endif
}