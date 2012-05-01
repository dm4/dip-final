/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "FlipAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void FlipAnimation::play(Picture &picture)
{
	resetTransformMatrix();
	handleFlipState();
	rotate();
	display(picture);
}

void FlipAnimation::initRotationAxis()
{
	Point3f rotationAxis(0.0f, 0.0f, 0.0f);
	switch (flipOrientation)
	{
	case FLIP_UPPER:
		rotationAxis = Point3f(1.0f, 0.0f, 0.0f);
		break;
	case FLIP_UPPERRIGHT:
		rotationAxis = Point3f(static_cast<float>(sourceImage.rows), 
			static_cast<float>(sourceImage.cols), 0.0f);
		break;
	case FLIP_RIGHT:
		rotationAxis = Point3f(0.0f, 1.0f, 0.0f);
		break;
	case FLIP_LOWERRIGHT:
		rotationAxis = Point3f(static_cast<float>(-sourceImage.rows), 
			static_cast<float>(sourceImage.cols), 0.0f);
		break;
	case FLIP_LOWER:
		rotationAxis = Point3f(-1.0f, 0.0f, 0.0f);
		break;
	case FLIP_LOWERLEFT:
		rotationAxis = Point3f(static_cast<float>(-sourceImage.rows), 
			static_cast<float>(-sourceImage.cols), 0.0f);
		break;
	case FLIP_LEFT:
		rotationAxis = Point3f(0.0f, -1.0f, 0.0f);
		break;
	case FLIP_UPPERLEFT:
		rotationAxis = Point3f(static_cast<float>(sourceImage.rows), 
			static_cast<float>(-sourceImage.cols), 0.0f);
		break;
	default:
		break;
	}

	float length = sqrt(rotationAxis.x * rotationAxis.x + 
		rotationAxis.y * rotationAxis.y + rotationAxis.z * rotationAxis.z);
	unitRotationAxis = Point3f(rotationAxis.x / length, 
		rotationAxis.y / length, rotationAxis.z / length);
}

void FlipAnimation::resetTransformMatrix()
{
	float transform[4][4] = {
		{1.0f,	0.0f,	0.0f,	static_cast<float>(-sourceImage.cols >> 1)},
		{0.0f,	1.0f,	0.0f,	static_cast<float>(-sourceImage.rows >> 1)},
		{0.0f,	0.0f,	1.0f,	0.0f},
		{0.0f,	0.0f,	0.0f,	1.0f}
	};

	Mat(4, 4, CV_32FC1, transform).copyTo(transformMatrix);
}

void FlipAnimation::handleFlipState()
{
	switch(flipState) {
	case FLIP_1:
		sourceImage = startImage;
		if (index >= speed) {
			index = 0;
			flipState = FLIP_2;
		}
		break;
	case FLIP_2:
		sourceImage = endImage;
		if (index >= speed) {
			index = 0;
			flipState = FLIP_END;
		}
		break;
	case FLIP_END:
		if (canReverse) {
			flipState = FLIP_1;
			canReverse = false;

			Mat temp = startImage;
			startImage = endImage;
			endImage = temp;
		}
		else
			ended = true;
		break;
	default:
		break;
	}
}

void FlipAnimation::rotate() {
	float floatIndex = static_cast<float>(index);
	switch(flipState) {
	case FLIP_1:
		angle = floatIndex * floatIndex / speed / speed * 90.0f * deg2rad;
		break;
	case FLIP_2:
		angle = - ((speed-floatIndex) * (speed-floatIndex) / speed / speed) 
			* 90.0f * deg2rad;
		break;
	case FLIP_END:
		angle = 0.0f;
		break;
	default:
		break;
	}

	index += 1;
	rotateWithRotationAxis();
}

void FlipAnimation::rotateWithRotationAxis() 
{
	float c = cos(angle), s = sin(angle);
	// rotation matrix reference: http://en.wikipedia.org/wiki/Rotation_matrix
	float rotation[4][4] = {
		{c + unitRotationAxis.x*unitRotationAxis.x*(1-c),	unitRotationAxis.x*unitRotationAxis.y*(1-c) - unitRotationAxis.z*s,	unitRotationAxis.x*unitRotationAxis.z*(1-c) + unitRotationAxis.y*s,	0.0f},
		{unitRotationAxis.y*unitRotationAxis.x*(1-c) + unitRotationAxis.z*s,	c + unitRotationAxis.y*unitRotationAxis.y*(1-c),	unitRotationAxis.y*unitRotationAxis.z*(1-c) - unitRotationAxis.x*s,	0.0},
		{unitRotationAxis.z*unitRotationAxis.x*(1-c) - unitRotationAxis.y*s,	unitRotationAxis.z*unitRotationAxis.y*(1-c) + unitRotationAxis.x*s,	c + unitRotationAxis.z*unitRotationAxis.z*(1-c),	0.0f},
		{0.0f,					0.0f,					0.0f,					1.0f}
	};
	Mat rotationMatrix(4, 4, CV_32FC1, rotation);
	gemm(rotationMatrix, transformMatrix, 1.0, noArray(), 0.0, transformMatrix);
}

void FlipAnimation::display(Picture &picture)
{
	Point2f originalCornerPoints[4] = {
		Point2f(0.0f, 0.0f), Point2f(static_cast<float>(sourceImage.cols - 1), 0.0f), 
		Point2f(static_cast<float>(sourceImage.cols - 1), 
			static_cast<float>(sourceImage.rows - 1)), 
		Point2f(0.0f, static_cast<float>(sourceImage.rows - 1))
	};

	Point2f currentCornerPoints[4] = {Point2f(0.0f, 0.0f), Point2f(0.0f, 0.0f), 
		Point2f(0.0f, 0.0f), Point2f(0.0f, 0.0f)};
	for (int i = 0; i < 4; ++i)
		perspective(originalCornerPoints[i], currentCornerPoints[i]);

	Mat perspectiveMatrix = getPerspectiveTransform(originalCornerPoints, currentCornerPoints);
	warpPerspective(sourceImage, picture, perspectiveMatrix, 
		picture.size(), INTER_LINEAR, BORDER_CONSTANT);
}

void FlipAnimation::perspective(const cv::Point2f &src, cv::Point2f &dst)
{
	float srcArray[4] = {src.x, src.y, 0.0f, 1.0f};
	Mat srcMat(4, 1, CV_32FC1, srcArray);
	Mat dstMat;
	gemm(transformMatrix, srcMat, 1.0, noArray(), 0.0, dstMat);


	float t = D / (D - dstMat.at<float>(2));
	dst = Point2f(dstMat.at<float>(0) * t + (sourceImage.cols >> 1), 
		dstMat.at<float>(1) * t + (sourceImage.rows >> 1));
}