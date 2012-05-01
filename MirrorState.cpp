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

void MirrorState::processTime(Director *director, const int64 &currentTickCount)
{
	duration = director->getDuration(currentTickCount);
}

void MirrorState::processKeyEvent(Director *director, const int &key)
{
}

void MirrorState::processMouseEvent(Director *director, const Point &mousePos)
{
}

// void MirrorState::processOSC(Director *director, 
// 	const map<string, osc::ReceivedMessage *> &messageMap)
// {
// }

void MirrorState::processAnimation(Director *director)
{
	if (!director->getCanRecord()) {
		director->setCanRecord(true);
		return;
	}
	const int center = numPhotos / 2 + numPicturesInPainting.width / 2;
	Picture& picture = *director->getPictureAt(center);
	Mat flipImage;
	flip(director->getRecordImage(), flipImage, 1);
	picture.setContent(flipImage);

	if (!director->isAllAnimationEnd())
		return;

	ImagesPool *imagesPool = director->getImagesPool();
	switch (showImageFlag) {
	case 0:
		if (duration > 2.0f) {
			showImageFlag = 1;

			Picture& picture = *director->getPictureAt(center-1);
			picture.setAnimation(FlipAnimationEnum, imagesPool->starPhotos[center-1],
				imagesPool->starPhotos[(center-1+3) % numPhotos]);

			FlipAnimation *flipAnimation = static_cast<FlipAnimation *>(picture.getAnimation());
			flipAnimation->setProperties(flips[0]);

			int64 enableAnimationTick = static_cast<int64>(director->getCurrentTime());
			picture.setEnableAnimationTime(enableAnimationTick);
		}
		break;
	case 1:
		if (duration > 3.0f) {
			showImageFlag = 2;
			Picture& picture = *director->getPictureAt(center+1);
			picture.setAnimation(FlipAnimationEnum, imagesPool->starPhotos[center+1],
				imagesPool->starPhotos[(center+1+3) % numPhotos]);

			FlipAnimation *flipAnimation = static_cast<FlipAnimation *>(picture.getAnimation());
			flipAnimation->setProperties(flips[4]);

			picture.setEnableAnimationTime(director->getCurrentTime());
		}
		break;
	default:
		if (duration > 4.0f) {
			director->setCanRecord(false);
			director->setAnimationState(new FadeinState);
		}
		break;
	}
}
