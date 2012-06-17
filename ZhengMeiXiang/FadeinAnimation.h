#pragma once
#ifndef FADEINANIMATION_H
#define FADEINANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

class FadeinAnimation : public Animation {
	public:
		FadeinAnimation(cv::InputArray startImg, cv::InputArray endImg): Animation(startImg, endImg) {
			a = 0;
		}
		FadeinAnimation(const CvArr *startImg, const CvArr *endImg): Animation(startImg, endImg) {
			a = 0;
		}
		void play(Picture &picture);
		int a;
};

#endif /* FADEINANIMATION_H */
