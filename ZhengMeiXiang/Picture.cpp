/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "Picture.h"
#include "AnimationFactory.h"

using namespace cv;

void Picture::playAnimation()
{
	if (getTickCount() < enableAnimationTickCount)
		return;

	animation->setEnd(false);
	animation->play(*this);
}

void Picture::setAnimation(const AnimationEnum &animationEnum, 
	const cv::InputArray &startImg, const cv::InputArray &endImg)
{
	delete animation;

	Mat startImage = startImg.getMat();
	Mat endImage = endImg.getMat();
	if (!startImg.empty() && startImg.size() != this->size())
		cv::resize(startImg, startImage, this->size());
	if (!endImg.empty() && endImg.size() != this->size())
		cv::resize(endImg, endImage, this->size());
	
	animation = AnimationFactory::createAnimation(animationEnum, startImage, endImage);
}

void Picture::setAnimation(const AnimationEnum &animationEnum, 
	const CvArr *startImg, const CvArr *endImg)
{
	delete animation;
	animation = AnimationFactory::createAnimation(animationEnum, 
		cvarrToMat(startImg), cvarrToMat(endImg));
}

void Picture::setContent(const InputArray &image)
{
	if (image.size() == this->size()) {
		image.getMat().convertTo(*this, this->getType());
		return;
	}

	Mat resizeImage;
	cv::resize(image, resizeImage, this->size());
	resizeImage.convertTo(*this, this->getType());
}

void Picture::setContent(const CvArr* image)
{
	setContent(cv::cvarrToMat(image));
}

void Picture::setFrame(const cv::InputArray &roi, const cv::Rect &rect)
{
	Mat origin;
	this->copyTo(origin);
	setContent(zeros(frame.height, frame.width, getType()));
	this->data = roi.getMat().data;
	this->rows = rect.height;
	this->cols = rect.width;
	setContent(origin);
	frame = rect;
}