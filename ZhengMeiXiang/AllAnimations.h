/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ALL_ANIMATIONS_H
#define ALL_ANIMATIONS_H

#include "IdleAnimation.h"
#include "FlipAnimation.h"
#include "FadeoutAnimation.h"

/*!
 * \brief
 * the enumeration of animation.
 * 
 * If you create a new animation, you must add it to the enumeration and 
 an animation factory create the animation.
 * 
 */
typedef enum AnimationEnum {
	IdleAnimationEnum = 0,
	FlipAnimationEnum = 1,
	FadeoutAnimationEnum = 2
} AnimationEnum;

#endif // ALL_ANIMATIONS_H