#define _USE_MATH_DEFINES

#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\opencv.hpp> 
#include <cmath>

using namespace cv;
using namespace std;

double PI = M_PI;

int main(int argc, char** argv)
{
    VideoCapture cap(argv[1]);

    if (!cap.isOpened())
    {
        printf("Can't open the video");
        return -1;
    }

    Mat image, rectimg;
    int x, y, w, h;


    while (1)
    {
        cap >> image;

        if (image.empty())
        {
            printf("empty image");
            return 0;
        }
        namedWindow("Original image", WINDOW_NORMAL);
        imshow("Original image", image);
        x = 0;
        //x = image.size().width / 8;
        y = image.size().height / 5 * 3;
        w = image.size().width;
        h = image.size().height / 5 * 2;

        Rect rect(x, y, w, h);
        rectangle(image, rect, Scalar(0, 255, 0));
        rectimg = image(rect);   // ROI
        GaussianBlur(rectimg, rectimg, Size(3, 3), 0);
        imshow("gaussian", rectimg);


        Mat contours, mask1, mask2, mask3, test, hsv, color;

        Scalar lower_white = Scalar(130, 130, 130);
        Scalar upper_white = Scalar(255, 255, 255);
        inRange(rectimg, lower_white, upper_white, mask1);
        /*bitwise_and(rectimg, rectimg, test, mask1);
        imshow("white", test);*/


        cvtColor(rectimg, hsv, COLOR_BGR2HSV); //hsv����

        for (int y = 0; y < hsv.rows; y++) {   // ä�� 255�� ����
            for (int x = 0; x < hsv.cols; x++) {
                hsv.at<Vec3b>(y, x)[1] = 255;
                //if(hsv.at<Vec3b>(y, x)[1] > 40) hsv.at<Vec3b>(y, x)[1] = 255;
                //hsv.at<Vec3b>(y, x)[1] = saturate_cast<uchar>((hsv.at<Vec3b>(y, x)[1]) * 2);
                //if (hsv.at<Vec3b>(y, x)[1] > 255) hsv.at<Vec3b>(y, x)[1] = 255;
            }
        }

        // Scalar lower_yellow = Scalar(10, 100, 100); //����� ���� (HSV)
        // Scalar upper_yellow = Scalar(40, 255, 255);
        Scalar lower_yellow = Scalar(25, 170, 100); //����� ���� (HSV)
        Scalar upper_yellow = Scalar(35, 255, 200);
        /*inRange(hsv, lower_yellow, upper_yellow, mask2);*/
        //bitwise_and(rectimg, hsv, test, mask2);
        //imshow("img", test);

        // Scalar lower_blue = Scalar(90, 40, 70); //�Ķ��� ���� (HSV)
        //Scalar upper_blue = Scalar(120, 100, 150);
        Scalar lower_blue = Scalar(90, 255, 70); //�Ķ��� ���� (HSV)
        Scalar upper_blue = Scalar(120, 255, 150);
        inRange(hsv, lower_blue, upper_blue, mask3);
        bitwise_and(rectimg, hsv, test, mask3);
        imshow("blue", test);


        addWeighted(mask1, 1.0, mask3, 1.0, 0.0, color);
        //addWeighted(color, 1.0, mask2, 1.0, 0.0, color);
        imshow("color", color);

        //erode(contours, contours, Mat());
        Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(color, color, MorphTypes::MORPH_CLOSE, element);
        morphologyEx(color, color, MorphTypes::MORPH_OPEN, element);

        //imshow("contour", contours);

        //bitwise_and(image, hsv, test, mask3);
        //imshow("img", image);
        //imshow("blue_image", test);
        //imshow("blue_mask", mask3);

        Canny(color, contours, 125, 350);       // ROI�� ĳ�� �˰��� ����
        // �� ���� ���� ���� ��ȯ
        imshow("caany", contours);
        vector<Vec4i> lines; //������ ������ �� ���� ����
        HoughLinesP(contours, lines, 1, PI / 180, 10, 0, 0); //  �ܰ躰 ũ��, ��ǥ(vote) �ִ� ����, ���� ���� ��ȭ ���� �ʿ� //400���� �ٲ�~~

        // �� �׸���
        Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
        cout << "Lines detected: " << lines.size() << endl;      // �� ���͸� �ݺ��� �� �׸���
        vector<Vec4i>::const_iterator it = lines.begin();


        while (it != lines.end()) {
            cv::Point pt1((*it)[0], (*it)[1]);
            cv::Point pt2((*it)[2], (*it)[3]);
            cv::line(rectimg, pt1, pt2, (255), 1);
            ++it;
        }

        rectimg.copyTo(image(rect));   // ROI ���� �̹����� ����
        namedWindow("Detected Lines with Hough", WINDOW_NORMAL);
        imshow("Detected Lines with Hough", image);
        if (waitKey(1) == 27) {
            break;
        }
    }
    return 0;
}