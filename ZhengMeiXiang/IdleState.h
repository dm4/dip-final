#pragma once
#ifndef IDLE_STATE_H
#define IDLE_STATE_H

#include "AnimationState.h"

class IdleState: public AnimationState
{
public:
	IdleState(): needToPlayDefaultMusic(true) {
        isInitialized = false;
    }
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
 	void processOSC(Director *director, const std::map<std::string, osc::ReceivedMessage *> &messageMap);
	void processAnimation(Director *director);

private:
	bool needToPlayDefaultMusic;
    bool isInitialized;
	cv::Rect bigFrame;
	cv::Rect originFrame;
};

#endif // IDLE_STATE_H
