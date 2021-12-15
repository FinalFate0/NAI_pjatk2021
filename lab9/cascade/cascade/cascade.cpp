#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace std;
using namespace cv;

CascadeClassifier cat_cascade;

int main(void) {
	VideoCapture capture("cats3.mp4");
	Mat frame;
	if (!cat_cascade.load("haarcascade_frontalcatface_extended.xml")) {
		return -1;
	}
	if (!capture.isOpened()) {
		return -2;
	}

	while (capture.read(frame)) {
		if (frame.empty()) return -1;

		vector<Rect> cat;
		Mat frame_gray;

		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		cat_cascade.detectMultiScale(frame_gray, cat, 1.1, 3, 0, Size(100, 100));

		int cats = 0;
		for (size_t i = 0; i < cat.size(); i++) {
			rectangle(frame, Point(cat[i].x, cat[i].y), Point(cat[i].x + cat[i].width, cat[i].y + cat[i].height), Scalar(0, 0, 255), 4, LINE_8);
			putText(frame, "Kot nr" + to_string(i + 1), Point(cat[i].x, cat[i].y + cat[i].height + 20), FONT_HERSHEY_SIMPLEX, 0.55, Scalar(0, 255, 255), 2);
			cats++;
		}

		putText(frame, "Widoczne koty: " + to_string(cats), Point(10, 25), FONT_HERSHEY_SIMPLEX, 0.80, Scalar(0, 0, 255), 2);



		imshow("frame", frame);

		int key_pressed = waitKey(15);
		if (key_pressed == 27) {
			break;
		}
	}
	return 0;
}


