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
		namedWindow("Original image");
		imshow("Original image", image);

		x = image.size().width / 4;
		y = image.size().height / 2;
		w = image.size().width / 2;
		h = image.size().height / 2;

		Rect rect(x, y, w, h);
		rectangle(image, rect, Scalar(0, 255, 0));
		rectimg = image(rect);	// ROI

		Mat contours, mask1, mask2, mask3, test, hsv;
		Scalar lower_white = Scalar(180, 180, 180); //흰색 차선 (RGB)
		Scalar upper_white = Scalar(255, 255, 255);
		inRange(rectimg, lower_white, upper_white, mask1);
		
		cvtColor(rectimg, hsv, COLOR_BGR2HSV); //hsv변경 (노랑잡기)
		Scalar lower_yellow = Scalar(10, 100, 100); //노란색 차선 (HSV)
		Scalar upper_yellow = Scalar(40, 255, 255);
		inRange(hsv, lower_yellow, upper_yellow, mask2);

		Scalar lower_blue = Scalar(110, 40, 70); //파란색 차선 (HSV)
		Scalar upper_blue = Scalar(150, 100, 130);
		inRange(hsv, lower_blue, upper_blue, mask3);

		addWeighted(mask1, 1.0, mask2, 1.0, 0.0, contours);

		addWeighted(contours, 1.0, mask3, 1.0, 0.0, contours);

		imshow("t", contours);

		//bitwise_and(image, hsv, test, mask3);

		//imshow("img", image);
		//imshow("blue_image", test);
		//imshow("blue_mask", mask3);

		Canny(rectimg, contours, 125, 350); 		// ROI에 캐니 알고리즘 적용
		// 선 감지 위한 허프 변환
		vector<Vec4i> lines;
		HoughLinesP(contours, lines, 1, PI / 180, 200, 0, 50); //  단계별 크기, 투표(vote) 최대 개수, 수에 따른 변화 관찰 필요 //400으로 바꿈~~

		// 선 그리기
		Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
		cout << "Lines detected: " << lines.size() << endl;		// 선 벡터를 반복해 선 그리기
		vector<Vec4i>::const_iterator it = lines.begin();
		while (it != lines.end()) {
			float rho = (*it)[0]; // 첫 번째 요소는 rho 거리
			float theta = (*it)[1]; // 두 번째 요소는 델타 각도
			if (theta < PI / 4. || theta > 3. * PI / 4.) { // 수직 행
				Point pt1(rho / cos(theta), 0); // 첫 행에서 해당 선의 교차점
				Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
				// 마지막 행에서 해당 선의 교차점
				line(rectimg, pt1, pt2, Scalar(255), 1); // 하얀 선으로 그리기
			}
			else { // 수평 행
				Point pt1(0, rho / sin(theta)); // 첫 번째 열에서 해당 선의 교차점
				Point pt2(result.cols, (rho - result.cols * cos(theta)) / sin(theta));
				// 마지막 열에서 해당 선의 교차점
				line(rectimg, pt1, pt2, Scalar(255), 1); // 하얀 선으로 그리기
			}
			cout << "line: (" << rho << "," << theta << ")\n";
			++it;
		}
		rectimg.copyTo(image(rect));	// ROI 원래 이미지에 복붙
		namedWindow("Detected Lines with Hough");
		imshow("Detected Lines with Hough", image);
		if (waitKey(1) == 27) {
			break;
		}
	}
	return 0;
}
