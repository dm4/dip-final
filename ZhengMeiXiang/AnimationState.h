/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef ANIMATION_STATE_H
#define ANIMATION_STATE_H

#include <opencv2/opencv.hpp>
#include <map>
#include "osc/OscReceivedElements.h"

class Director;

/*!
 * \brief
 * An interface for animation state.
 * 
 * We use state pattern to maintain a finite state machine. \n
 All animation state must inherit this class.
 */
class AnimationState
{
public:
	/*!
	* \brief
	* process a animation state by time.
	* 
	* Change the animation state or the properties of the animation state by time. \n
	If the animation can not be affected by time, you do nothing in this function.
	\param director a director pointer.
	\param currentTickCount the current tick.
	*/
	virtual void processTime(Director *director, const int64 &currentTickCount) = 0;

	/*!
	* \brief
	* process a animation state by a key event.
	* 
	* Change the animation state or the properties of the animation state by a key event. \n
	If the animation can not be affected by a key event, you do nothing in this function.
	\param director a director pointer.
	\param key key event id.
	*/
	virtual void processKeyEvent(Director *director, const int &key) = 0;

	/*!
	* \brief
	* process a animation state by a mouse event.
	* 
	* Change the animation state or the properties of the animation state by a mouse event. \n
	If the animation can not be affected by a mouse event, you do nothing in this function.
	\param director a director pointer.
	\param mousePos a mouse position that the unit of x-axis is row and that of y-axis is column.
	*/
	virtual void processMouseEvent(Director *director, const cv::Point &mousePos) = 0;

	/*!
	* \brief
	* process a animation state by a OSC message.
	* 
	* Change the animation state or the properties of the animation state by a OSC message. \n
	It is necessary to clear this message after processing it.
	If the animation can not be affected by a OSC message, you do nothing in this function.
	\param director a director pointer.
	\param messageMap a OSC message map contains all messages with type;
	*/
	virtual void processOSC(Director *director, 
		const std::map<std::string, osc::ReceivedMessage *> &messageMap) = 0;

	/*!
	* \brief
	* process animation.
	* 
	* apply an animation and change the animation state. \n
	Basically, it is necessary to change the animation state in this function 
	except for repeating the animation.
	\param director a director pointer.
	*/
	virtual void processAnimation(Director *director) = 0;
};

#endif	// ANIMATION_STATE_H