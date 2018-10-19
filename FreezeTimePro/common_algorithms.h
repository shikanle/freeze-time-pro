#pragma once

#include "common.h"

namespace FreezeTimePro {

	class LowPassFilter {
	protected:
		double LastRate_;
		double FrontRate_;
		double LastValue_;
		int Count_;

	public:

		double operator()(double input) {
			if (++Count_ == 1) {
				return (this->LastValue_ = input);
			}
			else {
				return (this->LastValue_ = this->LastValue_ * this->LastRate_ + input * this->FrontRate_);
			}
		}

		LowPassFilter(double front_rate)
			: FrontRate_(front_rate), LastRate_(1.0 - front_rate), Count_(0) {
		}
	};

	class Vec2fLowPassFilter {
	protected:
		double LastRate_;
		double FrontRate_;
		cv::Vec2f LastValue_;
		int Count_;

	public:

		cv::Vec2f operator()(cv::Vec2f &input) {
			if (++Count_ == 1) {
				return (this->LastValue_ = input);
			}
			else {
				cv::Vec2f result(0, 0);
				cv::scaleAdd(this->LastValue_, this->LastRate_, result, result);
				cv::scaleAdd(input, this->FrontRate_, result, this->LastValue_);
				return this->LastValue_;
			}
		}

		Vec2fLowPassFilter(double front_rate)
			: FrontRate_(front_rate), LastRate_(1.0 - front_rate), Count_(0) {
		}
	};

	class Vec3dLowPassFilter {
	protected:
		double LastRate_;
		double FrontRate_;
		cv::Vec3d LastValue_;
		int Count_;

	public:

		cv::Vec3d operator()(cv::Vec3d &input) {
			if (++Count_ == 1) {
				return (this->LastValue_ = input);
			}
			else {
				cv::Vec3d result(0, 0, 0);
				cv::scaleAdd(this->LastValue_, this->LastRate_, result, result);
				cv::scaleAdd(input, this->FrontRate_, result, this->LastValue_);
				return this->LastValue_;
			}
		}

		Vec3dLowPassFilter(double front_rate)
			: FrontRate_(front_rate), LastRate_(1.0 - front_rate), Count_(0) {
		}
	};

	class LinearInterpolator {
	protected:
		double X_ = 0;
		double SumXY_ = 0;
		double SumX_ = 0;
		double SumY_ = 0;
		double SumSquaredX_ = 0;

	public:
		inline void Reset() {
			X_ = 0;
			SumXY_ = 0;
			SumX_ = 0;
			SumY_ = 0;
			SumSquaredX_ = 0;
		}

		inline LinearInterpolator() {
			Reset();
		}

		inline double GetSlope() {
			return (SumXY_ - SumX_ * SumY_ / X_) / (SumSquaredX_ - SumX_ * SumX_ / X_);
		}

		inline double GetIntercept(double &slope) {
			slope = (SumXY_ - SumX_ * SumY_ / X_) / (SumSquaredX_ - SumX_ * SumX_ / X_);
			return (SumY_ - slope * SumX_) / X_;
		}

		inline LinearInterpolator &operator<<(double y) {
			X_ = X_ + 1;
			SumXY_ += X_ * y;
			SumX_ += X_;
			SumY_ += y;
			SumSquaredX_ += X_ * X_;
			return *this;
		}
	};

	class LinearInterpolatorWithOffset : public LinearInterpolator {
	protected:
		double Offset_;

	public:
		inline LinearInterpolatorWithOffset(double offset)
			: LinearInterpolator(), Offset_(offset) {
		}

		inline double GetIntercept(double &slope) {
			slope = (SumXY_ - SumX_ * SumY_ / X_) / (SumSquaredX_ - SumX_ * SumX_ / X_);
			return (SumY_ - slope * SumX_) / X_ + Offset_;
		}

		inline LinearInterpolatorWithOffset &operator<<(double y) {
			LinearInterpolator::operator<<(y - Offset_);
			return *this;
		}
	};

}