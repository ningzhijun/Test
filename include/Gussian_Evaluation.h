#ifndef GUSSIAN_EVALUATION_H
#define GUSSIAN_EVALUATION_H
#include <string>
#include <opencv2/opencv.hpp>

class Gussian {
	void copyData(const Gussian &);
	int point_label_;
	cv::Mat data_;
	cv::Mat mean_;
	cv::Mat cov_;
	cv::Mat inv_;
	cv::Mat distri_;
	int roi_;
	double det_;
public:
	Gussian() {}
	Gussian(int pl, cv::Mat in_data, 
			cv::Mat mean, int roi = 100)
		:data_(in_data), roi_(roi),
   		point_label_(pl), mean_(mean),
		det_(0.0){
				cov_.release();
				inv_.release();
				distri_.release();
			}
	Gussian(const Gussian &);
	Gussian operator=(const Gussian &);
	int getDimension() {return data_.cols;}
	int getNumOfPoints() {return data_.rows;}
	void setMean(const cv::Mat &m) {mean_ = m;}
	void setMean();
	void setCov();
	int initParasByCov();
	double operator() (double x, double y);
	double compIndex(double x, double y);
	int genDistri();
	cv::Mat getMean() {return mean_;}
	cv::Mat getCov() {return cov_;} 
	cv::Mat getDis() {return distri_;}
};

#endif
