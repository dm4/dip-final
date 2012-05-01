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


Mat resizeMolePhoto;
const Size pictureDefaultSize(110, 150);
void FadeinState::processAnimation(Director *director)
{
	if (photo.empty()) {
		// 		director->playMusic("Musics/thunder.wav");
		director->takePhoto(photo);

		for (int i = 0; i < numPhotos; ++i) {
			Picture& picture = *director->getPictureAt(i);
			picture.setContent(photo);
		}
		Mat molePhoto = imread("Pics/Diglett.png");
		cv::resize(molePhoto, resizeMolePhoto, pictureDefaultSize);
		srand(time(NULL));
	}

	if( rand()%10 > 7){
		int mole = rand()%44;
		Picture& molePic = *director->getPictureAt(mole);
		molePic.setContent(resizeMolePhoto);
		molePic.setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
		molePic.setEnableAnimationTime(director->getCurrentTime());
	}
	Picture& picture = *director->getPictureAt(eyePosIndex);
	picture.setFocus(true);
	if (!picture.getAnimation()->animationEnded())
		return;

	picture.setAnimation(
			FadeoutAnimationEnum,
			picture,
			photo
			);
	// 	FlipAnimation *flipAnimation = static_cast<FlipAnimation *>(picture.getAnimation());
	// 	flipAnimation->setCanReverse(true);
	// 	flipAnimation->setProperties(flips[rand() % 8]);
	FadeoutAnimation *ani = static_cast<FadeoutAnimation *>(picture.getAnimation());
	ani->setCanReverse(true);
	picture.setEnableAnimationTime(director->getCurrentTime());
}
