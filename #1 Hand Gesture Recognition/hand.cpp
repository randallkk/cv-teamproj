#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/cvconfig.h>
#include <opencv2/imgproc/imgproc.hpp> //cvtcolor

using namespace cv;
using namespace std;

int main(int, char**)
{
	Mat frame, want, cut;
	VideoCapture cap;
	int deviceID = 0;
	deviceID = 0;
	int apiID = cv::CAP_ANY;
	cap.open(deviceID + apiID);

	//예외 상황
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	cout << "Start grabbing" << endl;
	for (;;)
	{
		cap.read(frame);
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		imshow("Live", frame);

		cvtColor(frame, want, COLOR_BGR2HSV);

		char text[] = "winner";
		putText(want, text, Point(50, 50) , FONT_HERSHEY_SIMPLEX , 1, Scalar::all(255), 1, 8);
		rectangle(want, Point(100, 200), Point(300, 400),1);

		Rect cropRect(100, 200, 100, 100);
		Mat cut = frame(cropRect);

		namedWindow("crop"); // Create a window for display.
		imshow("crop", cut);// Show our image inside


		namedWindow("binary image"); // Create a window for display.
		imshow("binary image", want);// Show our image inside



		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}