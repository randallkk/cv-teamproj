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

Mat cam, med1, med2;	// rgb to gray, hsv, ycbcr의 y 중 어떤 
Mat image;

int main(int argc, char** argv) {
	
	Rect handrect(288, 12, 288, 288);	// 손을 넣어야 할 직사각형
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));	// 
	

	VideoCapture cap(0);	// webcam 영상을 opencv로 넘겨주기
	if (!cap.isOpened()){
		printf("Can't open the camera");
		return -1;
	}

	while (1){
		cap >> cam;
		flip(cam, med1, 1);
		cvtColor(med1, med2, COLOR_RGB2GRAY);
		// mask = erode(mask, skinkernel, iterations = 1)
		// mask = dilate(mask, skinkernel, iterations = 1)
		GaussianBlur(med2, med2, Size(23, 23), 0);
		/* cvtColor(med1, med2, COLOR_RGB2HSV);	// HSV
		GaussianBlur(med2, med2, Size(23, 23), 0);
		cvtColor(med1, med2, COLOR_RGB2YCrCb);	// YCbCr
		GaussianBlur(med2, med2, Size(23, 23), 0); */


		track(0, 0);
		imshow("camera img", cam);
		if (waitKey(1) == 27)
			break;
	}

	return 0;
}

void track(int, void*) {
	int count = 0;
	char a[40];
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//threshold(fgMaskMOG2, or2, thresh, maxVal, type);
	//GaussianBlur(fgMaskMOG2, fgMaskMOG2, Size(11, 11), 3.5, 3.5);
	//threshold(fgMaskMOG2, or2, 10, 255, THRESH_OTSU);
	GaussianBlur(fgMaskMOG2, fgMaskMOG2, Size(27, 27), 3.5, 3.5);
	threshold(fgMaskMOG2, fgMaskMOG2, thresh, maxVal, type); //THRESH_BINARY + THRESH_OTSU
	//Canny(or2, kenarlar, deger, deger * 2, 3);
	Canny(fgMaskMOG2, kenarlar, deger, deger * 2, 3); //OR2
	findContours(fgMaskMOG2, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); //OR2
	Mat cizim = Mat::zeros(kenarlar.size(), CV_8UC3); //kenarlar.size() or2.size()
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
		vector<vector<Point> >hullPoint(contours.size()); // 손의 움직임에 따라 손을 감싸는 다각형
		vector<vector<Vec4i> > defects(contours.size()); // 손가락 끝의 녹색 점 .. 다차원 매트릭스
		vector<vector<Point> >defectPoint(contours.size()); // 손가락 끝점 x, y를 점으로 유지
		vector<vector<Point> >contours_poly(contours.size()); // 손을 감싸는 움직일 수있는 직사각형
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
							circle(griGoruntu, contours[i][p_end], 3, Scalar(0, 255, 0), 2); //i ydi
							count++;
						}

					}

					if (count == 0)
						// mouseevent 묵찌빠 귀납법으로 해야 할 듯?
					else if (count == 2)
						// mouseevent 묵찌빠
					else if (count == 5 || count == 6)
						// mouseevent 묵찌빠
					else
						strcpy_s(a, "인식할 수 없습니다.");

					putText(aynali, a, Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);

					drawContours(cizim, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
					drawContours(cizim, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
					drawContours(griGoruntu, hullPoint, i, Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
					approxPolyDP(contours[i], contours_poly[i], 3, false);
					boundRect[i] = boundingRect(contours_poly[i]);
					rectangle(griGoruntu, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2, 8, 0);
					minRect[i].points(rect_point);
					for (size_t k = 0; k < 4; k++) {
						line(griGoruntu, rect_point[k], rect_point[(k + 1) % 4], Scalar(0, 255, 0), 2, 8);
					}

				}
			}

		}

	}


	imshow("Sonuc", cizim);

}