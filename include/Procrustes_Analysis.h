#ifndef PROCRUSTES_ANALYSIS_H
#define PROCRUSTES_ANALYSIS_H
#include <vector>
#include <opencv2/opencv.hpp>
class GPA {
public:
	void simTransform(const cv::Mat &,
					  const cv::Mat &,
					  cv::Mat &);
	void calcDistance(const cv::Mat &,
					  const cv::Mat &,
					  cv::Mat &);
};


#endif
