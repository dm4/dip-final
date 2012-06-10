/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "OSCListener.h"
#include <iostream>

using namespace std;
using namespace osc;

void OSCListener::ProcessMessage( const ReceivedMessage& m, 
	const IpEndpointName& remoteEndpoint )
{
	try
	{
		messageMap[m.AddressPattern()] = new ReceivedMessage(m);
	}
	catch (Exception& e)
	{
		cerr << "error while parsing message in OSCListener: "
			<< m.AddressPattern() << ": " << e.what() << "\n";
	}
}

ReceivedMessage* OSCListener::getMessageWithType(const std::string &type)
{
	map<string, ReceivedMessage*>::iterator messageMapIter = messageMap.find(type);
	if (messageMapIter == messageMap.end())
		return NULL;
	else
		return messageMapIter->second;
}

void OSCListener::clearMessageWithType(const std::string &type)
{
	map<string, ReceivedMessage*>::iterator messageMapIter = messageMap.find(type);
	if (messageMapIter == messageMap.end())
		return;

	messageMap.erase(messageMapIter);
}