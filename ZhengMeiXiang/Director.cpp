/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "Director.h"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "AllAnimationStates.h"
#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"

#if SIMULATOR == 1
cv::VideoCapture capture(0);

#else
#include "C/FlyCapture2_C.h"

fc2Context context;
fc2Image rawImage, image;

#endif

using namespace std;
using namespace cv;
using namespace osc;

const int delay = 1000 / 35; //!< delay time
const int sendOSCPort = 7000; //!< Port for sending OSC messages
const int oscMessageBufferSize = 1024; //!< Buffer size for a OSC message
const string settingFilePath = "my-settings.txt"; //!< Setting file path
const string starPhotosDirPath = "Stars/"; //!< Star photo Directory path
const Size pictureDefaultSize(110, 150);

Point2f normalMousePos(0.0f, 0.0f); //!< normalized mouse pos


void onMouse(int event, int x, int y, int flags, void *param)
{
	normalMousePos = Point2f(static_cast<float>(x) / paintingSize.width, 
		static_cast<float>(y) / paintingSize.height);
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		break;
	default:
		break;
	}
}

void Director::action()
{
	setMouseCallback(winName, onMouse, 0);

	if (!readData()) {
		cerr << "Read data error!" << endl;
		return;
	}
	if (!initCamera()) {
		cerr << "Init camera error!" << endl;
		return;
	}

	int key = 0;
	while(key != ESC) {
		processGlobalKeyEvent(key);
		processGlobalOSC();
		record();

		animationState->processTime(this, getCurrentTime());
		animationState->processKeyEvent(this, key);
		animationState->processMouseEvent(this, get2DEyePos(normalMousePos));
		animationState->processOSC(this, oscListener->getMessageMap());
		animationState->processAnimation(this);
		
		playAnimation();
			
		imshow(winName, painting);
		key = waitKey(delay);
	}
}

void Director::playAnimation()
{
	for (vector<Picture>::iterator pictureIterator = pictures.begin();
		pictureIterator != pictures.end(); ++pictureIterator)
		pictureIterator->playAnimation();
}

bool Director::isAllAnimationEnd()
{
	for (std::vector<Picture>::iterator pictureIterator = pictures.begin();
		pictureIterator != pictures.end(); ++pictureIterator) {
		if (!pictureIterator->getAnimation()->animationEnded())
			return false;
	}

	return true;
}

bool Director::readData()
{
	// Read setting
	ifstream ifs(settingFilePath.c_str());
	if (ifs.bad()) {
		cerr << "Read the setting file: " << settingFilePath << " error" << endl;
		return false;
	}
	int numRead = 0;
	while(ifs.good() && numRead < numPhotos) {
		cout << "numRead: " << numRead << endl;
		Rect rect;
		ifs >> rect.x >> rect.y >> rect.width >> rect.height;
		pictures.push_back(Picture(painting(rect), rect));
		++ numRead;
	}

	// Read all stars' photos
	//for (int i = 0; i < numPhotos; ++i) {
	//	ostringstream oss;
	//	oss << starPhotosDirPath << "imgSrc" 
	//		<< setfill('0') << setw(2) << i << ".jpg";
	//	Mat starPhoto = imread(oss.str());
	//	if (starPhoto.data == NULL) {
	//		cerr << "Read the star photo: " << oss.str() << " error" << endl;
	//		return false;
	//	}
	//	Mat resizeStarPhoto;
	//	resize(starPhoto, resizeStarPhoto, pictures[i].size());

	//	imagesPool->starPhotos.push_back(resizeStarPhoto);
	//}

	return true;
}

bool Director::initCamera()
{
#if SIMULATOR == 1
	if (!capture.isOpened()) {
		cerr << "Error: Can not open camera!" << endl;
		return false;
	}
#else
	fc2Error error = fc2CreateContext(&context);
	if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2CreateContext:" << error << endl;
        return false;
    }        

	unsigned int numCameras = 0;
    error = fc2GetNumOfCameras(context, &numCameras);
    if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2GetNumOfCameras: " << error << endl;
        return false;
    }
    if (numCameras == 0) {
        // No cameras detected
		cerr << "No cameras detected." << endl;
        return false;
    }

    // Get the 0th camera
	fc2PGRGuid guid;
    error = fc2GetCameraFromIndex(context, 0, &guid);
    if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2GetCameraFromIndex: " << error << endl;
        return false;
    }    

    error = fc2Connect(context, &guid);
    if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2Connect: " << error << endl;
        return false;
    }

    error = fc2StartCapture(context);
    if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2StartCapture: " << error << endl;
        return false;
    }

	error = fc2CreateImage(&rawImage);
	if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2CreateImage: " << error << endl;
		return false;
	}
	error = fc2CreateImage(&image);
	if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2CreateImage: " << error << endl;
		return false;
	}
#endif

	return true;
}

void Director::processGlobalKeyEvent(const int key)
{
	switch(key) {
	case 'r': case 'R':
		setAnimationState(new IdleState);
		break;
	case 'l': case 'L':
		setAnimationState(new LayoutState);
		break;
	default:
		break;
	}
}

