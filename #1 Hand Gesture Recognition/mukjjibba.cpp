#include <iostream>
#include <string.h>
#include <ctime>

using namespace std;

int main()
{
    srand(time(NULL));
    rand(); //�����Լ�

    int A, B; //�������� ����
    
    int com = rand() % 3 + 1; //���� ��ǻ�� ����� ����.
    
    int user; 
    cin >> user; //����� ����� �Է¹ޱ�

    switch (com) {
    case 0: {
        image = imread("��.jpg", 1);
        namedWindow("��");
        imshow("��", image);
        break;
    }
    case 1: {
        image = imread("��.jpg", 1);
        namedWindow("��");
        imshow("��", image);
        break;
    }
    case 2: {
        image = imread("��.jpg", 1);
        namedWindow("��");
        imshow("��", image);
        break;
    }
    }

    //���� �����ϱ�
    while (1) {
        if ((user + 1) % 3 == com) {
            A = 1, B = 0; //���� ����
            break;
        }
        else if ((com + 1) % 3 == user) {
            A = 0, B = 1; //�� ����
            break;
        }
        else {
            com = rand() % 3 + 1;
            cin >> user;
            cout << "���º�";
        }
    }

    while (1) {
        if (A == 1 && B == 0) {
            cout << "���� ����";
            com = rand() % 3 + 1;
            cin >> user;

            switch (com) {
            case 0: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
                break;
            }
            case 1: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
                break;
            }
            case 2: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
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
                cout << "�ٽ�";
                continue;
            }
        }
        else{
            cout << "�� ����";
            com = rand() % 3 + 1;
            cin >> user;

            switch (com) {
            case 0: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
                break;
            }
            case 1: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
                break;
            }
            case 2: {
                image = imread("��.jpg", 1);
                namedWindow("��");
                imshow("��", image);
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
                cout << "�ٽ�";
                continue;
            }
        }
    }
}