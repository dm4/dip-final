/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef INTERACTIVE_STATE_H
#define INTERACTIVE_STATE_H

#include "AnimationState.h"

/*!
 * \brief
 * Interactive animation state.
 * 
 * Show your photo and interact with ZhengMeiXiang.
 */
class InteractiveState: public AnimationState
{
public:
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, 
		const std::map<std::string, osc::ReceivedMessage *> &messageMap);
	void processAnimation(Director *director);

private:
	int eyePosIndex;
	cv::Mat photo;
};

#endif // INTERACTIVE_STATE_H