/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef IMAGES_POOL_H
#define IMAGES_POOL_H

#include <opencv2/opencv.hpp>

/*!
 * \brief
 * A pool contains all used images.

   If you need to read images from files, please cache these data in this class.
 * 
 */
class ImagesPool
{
public:
	std::vector<cv::Mat> starPhotos; //!< all star photos
};

#endif // IMAGES_POOL_H