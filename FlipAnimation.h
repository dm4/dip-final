/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef FLIPANIMATION_H
#define FLIPANIMATION_H

#include "Animation.h"
#include <opencv2/opencv.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

typedef enum FlipAnimationOrientation{FLIP_UPPER, FLIP_UPPERRIGHT, FLIP_RIGHT, FLIP_LOWERRIGHT, 
	FLIP_LOWER, FLIP_LOWERLEFT, FLIP_LEFT, FLIP_UPPERLEFT} FlipAnimationOrientation; //!< 8 orientaions
typedef enum FlipAnimationState{FLIP_1, FLIP_2, FLIP_END} FlipAnimationState; //!< - FLIP_1: start image -> end image.
																			  //!< - FLIP_2: end imaeg -> start image.
																			  //!< - FLIP_END: the animation is end.

const FlipAnimationOrientation flips[8] = {FLIP_LEFT, FLIP_UPPERLEFT, FLIP_UPPER, 
	FLIP_UPPERRIGHT, FLIP_RIGHT, FLIP_LOWERRIGHT, FLIP_LOWER, FLIP_LOWERLEFT}; //!< Flip orientation by a angle.
const float deg2rad = static_cast<float>(M_PI / 180.0f); //!< degree to rad

/*!
 * \brief
 * Flip animation.
 * 
 * 3 steps:
	- Initialize a rotation axis and compute a rotation angle
	- Compute a rotation matrix
	- Perspective transformation
 * 
 */
class FlipAnimation : public Animation 
{
public:
	FlipAnimation(cv::InputArray startImg, cv::InputArray endImg):
		Animation(startImg, endImg), flipOrientation(FLIP_LEFT), 
		sourceImage(startImg.getMat()), flipState(FLIP_1), angle(0), D(300.0f), 
		speed(20.0f), index(0) 
	{
		initRotationAxis();
		resetTransformMatrix();
	}
	FlipAnimation(const CvArr *startImg, const CvArr *endImg):
	Animation(startImg, endImg), flipOrientation(FLIP_LEFT), 
		sourceImage(cv::cvarrToMat(startImg)), flipState(FLIP_1), angle(0), D(300.0f), 
		speed(20.0f), index(0) 
	{
		initRotationAxis();
		resetTransformMatrix();
	}
	void play(Picture &picture);

	/*!
	*  \brief A setter for some properties about the flip animation.
	*  \param orientation a flip orientation
	   \param newSpeed a flip speed.
	*/
	inline void setProperties(const FlipAnimationOrientation &orientation, 
		const float &newSpeed = 20.0f) 
	{
		flipOrientation = orientation;
		initRotationAxis();
		speed = newSpeed;
	}

private:
	void initRotationAxis();
	void resetTransformMatrix();
	void handleFlipState();
	void rotate();
	void rotateWithRotationAxis();
	void display(Picture &picture);
	void perspective(const cv::Point2f &src, cv::Point2f &dst);

private:
	FlipAnimationOrientation flipOrientation;
	float speed;

	float angle;
	FlipAnimationState flipState;
	cv::Mat transformMatrix;
	cv::Mat sourceImage;
	cv::Point3f unitRotationAxis;
	const float D;
	int index;
};

#endif /* FLIPANIMATION_H */
