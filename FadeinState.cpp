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

void FadeinState::processAnimation(Director *director)
{
    Mat molePhoto = imread("Pics/Diglett.png");
    if (!isInitialized) {
        // 		director->playMusic("Musics/thunder.wav");
        // 		director->takePhoto(photo);

        for (int i = 0; i < numPhotos; ++i) {
            Picture& picture = *director->getPictureAt(i);
            picture.setContent(molePhoto);
        }
        srand(time(NULL));
        isInitialized = true;
    }

    if( rand()%10 > 7){
        int mole = rand()%44;
        Picture& molePic = *director->getPictureAt(mole);
        if (molePic.getAnimation()->animationEnded()) {
            molePic.setContent(molePhoto);
            molePic.setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
            molePic.setEnableAnimationTime(director->getCurrentTime());
        }
    }

    Picture& picture = *director->getPictureAt(eyePosIndex);
    picture.setFocus(true);
    if (picture.getAnimation()->animationEnded()) {
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
