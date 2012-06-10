#pragma once
#ifndef HUMAN_STATE_H
#define HUMAN_STATE_H

#include "AnimationState.h"
#include "Picture.h"

class HumanState: public AnimationState
{
	public:
		HumanState() {
			isInitialized = false;
		}
		void processTime(Director *director, const int64 &currentTickCount);
		void processKeyEvent(Director *director, const int &key);
		void processMouseEvent(Director *director, const cv::Point &mousePos);
		void processOSC(Director *director, const std::map<std::string, osc::ReceivedMessage *> &messageMap);
		void processAnimation(Director *director);
        void setScore(Picture &picture, int score);

	private:
		int eyePosIndex;
		cv::Mat photo;
		bool isInitialized;
		int hasMole[6];
		IplImage* black;
};

#endif // HUMAN_STATE_H
