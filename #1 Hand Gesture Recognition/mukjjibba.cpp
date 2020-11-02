#include <iostream>
#include <string.h>
#include <ctime>

using namespace std;

int game(int user)
{
    srand(time(NULL));
    rand(); //랜덤함수

    int A, B; //선후위한 변수

    putText(image, "준비", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
    waitkey(10);
    putText(image, "시작", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
    waitkey(10);

    int com = rand() % 3 + 1; //랜덤 컴퓨터 묵찌빠 결정.

     //사용자 묵찌빠 입력받기

    while (1) {
        if (count == 0)
            user = 0;
        else if (count == 2)
            user = 1;
        else if (count == 5)
            user = 2;
        else
            continue;
    }
   
    switch (com) {
    case 0: {
        image = imread("묵.png", 1);
        namedWindow("묵");
        imshow("묵", image);
        waitkey(20);
        break;
    }
    case 1: {
        image = imread("찌.png", 1);
        namedWindow("찌");
        imshow("찌", image);
        waitkey(20);
        break;
    }
    case 2: {
        image = imread("빠.png", 1);
        namedWindow("빠");
        imshow("빠", image);
        waitkey(20);
        break;
    }
    }

    //선후 결정하기
    while (1) {
        if ((user + 1) % 3 == com) {
            A = 1, B = 0; //유저 공격
            break;
        }
        else if ((com + 1) % 3 == user) {
            A = 0, B = 1; //컴 공격
            break;
        }
        else {
            com = rand() % 3 + 1;

            while (1) {
                if (count == 0)
                    user = 0;
                else if (count == 2)
                    user = 1;
                else if (count == 5)
                    user = 2;
                else
                    continue;
            }
            putText(image, "다시", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
            //cout << "무승부";
        }
    }

    while (1) {
        if (A == 1 && B == 0) {
            putText(image, "당신의 공격", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
            //cout << "유저 공격";
            com = rand() % 3 + 1;

            while (1) {
                if (count == 0)
                    user = 0;
                else if (count == 2)
                    user = 1;
                else if (count == 5)
                    user = 2;
                else
                    continue;
            }

            switch (com) {
            case 0: {
                image = imread("묵.png", 1);
                namedWindow("묵");
                imshow("묵", image);
                waitkey(20);
                break;
            }
            case 1: {
                image = imread("찌.png", 1);
                namedWindow("찌");
                imshow("찌", image);
                waitkey(20);
                break;
            }
            case 2: {
                image = imread("빠.png", 1);
                namedWindow("빠");
                imshow("빠", image);
                waitkey(20);
                break;
            }
            }

            if (user == com) {
                putText(image, "winner", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
                //cout << "winner";
            }
            else {
                com = rand() % 3 + 1;

                while (1) {
                    if (count == 0)
                        user = 0;
                    else if (count == 2)
                        user = 1;
                    else if (count == 5)
                        user = 2;
                    else
                        continue;
                }
                A == 1 && B == 0;
                putText(image, "다시", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
                //cout << "다시";
                continue;
            }
        }
        else{
            putText(image, "컴 공격", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
            //cout << "컴 공격";
            com = rand() % 3 + 1;

            while (1) {
                if (count == 0)
                    user = 0;
                else if (count == 2)
                    user = 1;
                else if (count == 5)
                    user = 2;
                else
                    continue;
            }

            switch (com) {
            case 0: {
                image = imread("묵.png", 1);
                namedWindow("묵");
                imshow("묵", image);
                waitkey(20);
                break;
            }
            case 1: {
                image = imread("찌.png", 1);
                namedWindow("찌");
                imshow("찌", image);
                waitkey(20);
                break;
            }
            case 2: {
                image = imread("빠.png", 1);
                namedWindow("빠");
                imshow("빠", image);
                waitkey(20);
                break;
            }
            }

            if (user == com) {
                putText(image, "loser", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
                //cout << "loser";
            }
            else {
                com = rand() % 3 + 1;

                while (1) {
                    if (count == 0)
                        user = 0;
                    else if (count == 2)
                        user = 1;
                    else if (count == 5)
                        user = 2;
                    else
                        continue;
                }
                A == 0 && B == 1;
                putText(image, "다시", Point(75, 450), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 255, 0), 3, 8, false);
                //cout << "다시";
                continue;
            }
        }
    }
}
