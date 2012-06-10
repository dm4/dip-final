#pragma once
#ifndef ENDGAME_STATE_H
#define ENDGAME_STATE_H

#include "AnimationState.h"
#include "Picture.h"

class EndGameState: public AnimationState
{
	public:
		EndGameState() {
			isInitialized = false;
		}
		void processTime(Director *director, const int64 &currentTickCount);
		void processKeyEvent(Director *director, const int &key);
		void processMouseEvent(Director *director, const cv::Point &mousePos);
		void processOSC(Director *director, const std::map<std::string, osc::ReceivedMessage *> &messageMap);
		void processAnimation(Director *director);

	private:
		void showText(Picture& picture);
		int eyePosIndex;
		bool isInitialized;
};

#endif // ENDGAME_STATE_H
