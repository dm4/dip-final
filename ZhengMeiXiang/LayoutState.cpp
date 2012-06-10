/*!
 *\author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include "AllAnimationStates.h"
#include "Director.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

void LayoutState::processTime(Director *director, const int64 &currentTickCount)
{

}

void LayoutState::processKeyEvent(Director *director, const int &key)
{
	Picture &picture = *director->getPictureAt(selectIndex);
	Mat painting = director->getPainting();
	Rect rect = picture.getFrame();

	switch(key) {
	case LEFT_ARROW:
		if (rect.x - 2 >= 0) {
			Rect newROI(rect.x - 2, rect.y, rect.width, rect.height);
			picture.setFrame(painting(newROI), newROI);
		}
		break;
	case UP_ARROW:
		if (rect.y - 2 >= 0) {
			Rect newROI(rect.x , rect.y - 2, rect.width, rect.height);
			picture.setFrame(painting(newROI), newROI);
		}
		break;
	case RIGHT_ARROW:
		if (rect.x + 2 + rect.width <= paintingSize.width) {
			Rect newROI(rect.x + 2, rect.y, rect.width, rect.height);
			picture.setFrame(painting(newROI), newROI);
		}
		break;
	case DOWN_ARROW:
		if (rect.y + 2 + rect.height <= paintingSize.height) {
			Rect newROI(rect.x , rect.y + 2, rect.width, rect.height);
			picture.setFrame(painting(newROI), newROI);
		}
		break;
	case 'w': case 'W':
		selectIndex = (selectIndex - numPicturesInPainting.width >= 0)?
			selectIndex - numPicturesInPainting.width : selectIndex;
		break;
	case 'a': case 'A':
		selectIndex = (selectIndex % numPicturesInPainting.width == 0)?
			selectIndex : selectIndex - 1;
		break;
	case 's': case 'S':
		selectIndex = (selectIndex + numPicturesInPainting.width <= numPhotos)?
			selectIndex + numPicturesInPainting.width : selectIndex;
		break;
	case 'd': case 'D':
		selectIndex = (selectIndex % numPicturesInPainting.width 
			== numPicturesInPainting.width - 1)? selectIndex : selectIndex + 1;
		break;
	case '[':
		{
			Rect newROI(rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4);
			if (newROI.width >= 50 && newROI.height >= 50 && 
				newROI.x + newROI.width <= paintingSize.width &&
				newROI.y + newROI.height <= paintingSize.height) {
					picture.setFrame(painting(newROI), newROI);
			}
		}
		break;
	case ']':
		{
			Rect newROI(rect.x - 2, rect.y - 2, rect.width + 4, rect.height + 4);
			if (newROI.width <= 200 && newROI.height <= 200 && 
				newROI.x >= 0 && newROI.y >= 0) {
					picture.setFrame(painting(newROI), newROI);
			}
		}
		break;
	case ENTER:
		{
			ofstream ofs("settings.txt");
			if (ofs.good()) {
				for (int i = 0; i < numPhotos; ++i) {
					Picture& picture = *director->getPictureAt(i);
					Rect frame = picture.getFrame();
					ofs << frame.x << " " << frame.y << " " << 
						frame.width << " " << frame.height << endl;
				}
			}
			else
				cerr << "Writing a setting file fails!" << endl;
			ofs.close();

			director->setAnimationState(new InitState);
		}
		break;
	default:
		break;
	}
}

void LayoutState::processMouseEvent(Director *director, const Point &mousePos)
{

}

void LayoutState::processOSC(Director *director, 
	const std::map<std::string, osc::ReceivedMessage *> &messageMap)
{
}

void LayoutState::processAnimation(Director *director)
{
	if (!director->getCanRecord())
		director->setCanRecord(false);

	ImagesPool *imagesPool = director->getImagesPool();
	for (int i = 0; i < numPhotos; ++i) {
		Picture& picture = *director->getPictureAt(i);
		if (i == selectIndex)
			picture.setContent(imagesPool->starPhotos[i]);
		else
			picture.setContent(Mat(picture.size(), picture.getType(), Scalar(255, 255, 255)));
	}
}