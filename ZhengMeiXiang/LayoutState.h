/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef LAYOUT_STATE_H
#define LAYOUT_STATE_H

#include "AnimationState.h"

/*!
 * \brief
 * Layout animation state.
 * 
 * Press the key 'l' or 'L' to enter the state.
 Arrange layout with:
  - arrow key: arrange the position of the selected image.
  - 'w'('W'), 'a'('A'), 's'('S'), 'd'('D'): change the selected image.
  - '[', ']': scale the selected image.
  - 'Enter': save this arrangement and return the initial animation state.
 */
class LayoutState: public AnimationState
{
public:
	LayoutState(): selectIndex(0) {}
	void processTime(Director *director, const int64 &currentTickCount);
	void processKeyEvent(Director *director, const int &key);
	void processMouseEvent(Director *director, const cv::Point &mousePos);
	void processOSC(Director *director, 
		const std::map<std::string, osc::ReceivedMessage *> &messageMap);
	void processAnimation(Director *director);

private:
	int selectIndex;
};

#endif // LAYOUT_STATE_H