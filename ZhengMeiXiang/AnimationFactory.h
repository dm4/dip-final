/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ANIMATION_FACTORY_H
#define ANIMATION_FACTORY_H

#include <iostream>
#include "AllAnimations.h"

using namespace std;

/*!
 * \brief
 * An animation factory.
 * 
	We use a simple factory pattern to create animation.
 */
class AnimationFactory
{
public:
	/*!
	*  \brief create an animation you want.
		\param animationEnum specify which animation you want to apply.
		\param startImg shown image after the animation starts.
		\param endImg shown image after the animation is end.
	*/
	static Animation* createAnimation(const AnimationEnum &animationEnum,
		const cv::InputArray &startImg, const cv::InputArray &endImg) 
	{
		switch(animationEnum) {
		case IdleAnimationEnum:
			return new IdleAnimation(startImg.getMat(), endImg.getMat());
			break;
		case FlipAnimationEnum:
			return new FlipAnimation(startImg.getMat(), endImg.getMat());
			break;
		case FadeoutAnimationEnum:
			return new FadeoutAnimation(startImg.getMat(), endImg.getMat());
			break;
		default:
			cerr << "AnimationFactory cannot create the Animation: " << animationEnum << endl;
			return new IdleAnimation(startImg.getMat(), endImg.getMat());
			break;
		}
	}
};

#endif // ANIMATION_FACTORY_H