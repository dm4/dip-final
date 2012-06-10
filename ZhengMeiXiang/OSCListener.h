/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#pragma once
#ifndef OSCLISTENER_H
#define OSCLISTENER_H

#include <map>
#include <string>
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"

/*!
 * \brief
 * A listener for open sound control (OSC).
 * 
 * There are some communications with OSC:
	- Pressure detection: communicate a bool value 
		controls the beginning or the end of interactive stage.
	- Taking a photo: get a user's photo.
	- Eye detection: get a user's eye position we guess.
 */
class OSCListener: public osc::OscPacketListener
{
public:
	OSCListener(): osc::OscPacketListener() {}

	/*!
	*  \brief A getter for a receivedMessage pointer with type.
	*  \param type a receivedMessage type such as 
	"/interactionState", "/Start", "/PoG", and so on.
	*  \return a receivedMessage pointer.
	*/
	osc::ReceivedMessage* getMessageWithType(const std::string &type);

	/*!
	*  \brief Clear the item in a message map with type.
	*  \param type a receivedMessage type such as 
	"/interactionState", "/Start", "/PoG", and so on.
	*/
	void clearMessageWithType(const std::string &type);

	/*!
	*  \brief A getter for a message map.
	*  \return a message map contains all messages.
	*/
	inline std::map<std::string, osc::ReceivedMessage *> getMessageMap() const {return messageMap;}

protected:
	/*!
	*  \brief process a received message.
	*  \param m a received message.
		\param remoteEndpoint information about the sender.
	*/
	void ProcessMessage( const osc::ReceivedMessage& m, 
		const IpEndpointName& remoteEndpoint );

private:
	std::map<std::string, osc::ReceivedMessage *> messageMap;
};
#endif // OSCLISTENER_H