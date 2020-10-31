#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\opencv.hpp> //blur efekti i?in
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	VideoCapture cap(0);
	if (!cap.isOpened()){
		printf("Can't open the camera");
		return -1;
	}

	Mat img;

	while (1){
		cap >> img;
		imshow("camera img", img);
		if (waitKey(1) == 27)
			break;
	}

	return 0;
}