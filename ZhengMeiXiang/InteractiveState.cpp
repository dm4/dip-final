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

void InteractiveState::processTime(Director *director, const int64 &currentTickCount)
{

}

void InteractiveState::processKeyEvent(Director *director, const int &key)
{
}

void InteractiveState::processMouseEvent(Director *director, const Point &mousePos)
{
#if SIMULATOR == 1
	eyePosIndex = mousePos.y * numPicturesInPainting.width + mousePos.x;
#endif
}

void InteractiveState::processOSC(Director *director, 
	const map<string, osc::ReceivedMessage *> &messageMap)
{
#if SIMULATOR == 0
	Point2f eyePos(0, 0);
	eyePos = director->getEyePosWithOSCMessage(messageMap);
	eyePosIndex = director->get1DEyePos(eyePos);
#endif
}

void InteractiveState::processAnimation(Director *director)
{
	if (photo.empty()) {
		director->playMusic("Musics/thunder.wav");
		director->takePhoto(photo);
		
		for (int i = 0; i < numPhotos; ++i) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(photo);
		}
	}

	Picture& picture = *director->getPictureAt(eyePosIndex);
	if (!picture.getAnimation()->animationEnded())
		return;

	picture.setAnimation(FlipAnimationEnum, photo, 
		director->getImagesPool()->starPhotos[0]);
	FlipAnimation *flipAnimation = static_cast<FlipAnimation *>(picture.getAnimation());
	flipAnimation->setCanReverse(true);
	flipAnimation->setProperties(flips[rand() % 8]);

	picture.setEnableAnimationTime(director->getCurrentTime());
}