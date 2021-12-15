#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>


using namespace std;
using namespace cv;

CascadeClassifier cat_cascade;
int cats = 0;
vector<Rect> last_frame_cats;
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

		vector<Rect> detected_cats;
		Mat frame_gray;
		
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		cat_cascade.detectMultiScale(frame_gray, detected_cats, 1.1, 4, 0, Size(100, 100));

		vector<Rect> this_frame_cats;
		for (size_t i = 0; i < detected_cats.size(); i++) {
			
			Rect detected_cat = Rect(Point(detected_cats[i].x, detected_cats[i].y), Point(detected_cats[i].x + detected_cats[i].width, detected_cats[i].y + detected_cats[i].height));
			rectangle(frame, Point(detected_cats[i].x, detected_cats[i].y), 
						Point(detected_cats[i].x + detected_cats[i].width, detected_cats[i].y + detected_cats[i].height),
						Scalar(0, 0, 255), 4, LINE_8);
			putText(frame, "Kot nr" + to_string(i + 1), Point(detected_cats[i].x, detected_cats[i].y + detected_cats[i].height + 20), FONT_HERSHEY_SIMPLEX, 0.55, Scalar(0, 255, 255), 2);
			int similar_cats = 0;
			for (auto& last_frame_cat : last_frame_cats) {
				if (abs(detected_cat.x - last_frame_cat.x) <= 250 &&
					abs(detected_cat.y - last_frame_cat.y) <= 250 &&
					abs(detected_cat.width - last_frame_cat.width) <= 250 &&
					abs(detected_cat.height - last_frame_cat.height) <= 250) {
					similar_cats++;
				}
			}
			if (similar_cats == 0) {
				cats++;
			}
			this_frame_cats.push_back(detected_cat);
		}

		putText(frame, "Widoczne koty: " + to_string(cats), Point(10, 25), FONT_HERSHEY_SIMPLEX, 0.80, Scalar(0, 0, 255), 2);



		imshow("frame", frame);

		last_frame_cats = this_frame_cats;


		int key_pressed = waitKey(15);
		if (key_pressed == 27) {
			break;
		}
	}
	return 0;
}