void Director::processGlobalOSC()
{
	ReceivedMessage *oscMessage = oscListener->getMessageWithType("/interactionState");
	if (oscMessage != NULL) {
		ReceivedMessageArgumentStream oscArgStream = oscMessage->ArgumentStream();
		bool interactionState = false;
		oscArgStream >>  interactionState >> EndMessage;
		if (!interactionState) {
			setAnimationState(new IdleState);
			oscListener->clearMessageWithType("/interactionState");
		}
	}

	oscMessage = oscListener->getMessageWithType("/Start");
	if (oscMessage != NULL) {
		ReceivedMessageArgumentStream oscArgStream = oscMessage->ArgumentStream();
		const char *photoPath = NULL;
		oscArgStream >> photoPath >> EndMessage;
		if (photoPath != NULL) {
			capturePhotoPath = photoPath;
			oscListener->clearMessageWithType("/Start");
		}
	}
}

void Director::record()
{
	if (!canRecord)
		return;
#if SIMULATOR == 1
	capture >> recordImage;
#else
	fc2Error error = fc2RetrieveBuffer(context, &rawImage);
	if (error != FC2_ERROR_OK) {
		cerr << "Error in retrieveBuffer: " << error << endl;
		return;
	}

	error = fc2ConvertImageTo(FC2_PIXEL_FORMAT_BGR, &rawImage, &image);
	if (error != FC2_ERROR_OK) {
		cerr << "Error in fc2ConvertImageTo: " << error << endl;
		return;
	}

	recordImage = Mat(image.rows, image.cols, CV_8UC3, image.pData);
#endif

	resize(recordImage, recordImage, pictureDefaultSize);
}

std::vector<Picture>::iterator Director::getPictureAt(const unsigned int &index) 
{
	std::vector<Picture>::iterator iter = pictures.begin();
	if (index >= pictures.size()) {
		std::cerr << "row: " << index / numPicturesInPainting.width << 
			", col: " << index % numPicturesInPainting.width << 
			" is out of bound!" << std::endl;
		return iter;
	}

	iter += index;
	return iter;
}

void Director::setAnimationState(AnimationState *newAnimationState) {
	if (animationState != NULL)
		delete animationState;
	animationState = newAnimationState;

	for (vector<Picture>::iterator iter = pictures.begin(); iter != pictures.end(); ++iter)
		iter->setAnimation(IdleAnimationEnum, cv::noArray(), cv::noArray());
}

vector<Picture>::iterator Director::getPictureAt(
	const unsigned int &row, const unsigned int &col)
{
	int index = col * numPicturesInPainting.width + row;
	return getPictureAt(index);
}

void Director::takePhoto(OutputArray &photo)
{
	Mat image;
	takePhotoByCamera(image);
	image.convertTo(photo, photo.type());
}

Point2f Director::getEyePosWithOSCMessage
	(const map<string, ReceivedMessage *> &messageMap) const
{
	Point2f eyePos(0, 0);
	map<string, ReceivedMessage *>::const_iterator messageMapIter = messageMap.find("/PoG");
	if (messageMapIter != messageMap.end()) {
		ReceivedMessage *oscMessage = messageMapIter->second;
		ReceivedMessageArgumentStream oscArgStream = oscMessage->ArgumentStream();
		oscArgStream >>  eyePos.x >> eyePos.y >> EndMessage;
		oscListener->clearMessageWithType("/PoG");
	}

	return eyePos;
}

int Director::get1DEyePos(const Point2f &normalEyePos) const
{
	Point2f realEyePos(normalEyePos.x * paintingSize.width, 
		normalEyePos.y * paintingSize.height);
	float minDistance = FLT_MAX;
	int index = 0;
	for (int i = 0; i < numPhotos; ++i) {
		Rect frame = pictures[i].getFrame();
		if (realEyePos.x >= frame.x && realEyePos.x <= frame.x + frame.width &&
			realEyePos.y >= frame.y && realEyePos.y <= frame.y + frame.height)
		{
			return i;
		}

		Vec2d vec(realEyePos.x - (frame.x + frame.width / 2),
			realEyePos.y - (frame.y + frame.height / 2));
		float distance = static_cast<float>(norm(vec));
		if (distance < minDistance) {
			index = i;
			minDistance = distance;
		}
	}

	//return index;
	return 6;
}

Point Director::get2DEyePos(const Point2f &normalEyePos) const
{
	int index = get1DEyePos(normalEyePos);
	return Point(index % numPicturesInPainting.width, index / numPicturesInPainting.width);
}

void Director::takePhotoByCamera(cv::Mat& photo)
{
#if SIMULATOR == 1
	capture >> photo;
#else
	sendOSCBoolMessage("localhost", sendOSCPort, "/test3", true);
	while(capturePhotoPath.empty()) {
		processGlobalOSC();
		Sleep(delay);
	}
	photo = imread(capturePhotoPath);
	capturePhotoPath.clear();
#endif

	resize(photo, photo, pictureDefaultSize);
}

void Director::sendOSCBoolMessage(const string &ipAddress, const int &port, 
	const string &name, const bool &message)
{
	UdpTransmitSocket updTransmitSocket( IpEndpointName(ipAddress.c_str(), port) );
	char buffer[oscMessageBufferSize];
	OutboundPacketStream packetStream( buffer, oscMessageBufferSize );

	packetStream << BeginMessage( name.c_str() ) << message  << EndMessage;
	updTransmitSocket.Send( packetStream.Data(), packetStream.Size() );
}