/*!
 *\mainpage
 * This is a documentation for ZhengMeiXiang API. \n
 The API can make you easier to create a interactive artwork for ZhengMeiXiang. \n
 Enjoy it! :D
 
 \section Required
 - Microsoft Visual Studio 2010
 - OpenCV 2.3.1+: Computer Vision Library \n
 http://code.opencv.org/projects/OpenCV/wiki/WikiStart
 - Oscpack 1.0.2+: C++ Open Sound Control (OSC) packet manipulation library \n
 http://code.google.com/p/oscpack/
 - Flycapture 2.2+ x64: Point Grey 1394 camera driver and api \n
 http://www.apostar.com.tw/download.html

 \section Note
 - Install Flycapture: It is necessary to select drivers for firewire cameras.
 - Please configure project setting for OpenCV by yourself. 
 If you want to use the default setting. Follow the steps:
	1. Download OpenCV 2.3.1
	2. Rename OpenCV directory name to "OpenCV 2.3.1" and put it to disk C.
	3. Configure the environment variables: Add 
	"C:\OpenCV 2.3.1\build\x64\vc10\bin;C:\OpenCV 2.3.1\build\common\tbb\intel64\vc10" to the variable, named PATH.
	4. Restart computer.
	5. Use Release configuration.
 * 
 Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>

 \author Zheng-Xiang Ke
 * 
 *\copyright Copyright (c) 2012 by <Zheng-Xiang Ke/ NTU IVLab>
 */
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <process.h>
#include <ctime>
#include "ip/UdpSocket.h"
#include "Director.h"

using namespace cv;

const Point winPos(-15, 6); //!< Window position
const Point mousePos(0, 0); //!< Mouse position
const int port = 9999; //!< Port for receiving OSC messages

void hideWinBarAndMoveMouse();
unsigned __stdcall receiveOSC(void *ArgList);

int main(void)
{
	hideWinBarAndMoveMouse();

	srand(static_cast<unsigned int>(time(NULL)));
	
	Director *director = new Director;

	unsigned int threadID = 0;
	HANDLE hThread = reinterpret_cast <HANDLE>(_beginthreadex(NULL, 0, receiveOSC, 
		static_cast<void *>(&(*director->getOSCListener())), 0, &threadID));

	director->action();
	
	delete director;
	destroyAllWindows();
	CloseHandle(hThread);

	return 0;
}

void hideWinBarAndMoveMouse()
{
#if SIMULATOR == 1
	namedWindow(winName, CV_WINDOW_NORMAL);
	cvResizeWindow(winName.c_str(), 1280, 480);
	cvMoveWindow(winName.c_str(), winPos.x, winPos.y);
#else
	namedWindow(winName);
	cvMoveWindow(winName.c_str(), winPos.x, winPos.y);

	HWND win = (HWND)cvGetWindowHandle(winName.c_str()) ;
	HWND window = GetParent(win) ;

	long style = GetWindowLong( window, GWL_STYLE);

	style = style & (~WS_CAPTION) ;

	SetWindowLong( window, GWL_STYLE, style);
#endif

	DWORD event = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
	mouse_event(event, mousePos.x, mousePos.y, 0, 0);
}

unsigned __stdcall receiveOSC(void *ArgList)
{	
	OSCListener *oscListener = static_cast<OSCListener *>(ArgList);
	UdpListeningReceiveSocket receiveSocket(
		IpEndpointName( IpEndpointName::ANY_ADDRESS, port ), oscListener );

	receiveSocket.RunUntilSigInt();

	return 0;
}