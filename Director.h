/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 *
 */
#pragma once
#ifndef DIRECTOR_H
#define DIRECTOR_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
// #include <Windows.h>
#include <string>
#include "Picture.h"
#include "InitState.h"
#include "FadeinState.h"
// #include "OSCListener.h"
#include "ImagesPool.h"

/*!
  \def SIMULATOR
  - 1: Simulator mode.
  - Show window bar and autoresize window.
  - Use a usb camera to capture image with OpenCV.
  - Take photo by a usb camera.
  - 0: In a exhibition, you must use this mode.
  - Show window bar and autoresize window.
  - Use a 1394 camera to capture iamge with FlyCapture.
  - Take photo by a digital single-lens camera.
 */
#define SIMULATOR 1

enum KeyNumber {ESC = 0x1b, ESC_LINUX = 0x10001b, LEFT_ARROW = 0x250000, UP_ARROW = 0x260000, 
	RIGHT_ARROW = 0x270000, DOWN_ARROW = 0x280000, ENTER = 13};

const std::string winName = "ZhengMeiXiang"; //!< Window name
const cv::Size paintingSize(2048, 768);		//!< Painting size
const cv::Size numPicturesInPainting(11, 4); //!< the numbers of pictures in row and column
const int numPhotos = numPicturesInPainting.width * numPicturesInPainting.height; //!< the total number pictures in the painting

/*!
 * \brief
 * The most top controller for ZhengMeiXiang.
 * 
 * Director controls all animations, receives all inputs such as 
 OSC, key events, and mouse events, plays background music, and takes user's photo.
 * 
 */
class Director
{
	public:
		// 	Director(): oscListener(new OSCListener()), animationState(new InitState), 
		// 		imagesPool(new ImagesPool), startTickCount(0), painting(cv::Mat(paintingSize, CV_8UC3)), 
		// 		canRecord(false), capturePhotoPath("") {}
		Director() {
			animationState = new FadeinState;
			imagesPool = new ImagesPool;
			startTickCount = 0;
			painting = cv::Mat(paintingSize, CV_8UC3);
			canRecord = false;
			capturePhotoPath = "";
		}

		/*!
		 *  \brief The artwork actions! This is a starting point.

		 Initialize all variables and processes anything you want,
		 such as recording, animation control, and so on.
		 */
		void action();

		/*!
		 *  \brief Play animation in the painting, i.e. play all animations for the all pictures.
		 */
		void playAnimation();

		/*!
		 *  \brief All animations for the all pictures were end or not.
		 *  \return
		 - true: all animations were end.
		 - false: otherwise.
		 */
		bool isAllAnimationEnd();

		/*!
		 *  \brief A getter for OSCListener.
		 *  \return a OSCListener pointer.
		 */
		// 	inline OSCListener* getOSCListener() const {return oscListener;}

		/*!
		 *  \brief A setter for startTickCount.
		 *  
		 set startTickCount to be start time when a audience starts interacting with the artwork.
		 */
		inline void setStartTickCount() {startTickCount = cv::getTickCount();}

		/*!
		 *  \brief A getter for animationState.
		 *  
		 There are many animation states in interaction. One state, one animation.
		 Hence, just change the animation state in order to change animation.
		 \param newAnimationState a new animation state that is set.
		 */
		void setAnimationState(AnimationState *newAnimationState);

		/*!
		 *  \brief Get a picture iterator located in the index.
		 *  \param index row-major order, i.e. if you want to access picture at (x, y), 
		 then index = y * width + x.
		 *  \return a picture iterator.
		 */
		std::vector<Picture>::iterator getPictureAt(const unsigned int &index);

		/*!
		 *  \brief Get a picture iterator located at (row, col).
		 *  \param row row.
		 \param col column.
		 *  \return a picture iterator.
		 */
		std::vector<Picture>::iterator getPictureAt(
				const unsigned int &row, const unsigned int &col);

		/*!
		 *  \brief Take a photo.
		 *  \param photo return photo taken.
		 */
		void takePhoto(cv::OutputArray photo);

		/*!
		 *  \brief Get a eye position from a OSC Message.
		 *  \param messageMap a OSC message map contains a eye position.
		 *  \return a eye position that the unit of x-axis and y-axis are float numbers.
		 */
		// 	cv::Point2f getEyePosWithOSCMessage
		// 		(const std::map<std::string, osc::ReceivedMessage *> &messageMap) const;

		/*!
		 *  \brief Get a 1-dimension eye position.
		 *  \param normalEyePos a normalized eye position, 
		 i.e. 0 <= eyePos.x <= 1 and 0 <= eyePos.y <= 1.
		 *  \return a eye position in row-major order.
		 */
		int get1DEyePos(const cv::Point2f &normalEyePos) const;

		/*!
		 *  \brief Get a 2-dimension eye position.
		 *  \param normalEyePos a normalized eye position, 
		 i.e. 0 <= eyePos.x <= 1 and 0 <= eyePos.y <= 1.
		 *  \return a eye position that the unit of x-axis is row and that of y-axis is column.
		 */
		cv::Point get2DEyePos(const cv::Point2f &normalEyePos) const;

		/*!
		 *  \brief Play background music.
		 *  \param musicPath a music file path.
		 */
		// 	inline void playMusic(const std::string &musicPath) const {
		// 		PlaySound(musicPath.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		// 	}

		/*!
		 *  \brief A getter for interactive duration.
		 *  \return duration (sec).
		 \param tickCount compute duration from startTickCount to tickCount
		 */
		inline float getDuration(const int64 &tickCount) {
			return static_cast<float>(tickCount - startTickCount) /
				static_cast<float>(cv::getTickFrequency());
		}

		/*!
		 *  \brief A getter for current time.
		 *  \return current time.
		 */
		inline int64 getCurrentTime() const { return cv::getTickCount(); }

		/*!
		 *  \brief A getter for ImagesPool.
		 *  \return a ImagesPool pointer.
		 */
		inline ImagesPool* getImagesPool() const {return imagesPool;}

		/*!
		 *  \brief A setter for canRecord.
		 *  \param recorded - true: record the user. -false: Don't record the user.
		 */
		inline void setCanRecord(const bool &recorded) {canRecord = recorded;}

		/*!
		 *  \brief A getter for canRecord.
		 *  \return canRecord - true: record the user. -false: Don't record the user.
		 */
		inline bool getCanRecord() const {return canRecord;}

		/*!
		 *  \brief A getter for recordImage.
		 *  \return a image that the camera captures.
		 */
		inline cv::Mat getRecordImage() const {return recordImage;}

		/*!
		 *  \brief A getter for painting.
		 *  \return a painting for ZhengMeiXiang.
		 */
		inline cv::Mat getPainting() const {return painting;}

	private:
		bool readData();
		bool initCamera();
		void processGlobalKeyEvent(const int key);
		// 	void processGlobalOSC();
		void record();
		void takePhotoByCamera(cv::Mat& photo);
		// 	void sendOSCBoolMessage(const std::string &ipAddress, const int &port, 
		// 		const std::string &name, const bool &message);

	private:
		// 	OSCListener *oscListener;
		AnimationState *animationState;
		ImagesPool *imagesPool;
		int64 startTickCount;
		cv::Mat painting;
		std::vector<Picture> pictures;
		bool canRecord;
		cv::Mat recordImage;
		std::string capturePhotoPath;
};

#endif	// DIRECTOR_H
