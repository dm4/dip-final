#include "FadeoutAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void FadeoutAnimation::play(Picture &picture) {
	if (!ended) {
		IplImage *img = new IplImage(picture);

		int step = img->widthStep;
		int channel = img->nChannels;

		a++;

		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				for (int k = 0; k < channel; k++) {
					img->imageData[i*step + j*channel + k] = (uchar)img->imageData[i*step + j*channel + k] * pow(0.9, a);
				}
			}
		}

		if(pow(0.9, a) < 0.001){
			ended = true;
		}

		if(picture.getIsFocus()){
            int len = 60;
            int centerX = picture.cols / 2;
            int centerY = picture.rows / 2;

			CvPoint p1, p2, p3, p4;
            p1 = cvPoint(centerX, centerY - len / 2);
            p2 = cvPoint(centerX, centerY + len / 2);
            p3 = cvPoint(centerX - len / 2, centerY);
            p4 = cvPoint(centerX + len / 2, centerY);

			CvScalar color;
			color = CV_RGB(255, 0 ,0);
			cvLine(img, p1, p2, color, 3, -1, 0);
			cvLine(img, p3, p4, color, 3, -1, 0);

			picture.setFocus(false);
		}
		picture.setContent(img);
	}
}

