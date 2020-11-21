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
		rectangle(image, rect, Scalar(255, 0, 0));
		rectimg = image(rect);
		// 캐니 알고리즘 적용
		Mat contours;
		Canny(rectimg, contours, 125, 350);
		// 선 감지 위한 허프 변환
		vector<Vec2f> lines;
		HoughLines(contours, lines, 1, PI / 180, 400); //  단계별 크기, 투표(vote) 최대 개수, 수에 따른 변화 관찰 필요 //400으로 바꿈~~
													  
		// 선 그리기
		Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
		cout << "Lines detected: " << lines.size() << endl;		// 선 벡터를 반복해 선 그리기
		vector<Vec2f>::const_iterator it = lines.begin();
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
		rectimg.copyTo(image(rect));
		namedWindow("Detected Lines with Hough");
		imshow("Detected Lines with Hough", image);
		if (waitKey(1) == 27) {
			break;
		}
	}
	return 0;
}
