/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef FADEIN_STATE_H
#define FADEIN_STATE_H

#include "AnimationState.h"
#include "Picture.h"

/*!
 * \brief
 * Fade in and interactive animation state.
 * 
 * Show your photo and interact with ZhengMeiXiang.
 */
class FadeinState: public AnimationState
{
	public:
		FadeinState() {
			isInitialized = false;
		}
		void processTime(Director *director, const int64 &currentTickCount);
		void processKeyEvent(Director *director, const int &key);
		void processMouseEvent(Director *director, const cv::Point &mousePos);
		// 	void processOSC(Director *director, const std::map<std::string, osc::ReceivedMessage *> &messageMap);
		void processAnimation(Director *director);
        void setScore(Picture &picture, int score);

	private:
		int eyePosIndex;
		cv::Mat photo;
		bool isInitialized;
		bool hasMole[44];
		IplImage* black;
};

#endif // FADEIN_STATE_H
