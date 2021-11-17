#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
int main(int argc, char** argv) {
	using namespace cv;

	int h1 = 0;
	int h2 = 255;
	int s1 = 0;
	int s2 = 255;
	int v1 = 0;
	int v2 = 255;

	VideoCapture cap1("nature.mp4");
	if (!cap1.isOpened())
		return -1;

	namedWindow("HSV", cv::WINDOW_NORMAL);
	createTrackbar("h1", "HSV", &h1, 255);
	createTrackbar("h2", "HSV", &h2, 255);
	createTrackbar("s1", "HSV", &s1, 255);
	createTrackbar("s2", "HSV", &s2, 255);
	createTrackbar("v1", "HSV", &v1, 255);
	createTrackbar("v2", "HSV", &v2, 255);

	while (true) {
		Mat og;
		cap1.read(og);
		GaussianBlur(og, og, Size(5, 5), 0);
		resize(og, og, Size(320, 200));

		Mat hsv;
		cvtColor(og, hsv, COLOR_BGR2HSV);

		imshow("obraz", hsv);

		if (waitKey(1) == 27)
			break;
	}
}