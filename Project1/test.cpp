#include <opencv2/core/core.hpp>
#include <opencv2\opencv.hpp> // opencv �⺻���� API�� ����ִ� �������
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define PI 3.141592

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	//���� ����
	VideoCapture capture;
	capture.open("highway.mp4");

	if (!capture.isOpened())
		return -1;

	cout << "Device opening..." << endl;
	namedWindow("File Play", WINDOW_AUTOSIZE);
	Mat frame;
	vector<Vec2f> lines;

	for (;;) {
		//Mat frame;
		Mat thres, canny;
		Mat Roi1, Roi2;

		capture >> frame;

		imshow("File Play", frame);
		if (waitKey(30) >= 0)  //27�� Esc, 32�� Space key
			break;

		cvtColor(frame, thres, CV_BGR2GRAY);
		GaussianBlur(thres, thres, Size(3, 3), 0, 3);
		Canny(thres, canny, 85, 110, 3);
		imshow("canny", canny);
		printf("%d, %d\n", frame.rows, frame.cols);

		Roi1 = canny(Rect(0, 2 * frame.rows / 3, frame.cols, 1 * frame.rows / 3));
		//Roi2 = canny(Rect(frame.cols / 2, 0, frame.cols / 2, frame.rows));

		imshow("Roi1", Roi1);
		//imshow("Roi2", Roi2);

		// �� ���� ���� ���� ��ȯ
		HoughLines(Roi1, lines, 1, PI / 180, 200);

		// �� �׸���
		Mat result(canny.rows, canny.cols, CV_8U, Scalar(255));
		cout << "Lines detected: " << lines.size() << endl;

		// �����͸��ݺ��ؼ��׸���
		vector<Vec2f>::const_iterator it = lines.begin();
		while (it != lines.end()) {
			float rho = (*it)[0]; // ù��°��Ҵ�rho �Ÿ�
			float theta = (*it)[1]; // �ι�°��Ҵ� ��Ÿ����
			if (theta < PI / 4. || theta > 3.*PI / 4.) { // ������
				Point pt1(rho / cos(theta), 0); // ù�࿡���ش缱�Ǳ�����
				Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
				pt1.y = pt1.y + 2 * frame.rows / 3;
				pt2.y = pt2.y + 2 * frame.rows / 3;

				// �������࿡���ش缱�Ǳ�����
				line(frame, pt1, pt2, Scalar(255), 1); // �Ͼἱ���α׸���
			}
			else { // ������
				Point pt1(0, rho / sin(theta)); // ù��°�������ش缱�Ǳ�����
				Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));
				pt1.y = pt1.y + 2 * frame.rows / 3;
				pt2.y = pt2.y + 2 * frame.rows / 3;

				// �������������ش缱�Ǳ�����
				line(frame, pt1, pt2, Scalar(255), 1); // �Ͼἱ���α׸���
			}
			//cout << "line: (" << rho << "," << theta << ")\n";
			++it;
		}

		/*
		HoughLinesP(canny, lines, 1, CV_PI / 180, 30, 30, 3);
		for (int i = 0; i < lines.size(); i++) {
		Vec4i L = lines[i];
		line(frame, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 5);
		}
		*/

		imshow("Example", frame);

	}

}