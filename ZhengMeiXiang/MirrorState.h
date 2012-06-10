/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef MIRROR_STATE_H
#define MIRROR_STATE_H

#include "AnimationState.h"

/*!
 * \brief
 * Mirror animation state.
 * 
 * You see your mirror image and two laughing images
 around your mirror image in ZhengMeiXiang.
 */
class MirrorState: public AnimationState
{
public:
	MirrorState(): duration(0.0f), showImageFlag(0) {}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, 
		const std::map<std::string, osc::ReceivedMessage *> &messageMap);
	void processAnimation(Director *director);

private:
	float duration;
	int showImageFlag;
};

#endif // MIRROR_STATE_H