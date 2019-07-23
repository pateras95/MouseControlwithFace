#include "pch.h"

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <Windows.h>

using namespace std;
using namespace cv;

void MouseMove(int dx, int dy)
{
	INPUT mouse = { 0 };
	mouse.type = INPUT_MOUSE;
	mouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	mouse.mi.dx = dx;
	mouse.mi.dy = dy;

	::SendInput(1, &mouse, sizeof(INPUT));
}

int main(int argc, const char** argv)
{
	VideoCapture captureDevice;
	Mat frame, image;

	int posx = 0, posy = 0; // The positions of the face of a person according to which the mouse has to be moved.


	//create the cascade classifier object used for the face detection
	CascadeClassifier face_cascade;
	//use the haarcascade_frontalface_alt.xml library
	face_cascade.load("C:\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalcatface.xml");

	//setup video capture device and link it to the first capture device

	captureDevice.open(0);

	//setup image files used in the capture process
	Mat captureFrame;
	Mat grayscaleFrame;
	Mat gray, smallImg;;

	//create a window to present the results
	namedWindow("outputCapture", 1);

	//create a loop to capture and find faces
	while (true)
	{
		//capture a new image frame
		captureDevice >> captureFrame;
		flip(captureFrame, captureFrame, 1);

		//convert captured image to gray scale and equalize
		cvtColor(captureFrame, gray, COLOR_BGR2GRAY);
		equalizeHist(smallImg, smallImg);

		//create a vector array to store the face found
		std::vector<Rect> faces;

		//find faces and store them in the vector array
		face_cascade.detectMultiScale(gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		//draw a rectangle for all found faces in the vector array on the original image
		for (int i = 0; i < faces.size(); i++)
		{
			Point pt1(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
			Point pt2(faces[i].x, faces[i].y);

			rectangle(captureFrame, pt1, pt2, Scalar(0, 255, 0, 0), 1, 8, 0);
		}

		//print the output
		imshow("outputCapture", captureFrame);
		if (faces.size() > 0)
		{
			/* My thresholds
			Center: 320, 210
			x+: 500, 210
			x-: 150, 210
			y+: 320, 350
			y-: 320, 117
			*/
			posx = faces[0].x + faces[0].width / 2; // X coordinate of the face tracked
			posy = faces[0].y + faces[0].height / 2; // Y coordinate of the face tracked
			/* for debugging
			cout << "X = " << posx << ", Y = " << posy << endl;
			*/
			int mouseX = (posx - 320) / 10;
			int mouseY = (posy - 210) / 10;
			MouseMove(mouseX, mouseY);


			//Click event
			//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			//mouse_event(MOUSEEVENTF_LEFTUP, mouseX, mouseY, 0, 0);
		}

		//pause for 33ms
		waitKey(33);
	}


	return 0;
}