﻿#include <stdio.h>
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
Mat cam, med;	// cam=orjinalGoruntu, med=griGoruntu
Mat masked;	//fgMaskMOG2=masked
Mat idk, cannied, image;	// idk=kirpik, cannied=kenarlar, image=main 화면=image
Mat pic;    // 컴퓨터가 뭐 냈는지(가위바위보)

int thresh = 140, maxVal = 255;
int type = 1, deger = 8;

int count;

int main(int argc, char** argv) {

    Ptr< BackgroundSubtractor> pMOG2;
    pMOG2 = createBackgroundSubtractorMOG2();

    Rect handrect(288, 80, 288, 288);	// 손을 넣어야 할 직사각형 =myRoi
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));	// 원래 (3, 3)


    VideoCapture cap(0);	// webcam 영상을 opencv로 넘겨주기
    if (!cap.isOpened()) {
        printf("Can't open the camera");
        return -1;
    }

    while (1) {
        cap >> cam;
        flip(cam, image, 1);
        rectangle(image, handrect, Scalar(0, 0, 255));
        idk = image(handrect);
        cvtColor(idk, med, COLOR_RGB2GRAY);
        // mask = erode(mask, skinkernel, iterations = 1)
        // mask = dilate(mask, skinkernel, iterations = 1)
        GaussianBlur(med, med, Size(23, 23), 0);
        /* cvtColor(med1, med, COLOR_RGB2HSV);	// HSV model
        GaussianBlur(med, med, Size(23, 23), 0);
        cvtColor(med1, med, COLOR_RGB2YCrCb);	// YCbCr model
        GaussianBlur(med, med, Size(23, 23), 0); */
        pMOG2->apply(idk, masked);

        int user=100;
        char a[40];
        // 가위바위보 게임

        track(0, 0);

        if (::count == 0) {
            user = 0;
            strcpy_s(a, "Rock");
        }
        else if (::count == 2) {
            user = 1;
            strcpy_s(a, "Sissors");
        }
        else if (::count == 5) {
            user = 2;
            strcpy_s(a, "Paper");
        }
        else {
            strcpy_s(a, "Cannot recognized");
            putText(image, a, Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
        }

        srand(time(NULL));

        ////string say;
        ////putText(image, say, Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
        ////waitKey(1);

        int com = rand() % 3; //랜덤 컴퓨터 가위바위보 결정.

        switch (com) {
        case 0: {
            pic = imread("묵.png", 1);
            namedWindow("묵");
            imshow("묵", pic);
            waitKey(20);
        }
        case 1: {
            pic = imread("찌.png", 1);
            namedWindow("찌");
            imshow("찌", pic);
            waitKey(20);
        }
        case 2: {
            pic = imread("빠.png", 1);
            namedWindow("빠");
            imshow("빠", pic);
            waitKey(20);
        }
        }

        if ((user + 1) % 3 == com) {
            putText(image, "winner", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);; //유저 승리
            //break;
        }
        else if ((com + 1) % 3 == user) {
            putText(image, "loser", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);; //컴 승리
            //break;
        }
        else {
            putText(image, "again", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false); //무승부
        }

        imshow("가위바위보", image);
        imshow("Blurred", med);
        if (waitKey(1) == 27)
            break;
    }

    return 0;
}

void track(int, void*) {
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
    Mat handline = Mat::zeros(cannied.size(), CV_8UC3); //cannied.size() or2.size()
    if (contours.size() > 0) {
        size_t indexOfBiggestContour = -1;
        size_t sizeOfBiggestContour = 0;
        for (size_t i = 0; i < contours.size(); i++) {
            if (contours[i].size() > sizeOfBiggestContour) {
                sizeOfBiggestContour = contours[i].size();
                indexOfBiggestContour = i;
            }
        }
        vector<vector<int> > hull(contours.size());
        vector<vector<Point> > hullPoint(contours.size()); // 손의 움직임에 따라 손을 감싸는 다각형
        vector<vector<Vec4i> > defects(contours.size()); // 손가락 끝의 녹색 점 .. 다차원 매트릭스
        vector<vector<Point> > defectPoint(contours.size()); // 손가락 끝점 x, y를 점으로 유지
        vector<vector<Point> > contours_poly(contours.size()); // 손을 감싸는 움직일 수있는 직사각형, contour polygon
        Point2f rect_point[4];
        vector<RotatedRect> minRect(contours.size());
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
                    ::count = 0;

                    for (size_t k = 0; k < defects[i].size(); k++) {
                        if (defects[i][k][3] > 13 * 256) {
                            int p_start = defects[i][k][0];
                            int p_end = defects[i][k][1];
                            int p_far = defects[i][k][2];
                            defectPoint[i].push_back(contours[i][p_far]);
                            circle(med, contours[i][p_end], 3, Scalar(0, 255, 0), 2); //i ydi
                            ::count++;
                        }

                    }
                }

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
    imshow("Contoured image", handline);

}