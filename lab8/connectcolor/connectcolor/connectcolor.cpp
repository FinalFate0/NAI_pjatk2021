#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
int main(int argc, char** argv) {
	using namespace cv;

	int h1 = 0;
	int h1b = 170;
	int h2 = 10;
	int h2b = 180;
	int s1 = 60;
	int s2 = 255;
	int v1 = 60;
	int v2 = 255;

	VideoCapture cap1("snooker.mp4");
	if (!cap1.isOpened())
		return -1;

	int framecount = 0;
	while (true) {
		Mat frame;
		//frame = imread("Untitled4.jpg");
		cap1.read(frame);
		framecount++;

		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Mat mask1, mask2;
		inRange(hsv, Scalar(h1, s1, v1), Scalar(h2, s2, v2), mask1);
		inRange(hsv, Scalar(h1b, s1, v1), Scalar(h2b, s2, v2), mask2);
		Mat mask = mask1 | mask2;


		int dilation_size = 8;
		auto structElem = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * dilation_size + 1, 2 * dilation_size + 1),
			Point(dilation_size, dilation_size));
		erode(mask, mask, structElem);
		dilate(mask, mask, structElem);

		Mat output;
		bitwise_and(hsv, hsv, output, mask);

		std::vector<std::vector<Point>> contours;
		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		std::sort(contours.begin(), contours.end(), [](auto& a, auto& b) {
			return contourArea(a, false) > contourArea(b, false);
			});

		
		if ((contours.size() > 0) && (contourArea(contours[0], false) > 10000)) {
			drawContours(frame, contours, -1, 255, 3);
		}
		double biggest1 = 0;
		double biggest2 = 0;
		Rect bounding_rect1, bounding_rect2;
		int biggest_idx1 = 0;
		int biggest_idx2 = 0;

		for (size_t i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i]);
			if (area > biggest1) {
				biggest1 = area;
				biggest_idx1 = i;
				bounding_rect1 = boundingRect(contours[i]);
			}
			else if (area > biggest2) {
				biggest2 = area;
				biggest_idx2 = i;
				bounding_rect2 = boundingRect(contours[i]);
			}
		}

		drawContours(frame, contours, biggest_idx1, Scalar(0,0,255), 3);
		drawContours(frame, contours, biggest_idx2, Scalar(0,0,255), 3);

		auto cx1 = bounding_rect1.x + bounding_rect1.width / 2; 
		auto cy1 = bounding_rect1.y + bounding_rect1.height / 2;

		auto cx2 = bounding_rect2.x + bounding_rect2.width / 2;
		auto cy2 = bounding_rect2.y + bounding_rect2.height / 2;


		if (std::abs(cy1 - cy2) < 100.0) {
			line(frame, Point(cx1, cy1), Point(cx2, cy2), Scalar(0, 0, 255), 3);
		}

		imshow("frame", frame);

		if (framecount == cap1.get(CAP_PROP_FRAME_COUNT)) {
			framecount = 0;
			cap1 = VideoCapture("snooker.mp4");
		}
		int key_pressed = waitKey(10);
		if (key_pressed == 27) {
			break;
		}
	}
}