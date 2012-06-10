#pragma once
#ifndef BEFOREGAME_STATE_H
#define BEFOREGAME_STATE_H

#include "AnimationState.h"
#include "Picture.h"

class BeforeGameState: public AnimationState
{
	public:
		BeforeGameState() {
			isInitialized = false;
		}
		void processTime(Director *director, const int64 &currentTickCount);
		void processKeyEvent(Director *director, const int &key);
		void processMouseEvent(Director *director, const cv::Point &mousePos);
		void processOSC(Director *director, const std::map<std::string, osc::ReceivedMessage *> &messageMap);
		void processAnimation(Director *director);

	private:
		int eyePosIndex;
		bool isInitialized;
        bool hasMole[6];
};

#endif // BEFOREGAME_STATE_H
