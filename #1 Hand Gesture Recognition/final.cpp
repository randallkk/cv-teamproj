#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\opencv.hpp> 
#include <opencv2/video/background_segm.hpp>

using namespace cv;
using namespace std;

void track(int, void*);
Mat orjinalGoruntu;
Mat masked;
Mat med, idk, cannied, image;
Mat pic;
int thresh = 140, maxVal = 255;
int type = 2, deger = 8;



int main() {
	Ptr< BackgroundSubtractor> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2();
	cv::Rect myRoi(288, 12, 288, 288);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
	Mat frame;
	Mat resizeF;
	VideoCapture cap;
	cap.open(0);

	char key = waitKey();

	while (1) {
		Mat aynali2;
		cap >> orjinalGoruntu;
		cv::flip(orjinalGoruntu, image, 1);
		//cv::rectangle(orjinalGoruntu, cv::Point(300, 300), cv::Point(12, 12), cv::Scalar(0, 0, 255));
		cv::rectangle(image, myRoi, cv::Scalar(0, 0, 255));
		idk = image(myRoi);
		cvtColor(idk, med, COLOR_RGB2GRAY);
		//equalizeHist(med, med);
		GaussianBlur(med, med, Size(23, 23), 0); //35,35	//15,15
		//threshold(med, or2, thresh, maxVal, THRESH_OTSU + CV_THRESH_BINARY_INV);
		namedWindow("Trackbar");
		createTrackbar("Threshold", "Trackbar", &thresh, 250, track);	// thresh=threshold, maxVal=, type=, deger=degree
		createTrackbar("MaxVal", "Trackbar", &maxVal, 255, track);
		createTrackbar("Type", "Trackbar", &type, 4, track);
		createTrackbar("Handline", "Trackbar", &deger, 100, track);
		pMOG2->apply(idk, masked);
		cv::rectangle(masked, myRoi, cv::Scalar(0, 0, 255));
		track(0, 0);
		imshow("가위바위보", image);	// =imshow("묵찌빠 게임", image);
		imshow("Binary", masked);	//배경제거
		imshow("Blurred", med);

		if (key == 27) break;

	}
	
	return 0;
}



void track(int, void*) {
	int count;
	char a[40];
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	GaussianBlur(masked, masked, Size(27, 27), 3.5, 3.5);
	threshold(masked, masked, thresh, maxVal, type); //THRESH_BINARY + THRESH_OTSU
	Canny(masked, cannied, deger, deger * 2, 3); //OR2
	findContours(masked, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); //OR2
	Mat handline = Mat::zeros(cannied.size(), CV_8UC3); //cannied.size() or2.size()	= handline
	if (contours.size() > 0) {
		size_t indexOfBiggestContour = -1;
		size_t sizeOfBiggestContour = 0;
		for (size_t i = 0; i < contours.size(); i++) {
			if (contours[i].size() > sizeOfBiggestContour) {
				sizeOfBiggestContour = contours[i].size();
				indexOfBiggestContour = i;
			}
		}
		vector<vector<int> >hull(contours.size());
		vector<vector<Point> >hullPoint(contours.size()); //손의 움직임에 따라 손을 감싸는 다각형
		vector<vector<Vec4i> > defects(contours.size()); //손가락 끝의 녹색 점 .. 다차원 매트릭스
		vector<vector<Point> >defectPoint(contours.size()); //손가락 끝점 x, y를 점으로 유지
		vector<vector<Point> >contours_poly(contours.size()); //손을 감싸는 움직일 수있는 직사각형, contour polygon		
		Point2f rect_point[4];
		vector<RotatedRect>minRect(contours.size());
		vector<Rect> boundRect(contours.size());
		for (size_t i = 0; i < contours.size(); i++) {
			if (contourArea(contours[i]) > 5000) {
				convexHull(contours[i], hull[i], true);
				convexityDefects(contours[i], hull[i], defects[i]);
				if (indexOfBiggestContour == i) {
					minRect[i] = minAreaRect(contours[i]);
					for (size_t k = 0; k < hull[i].size(); k++) {
						int ind = hull[i][k];
						hullPoint[i].push_back(contours[i][ind]);
					}
					count = 0;

					for (size_t k = 0; k < defects[i].size(); k++) {
						if (defects[i][k][3] > 13 * 256) {
							int p_start = defects[i][k][0];
							int p_end = defects[i][k][1];
							int p_far = defects[i][k][2];
							defectPoint[i].push_back(contours[i][p_far]);
							circle(med, contours[i][p_end], 3, Scalar(0, 255, 0), 2); //i ydi
							count++;
						}

					}

					int user;

					if (count == 1) {
						user = 0;
						strcpy_s(a, "user: Rock");
					}
					else if (count == 2 || count == 3) {
						user = 1;
						strcpy_s(a, "user: Sissors");
					}
					else if (count == 5 || count == 6) {
						user = 2;
						strcpy_s(a, "user: Paper");
					}
					else {
						strcpy_s(a, "user: Cannot recognized");
					}

					putText(image, a, Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);

					srand(time(NULL));

					int com = rand() % 3; //랜덤 컴퓨터 가위바위보 결정.

					switch (com) {
					case 0: {
						pic = imread("묵.png", 1);
						imshow("가위바위보", pic);
						waitKey(20);
						break;
					}
					case 1: {
						pic = imread("찌.png", 1);
						imshow("가위바위보", pic);
						waitKey(20);
						break;
					}
					case 2: {
						pic = imread("빠.png", 1);
						imshow("가위바위보", pic);
						waitKey(20);
						break;
					}
					}

					if ((user + 1) % 3 == com) {
						strcpy_s(a, "Winner");
						//break;
					}
					else if ((com + 1) % 3 == user) {
						strcpy_s(a, "loser");
						//break;
					}
					else {
						strcpy_s(a, "again");
					}
					putText(image, a, Point(40, 40), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);




					drawContours(handline, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
					drawContours(handline, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
					drawContours(med, hullPoint, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
					approxPolyDP(contours[i], contours_poly[i], 3, false);
					boundRect[i] = boundingRect(contours_poly[i]);
					rectangle(med, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
					minRect[i].points(rect_point);
					for (size_t k = 0; k < 4; k++) {
						line(med, rect_point[k], rect_point[(k + 1) % 4], Scalar(0, 255, 0), 2, 8);
					}

				}
			}

		}

	}


	imshow("Contoured image", handline);

}