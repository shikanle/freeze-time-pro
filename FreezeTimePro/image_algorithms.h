#pragma once

#include "common.h"
#include "image_transform.h"
#include "common_algorithms.h"

namespace FreezeTimePro {

	class ImageAlgorithms {
	public:
		static ImageTransform Calibrate(cv::Mat frame, bool silently) {

			int frame_width = frame.cols;
			int frame_height = frame.rows;
			int calibrate_distance = (frame_height * 95 / 100);
			ImageTransform retval(frame_width, frame_height);

			cv::Mat prevImg;
			cv::Mat hsvImg, imgThresholded, imgBorder;
			cv::Vec2f point0(2);
			cv::Vec2f point1(2);
			cv::Point2f pointCenter;
			std::vector<cv::Point2f> p2ds(2);
			cv::cvtColor(frame, hsvImg, cv::COLOR_BGR2HSV);
			cv::inRange(hsvImg, cv::Scalar(85, 80, 50), cv::Scalar(155, 255, 255), imgThresholded);
			cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(frame_width / 200.0, frame_width / 200.0));
			cv::morphologyEx(imgThresholded, imgThresholded, cv::MORPH_OPEN, element);
			cv::morphologyEx(imgThresholded, imgThresholded, cv::MORPH_CLOSE, element);
			cv::threshold(imgThresholded, imgThresholded, 1, 255, cv::THRESH_BINARY);
			cv::copyMakeBorder(imgThresholded, imgBorder, 1, 1, 1, 1, cv::BORDER_CONSTANT, 0);
			std::vector<std::vector<cv::Point> > contours0;
			std::vector<cv::Vec4i> hierarchy;
			cv::findContours(imgBorder, contours0, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
			int point_num = 0;
			for (int idx = 0; idx < (int)contours0.size(); idx++) {
				cv::Rect bound = cv::boundingRect(cv::Mat(contours0[idx]));
				cv::Point2f bc = cv::Point2f(bound.x + bound.width / 2, bound.y + bound.height / 2);
				uchar x = imgThresholded.at<uchar>(bc);
				if (x > 0) {
					if (point_num == 0) {
						point0[0] = bc.x;
						point0[1] = bc.y;
						point_num++;
					} else if (point_num == 1) {
						point1[0] = bc.x;
						point1[1] = bc.y;
						point_num++;
					} else {
						point_num++;
					}
				}
			}
			if (point_num == 2) {
				//set p2ds
				p2ds[0].x = point0[0];
				p2ds[0].y = point0[1];
				p2ds[1].x = point1[0];
				p2ds[1].y = point1[1];
				cv::circle(frame, p2ds[0], 5, cv::Scalar(255, 0, 0), 2);
				cv::circle(frame, p2ds[1], 5, cv::Scalar(255, 0, 0), 2);
				pointCenter.x = p2ds[0].x * 0.5 + p2ds[1].x * 0.5;
				pointCenter.y = p2ds[0].y * 0.5 + p2ds[1].y * 0.5;
				//set retval
				double dx = retval.TranslateX = frame_width / 2 - pointCenter.x;
				double dy = retval.TranslateY = frame_height / 2 - pointCenter.y;
				if (dx * dx + dy * dy <= 1.0 * 1.0) {
					cv::circle(frame, pointCenter, 4, cv::Scalar(0, 255, 0), 3);
				} else {
					cv::circle(frame, pointCenter, 3, cv::Scalar(0, 0, 255), 1);
				}

				double rx = p2ds[0].x - p2ds[1].x;
				double ry = p2ds[0].y - p2ds[1].y;
				retval.Rotation = atan2(p2ds[0].x - p2ds[1].x, p2ds[0].y - p2ds[1].y);
				if (abs(retval.Rotation) <= 0.01) {
					cv::line(frame, p2ds[0], p2ds[1], cv::Scalar(0, 255, 0), 3);
				} else {
					cv::line(frame, p2ds[0], p2ds[1], cv::Scalar(0, 128, 255), 1);
				}
				retval.Scaling = (double)calibrate_distance / sqrt(rx * rx + ry * ry);

				// translate image
				auto transform_m = retval.GetAffineMatrix();
				retval.FillRate = retval.GetFillRate(transform_m);
				cv::warpAffine(frame, prevImg, transform_m, frame.size());
				
				retval.IsAvailable = true;
			} else {
				retval.IsAvailable = false;
			}

			if (!silently) {

				// helper lines
				float halfw = frame_width / 2.0f;
				float halfh = frame_height / 2.0f;
				cv::line(frame, cv::Point2f(halfw - 20, halfh),
					cv::Point2f(halfw + 20, halfh), cv::Scalar(200, 200, 200), 1);
				cv::line(frame, cv::Point2f(halfw, halfh - 110),
					cv::Point2f(halfw, halfh + 20), cv::Scalar(200, 200, 200), 1);
				cv::circle(frame, cv::Point2f(halfw, halfh), 5, cv::Scalar(200, 200, 200), 1);

				float rw = halfw * (float)retval.FillRate;
				float rh = halfh * (float)retval.FillRate;
				cv::Point2f cps[4];
				cps[0] = cv::Point2f(halfw - rw, halfh - rh);
				cps[1] = cv::Point2f(halfw + rw, halfh - rh);
				cps[2] = cv::Point2f(halfw + rw, halfh + rh);
				cps[3] = cv::Point2f(halfw - rw, halfh + rh);
				for (int k = 0; k < 4; k++) {
					cv::line(prevImg, cps[k], cps[(k + 1) % 4], cv::Scalar(200, 200, 200), 1);
				}

				cv::resize(frame, frame, cv::Size(1024, 768));
				if (!prevImg.empty()) {
					cv::resize(prevImg, prevImg, cv::Size(1024, 768));
				}
				if (!imgThresholded.empty()) {
					cv::resize(imgThresholded, imgThresholded, cv::Size(1024, 768));
				}

				auto output = Format("T: %.2lf, %.2lf | R: %.2lf | Z: %.2lf | Fill: %.2lf%% | [P] Preview | [T] Threshold",
					retval.TranslateX, retval.TranslateY, retval.Rotation * 57.2957795, retval.Scaling, retval.FillRate * 100);
				cv::putText(frame, output, cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255));

				int mode = 0;
				while (true) {
					switch (mode) {
					case 'p':
					case 'P':
						if (!prevImg.empty()) {
							cv::imshow("Calibration - FreezeTime Pro", prevImg);
						}
						break;
					case 't':
					case 'T':
						cv::imshow("Calibration - FreezeTime Pro", imgThresholded);
						break;
					default:
						cv::imshow("Calibration - FreezeTime Pro", frame);
						break;
					}
					int new_mode = cv::waitKey();
					if ((new_mode == 10) || (new_mode == 13)) {
						break;
					}
					else if (new_mode == 27) {
						retval.IsAvailable = false;
						break;
					}
					else if (new_mode == mode) {
						mode = 0;
					}
					else {
						mode = new_mode;
					}
				}
				cv::destroyWindow("Calibration - FreezeTime Pro");
			}
			
			return retval;
		}
	};

}