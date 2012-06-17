#include "FadeinAnimation.h"
#include "Picture.h"
#include <opencv2/opencv.hpp>
#include <iostream>

#define FADEIN_COEFF 0.01

using namespace cv;
using namespace std;

void FadeinAnimation::play(Picture &picture) {
	if (!ended) {
		IplImage *source = new IplImage(startImage);
		IplImage *img = new IplImage(picture);

		int step = img->widthStep;
		int channel = img->nChannels;
		cout << "channel" << channel << endl;

		a++;
		if (a > 1/FADEIN_COEFF)
			a = 1/FADEIN_COEFF;
		cout << a << endl;

		for (int i = 0; i < img->height; i++) {
			for (int j = 0; j < img->width; j++) {
				for (int k = 0; k < channel; k++) {
					//img->imageData[i*step + j*channel + k] = (uchar)img->imageData[i*step + j*channel + k] * pow(0.95, a);
					img->imageData[i*step + j*channel + k] = (uchar)(source->imageData[i*step + j*channel + k]) * (0.02 * a);
				}
			}
		}

		if (FADEIN_COEFF * a > 0.9) {
            setEnd(true);
		}

		picture.setContent(img);
	}
}

