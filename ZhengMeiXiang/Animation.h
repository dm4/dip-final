/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H

#include <opencv2/opencv.hpp>

class Picture;

/*!
 * \brief
 * An interface for animation.
 * 
 * All animation must inherit this class.
 */
class Animation
{
public:
	Animation(): startImage(cv::Mat()), endImage(cv::Mat()),
		canReverse(false), ended(true) {}
	Animation(const cv::InputArray &startImg, const cv::InputArray &endImg):
	  startImage(startImg.getMat()), endImage(endImg.getMat()), 
		  canReverse(false), ended(true) {}
	Animation(const CvArr *startImg, const CvArr *endImg):
	startImage(cv::cvarrToMat(startImg)), endImage(cv::cvarrToMat(endImg)), 
		canReverse(false), ended(true) {}

	/*!
	*  \brief the animation were end or not.
	*  \return
		- true: the animation were end.
		- false: otherwise.
	*/
	inline bool animationEnded() const {return ended;}

	/*!
	*  \brief A setter for end.
	*  \param end 
		- true: the animation is end.
		- false: otherwise.
	*/
	inline void setEnd(const bool &end) {ended = end;}

	/*!
	*  \brief A setter for reverse. If you want to this animation can be reversed, 
	then setting true, otherwise setting false.
	*  \param reverse 
		- true: the animation can be reversed.
		- false: otherwise.
	*/
	inline void setCanReverse(const bool &reverse) {canReverse = reverse;}

	/*!
	*  \brief play animation. It must be overriding.
	*  \param picture a picture applies this animation.
	*/
	virtual void play(Picture &picture) = 0;

protected:
	bool canReverse;
	bool ended;
	cv::Mat startImage;
	cv::Mat endImage;
};

#endif // ANIMATION_H