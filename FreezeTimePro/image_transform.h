#pragma once

#include "common.h"

namespace FreezeTimePro {

	struct ImageTransform {
		double TranslateX;
		double TranslateY;
		double Rotation;
		double Scaling;
		double FillRate;
		int FrameWidth;
		int FrameHeight;
		bool IsAvailable;

		static cv::Point2d Transform(const cv::Point2d &p, cv::Mat &m) {
			return cv::Point2d(
				m.at<float>(0, 0) * p.x + m.at<float>(0, 1) * p.y + m.at<float>(0, 2),
				m.at<float>(1, 0) * p.x + m.at<float>(1, 1) * p.y + m.at<float>(1, 2));
		}

		void GetCornerPoints(cv::Point2d ps[4], cv::Mat &m) {
			ps[0] = Transform(cv::Point2d(0, 0), m);
			ps[1] = Transform(cv::Point2d(FrameWidth, 0), m);
			ps[2] = Transform(cv::Point2d(FrameWidth, FrameHeight), m);
			ps[3] = Transform(cv::Point2d(0, FrameHeight), m);
		}

		double Intersect(cv::Point2d &p0, cv::Point2d &p1, double w) {
			double a = p1.x - p0.x;
			double b = FrameWidth * 0.5;
			double c = p1.y - p0.y;
			double d = FrameHeight * w;
			double delta = a * d - b * c;
			if (delta == 0) {
				return -1;
			}
			double m0 = b - p0.x;
			double m1 = FrameHeight * 0.5 - p0.y;
			double t = (d * m0 - b * m1) / delta;
			if ((t < 0) || (t > 1)) {
				return -1;
			}
			return abs((-c * m0 + a * m1) / delta);
		}

		double Intersect(cv::Point2d &p0, cv::Point2d &p1) {
			double k0 = Intersect(p0, p1, 0.5);
			if (k0 < 0) {
				return Intersect(p0, p1, -0.5);
			}
			else {
				double k1 = Intersect(p0, p1, -0.5);
				if (k1 < 0) {
					return k0;
				}
				else {
					return MIN(k0, k1);
				}
			}
		}

		double GetFillRate(cv::Mat &m) {
			cv::Point2d ps[4];
			GetCornerPoints(ps, m);
			double mink = 1e20;
			for (int i = 0; i < 4; i++) {
				double k = Intersect(ps[i], ps[(i + 1) % 4]);
				if (k > 0) {
					if (k < mink) {
						mink = k;
					}
				}
			}
			if (mink == 1e20) {
				return -1;
			}
			else {
				return mink;
			}
		}

		cv::Mat GetAffineMatrix() {
			double cx = FrameWidth / 2.0;
			double cy = FrameHeight / 2.0;
			double tx = TranslateX;
			double ty = TranslateY;
			double scost = cos(-Rotation) * Scaling;
			double ssint = sin(-Rotation) * Scaling;
			cv::Mat t_mat = cv::Mat::zeros(2, 3, CV_32FC1); // row, col
			t_mat.at<float>(0, 0) = scost;
			t_mat.at<float>(0, 1) = ssint;
			t_mat.at<float>(0, 2) = scost * (tx - cx) + ssint * (ty - cy) + cx;
			t_mat.at<float>(1, 0) = -ssint;
			t_mat.at<float>(1, 1) = scost;
			t_mat.at<float>(1, 2) = ssint * (cx - tx) + scost * (ty - cy) + cy;
			return t_mat;
		}

		ImageTransform() : 
			FrameWidth(0), FrameHeight(0), IsAvailable(false),
			TranslateX(0), TranslateY(0), Rotation(0), Scaling(1), FillRate(1) { }

		ImageTransform(int width, int height) : 
			FrameWidth(width), FrameHeight(height), IsAvailable(true),
			TranslateX(0), TranslateY(0), Rotation(0), Scaling(1), FillRate(1) { }
	};

}