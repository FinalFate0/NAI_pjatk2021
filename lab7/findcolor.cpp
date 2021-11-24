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

	int size_x = 320;
	int size_y = 200;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-x") {
			if (i + 1 < argc) {
				i++;
				char* d;
				size_x = std::strtol(argv[i], &d, 10);
				if (*d) {
					std::cerr << "-x must be followed by a positive number";
					return 1;
				}
			}
			else {
				std::cerr << "-x option requires one argument" << std::endl;
				return 1;
			}
		}
		else if (arg == "-y") {
			if (i + 1 < argc) {
				i++;
				char* d;
				size_y = std::strtol(argv[i], &d, 10);
				if (*d) {
					std::cerr << "-y must be followed by a positive number";
					return 1;
				}
			}
			else {
				std::cerr << "-y option requires one argument" << std::endl;
				return 1;
			}
		}
	}

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

	int framecount = 0;
	while (true) {
		Mat frame;
		cap1.read(frame);
		framecount++;

		GaussianBlur(frame, frame, Size(5, 5), 0);
		resize(frame, frame, Size(size_x, size_y));

		Mat hsv;
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		Mat inrange;
		inRange(hsv, Scalar(h1, s1, v1), Scalar(h2, s2, v2), inrange);

		Mat output;
		inrange.copyTo(output);
		cvtColor(output, output, COLOR_GRAY2BGR);

		putText(output,
			"h1:" + std::to_string(h1) +
			" h2:" + std::to_string(h2) +
			" s1:" + std::to_string(s1) +
			" s2:" + std::to_string(s2) +
			" v1:" + std::to_string(v1) +
			" v2:" + std::to_string(v2),
			Point(10, 20),
			FONT_HERSHEY_DUPLEX,
			0.4,
			Scalar(60, 70, 255),
			1,
			LINE_AA);

		imshow("obraz", output);


		if (framecount == cap1.get(CAP_PROP_FRAME_COUNT)) {
			framecount = 0;
			cap1 = VideoCapture("nature.mp4");
		}

		int key_pressed = waitKey(1);
		if (key_pressed == 27) {
			break;
		}
		else if (key_pressed == 120) {
			//Mat select;
			//inrange.copyTo(select);
			//imshow("zdjecie", select);
			Rect roi_rect = selectROI("obraz", inrange);
			Mat roi = Mat(inrange, roi_rect);
			imwrite("output.jpg", roi);
		}
	}
}