#pragma once
#ifndef GHOST_STATE_H
#define GHOST_STATE_H

#include "AnimationState.h"
#include "Picture.h"

class GhostState: public AnimationState
{
	public:
		GhostState() {
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

#endif // GHOST_STATE_H
