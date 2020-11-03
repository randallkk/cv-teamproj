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
Mat cam;
Mat masked;	// masked=masked
Mat med, idk, or2, cannied, image;	//griGoruntu=med, kirpik=idk, aynali=main 화면=image
int thresh = 140, maxVal = 255;
int type = 1, deger = 8;


int main() {
	Ptr< BackgroundSubtractor> pMOG2;
	pMOG2 = createBackgroundSubtractorMOG2();
	cv::Rect myRoi(288, 12, 288, 288);	//myRoi=handrect
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
	Mat frame;
	Mat resizeF;
	VideoCapture cap;
	cap.open(0);
	while (1)
	{
		Mat aynali2;
		cap >> cam;
		cv::flip(cam, image, 1);
		//cv::rectangle(cam, cv::Point(300, 300), cv::Point(12, 12), cv::Scalar(0, 0, 255));
		cv::rectangle(image, myRoi, cv::Scalar(0, 0, 255));
		idk = image(myRoi);
		cvtColor(idk, med, COLOR_RGB2GRAY);
		//equalizeHist(med, med);
		GaussianBlur(med, med, Size(23, 23), 0); //35,35	//15,15
		//threshold(med, or2, thresh, maxVal, THRESH_OTSU + CV_THRESH_BINARY_INV);
		namedWindow("ayarla");
		createTrackbar("Esik", "ayarla", &thresh, 250, track);	// thresh=threshold, maxVal=, type=, deger=degree
		createTrackbar("Maksimum", "ayarla", &maxVal, 255, track);
		createTrackbar("Esik Tipi", "ayarla", &type, 4, track);
		createTrackbar("Kenarlar", "ayarla", &deger, 100, track);
		pMOG2->apply(idk, masked);
		cv::rectangle(masked, myRoi, cv::Scalar(0, 0, 255));

		//cv::flip(masked, aynali2, 1);

		track(0, 0);
		imshow("ORJINAL Goruntu", image);	// =imshow("묵찌빠 게임", image);
		imshow("ArkaPlan Kaldırıldı", masked);	//배경제거
		imshow("Gri", med);	// =imshow("Blurred", med2);




		char key = waitKey(24);
		if (key == 27) break;
	}

	return 0;
}



void track(int, void*) {
	int count = 0;
	char a[40];
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//threshold(masked, or2, thresh, maxVal, type);
	//GaussianBlur(masked, masked, Size(11, 11), 3.5, 3.5);
	//threshold(masked, or2, 10, 255, THRESH_OTSU);
	GaussianBlur(masked, masked, Size(27, 27), 3.5, 3.5);
	threshold(masked, masked, thresh, maxVal, type); //THRESH_BINARY + THRESH_OTSU
	//Canny(or2, cannied, deger, deger * 2, 3);
	Canny(masked, cannied, deger, deger * 2, 3); //OR2
	findContours(masked, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); //OR2
	Mat cizim = Mat::zeros(cannied.size(), CV_8UC3); //cannied.size() or2.size()	= handline
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
		vector<vector<Point> >hullPoint(contours.size()); //elin hareketine göre eli çevreleyen çokgen	
		vector<vector<Vec4i> > defects(contours.size()); //parmak uclarindaki yesil noktalar..multi dimensional matrix
		vector<vector<Point> >defectPoint(contours.size()); //point olarak parmak ucu noktalarýný x,y olarak tutuyor
		vector<vector<Point> >contours_poly(contours.size()); //eli çevreleyen hareketli dikdörtgen		
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

					if (count == 1)
						strcpy_s(a, "1");
					else if (count == 2)
						strcpy_s(a, "2");
					else if (count == 3)
						strcpy_s(a, "3");
					else if (count == 4)
						strcpy_s(a, "4");
					else if (count == 5 || count == 6)
						strcpy_s(a, "5");
					else
						strcpy_s(a, "EL GOSTER");

					putText(image, a, Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);

					drawContours(cizim, contours, i, Scalar(255, 255, 0), 2, 8, vector<Vec4i>(), 0, Point());
					drawContours(cizim, hullPoint, i, Scalar(255, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
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


	imshow("Sonuc", cizim);

}