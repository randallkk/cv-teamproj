#include <iostream>
#include <string.h>
#include <ctime>

using namespace std;

int main()
{
    srand(time(NULL));
    rand(); //·£´ýÇÔ¼ö

    int A, B; //¼±ÈÄÀ§ÇÑ º¯¼ö
    
    int com = rand() % 3 + 1; //·£´ý ÄÄÇ»ÅÍ ¹¬Âîºü °áÁ¤.
    
    int user; 
    cin >> user; //»ç¿ëÀÚ ¹¬Âîºü ÀÔ·Â¹Þ±â

    switch (com) {
    case 0: {
        image = imread("¹¬.jpg", 1);
        namedWindow("¹¬");
        imshow("¹¬", image);
        break;
    }
    case 1: {
        image = imread("Âî.jpg", 1);
        namedWindow("Âî");
        imshow("Âî", image);
        break;
    }
    case 2: {
        image = imread("ºü.jpg", 1);
        namedWindow("ºü");
        imshow("ºü", image);
        break;
    }
    }

    //¼±ÈÄ °áÁ¤ÇÏ±â
    while (1) {
        if ((user + 1) % 3 == com) {
            A = 1, B = 0; //À¯Àú °ø°Ý
            break;
        }
        else if ((com + 1) % 3 == user) {
            A = 0, B = 1; //ÄÄ °ø°Ý
            break;
        }
        else {
            com = rand() % 3 + 1;
            cin >> user;
            cout << "¹«½ÂºÎ";
        }
    }

    while (1) {
        if (A == 1 && B == 0) {
            cout << "À¯Àú °ø°Ý";
            com = rand() % 3 + 1;
            cin >> user;

            switch (com) {
            case 0: {
                image = imread("¹¬.jpg", 1);
                namedWindow("¹¬");
                imshow("¹¬", image);
                break;
            }
            case 1: {
                image = imread("Âî.jpg", 1);
                namedWindow("Âî");
                imshow("Âî", image);
                break;
            }
            case 2: {
                image = imread("ºü.jpg", 1);
                namedWindow("ºü");
                imshow("ºü", image);
                break;
            }
            }

            if (user == com) {
                cout << "winner";
            }
            else {
                com = rand() % 3 + 1;
                cin >> user;
                A == 1 && B == 0;
                cout << "´Ù½Ã";
                continue;
            }
        }
        else{
            cout << "ÄÄ °ø°Ý";
            com = rand() % 3 + 1;
            cin >> user;

            switch (com) {
            case 0: {
                image = imread("¹¬.jpg", 1);
                namedWindow("¹¬");
                imshow("¹¬", image);
                break;
            }
            case 1: {
                image = imread("Âî.jpg", 1);
                namedWindow("Âî");
                imshow("Âî", image);
                break;
            }
            case 2: {
                image = imread("ºü.jpg", 1);
                namedWindow("ºü");
                imshow("ºü", image);
                break;
            }
            }

            if (user == com) {
                cout << "loser";
            }
            else {
                com = rand() % 3 + 1;
                cin >> user;
                A == 0 && B == 1;
                cout << "´Ù½Ã";
                continue;
            }
        }
    }
}