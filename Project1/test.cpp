#include <opencv2/core/core.hpp>
#include <opencv2\opencv.hpp> // opencv 기본적인 API가 들어있는 헤더파일
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define PI 3.141592

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	//차선 검출
	VideoCapture capture;
	capture.open("highway.mp4");

	if (!capture.isOpened())
		return -1;

	cout << "Device opening..." << endl;
	namedWindow("File Play", WINDOW_AUTOSIZE);
	Mat frame;
	vector<Vec2f> lines;
	Point banishP;
	Point pt1, pt2;
	vector<Vec2f>linesR;
	float resultLine[2];

	for (;;) {
		//Mat frame;
		Mat thres, canny;
		Mat Roi1, Roi2, Roi;

		capture >> frame;

		imshow("File Play", frame);
		if (waitKey(30) >= 0)  //27은 Esc, 32는 Space key
			break;

		cvtColor(frame, thres, CV_BGR2GRAY);
		GaussianBlur(thres, thres, Size(3, 3), 0, 3);
		Canny(thres, canny, 85, 110, 3);
		imshow("canny", canny);
		printf("%d, %d\n", frame.rows, frame.cols);

		Roi1 = canny(Rect(0, 0, frame.cols / 2, frame.rows));
		Roi2 = canny(Rect(frame.cols / 2, 0, frame.cols / 2, frame.rows));
		Roi = canny(Rect(0, 3 * frame.rows / 5, frame.cols, 2 * frame.rows / 5));

		//imshow("Roi1", Roi1);
		//imshow("Roi2", Roi2);
		imshow("Roi", Roi);

		// 선 감지 위한 허프 변환
		HoughLines(Roi, lines, 1, PI / 180, 200);

		// 선 그리기
		Mat result(canny.rows, canny.cols, CV_8U, Scalar(255));
		cout << "Lines detected: " << lines.size() << endl;

		// 선벡터를반복해선그리기
		vector<Vec2f>::const_iterator it = lines.begin();

		while (it != lines.end()) {
			float rho = (*it)[0]; // 첫번째요소는rho 거리
			float theta = (*it)[1]; // 두번째요소는 델타각도
			if (theta < PI || theta > PI / 2) { // 수직행
				Point pt1(rho / cos(theta), 0); // 첫행에서해당선의교차점
				Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
				// 마지막행에서해당선의교차점
				pt1.y = pt1.y + 3 * result.rows / 5;
				pt2.y = pt2.y + 3 * result.rows / 5;
				//circle(frame, pt1, 3, Scalar(255, 0, 0), 100);
				//circle(frame, pt2, 3, Scalar(255, 0, 255), 100);
				line(frame, pt1, pt2, Scalar(255, 0, 0), 5); // 하얀선으로그리기
			}

			else
			{ // 수평행
				Point pt1(0, rho / sin(theta)); // 첫번째열에서해당선의교차점
				Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));
				// 마지막열에서해당선의교차점
				pt1.y = pt1.y + 3 * result.rows / 5;
				pt2.y = pt2.y + 3 * result.rows / 5;
				//circle(frame, pt1, 3, Scalar(255, 255, 0), 100);
				//circle(frame, pt2, 3, Scalar(255, 255, 255), 100);
				line(frame, pt1, pt2, Scalar(0, 0, 0), 5); // 하얀선으로그리기
			}




			cout << "line: (" << rho << "," << theta << ")\n";

			++it;
		}


		//HoughLinesP(canny, lines, 1, CV_PI / 180, 30, 30, 3);
		//for (int i = 0; i < lines.size(); i++) {
		   //Vec4i L = lines[i];
		   //line(frame, Point(L[0], L[1]), Point(L[2], L[3]), Scalar(0, 0, 255), 5);
		//}


		imshow("Example", frame);


	}

}