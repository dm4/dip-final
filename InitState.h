/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef INIT_STATE_H
#define INIT_STATE_H

#include "AnimationState.h"

/*!
 * \brief
 * Initial animation state.
 * 
 * Initialize all variables in director.
 */
class InitState: public AnimationState
{
public:
	InitState(): needToPlayDefaultMusic(true) {}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
// 	void processOSC(Director *director, 
// 		const std::map<std::string, osc::ReceivedMessage *> &messageMap);
	void processAnimation(Director *director);

private:
	bool needToPlayDefaultMusic;
};

#endif // INIT_STATE_H
