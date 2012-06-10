#pragma once
#ifndef FADEOUTANIMATION_H
#define FADEOUTANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

class FadeoutAnimation : public Animation {
	public:
		FadeoutAnimation(cv::InputArray startImg, cv::InputArray endImg): Animation(startImg, endImg) {
			a = 0;
		}
		FadeoutAnimation(const CvArr *startImg, const CvArr *endImg): Animation(startImg, endImg) {
			a = 0;
		}
		void play(Picture &picture);
		int a;
};

#endif /* FADEOUTANIMATION_H */
