#include "Gussian_Evaluation.h"
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define PI 3.1415926535

void Gussian::copyData(const Gussian &gs) {
	this->point_label_ = gs.point_label_;
	this->data_ = gs.data_.clone();
	this->mean_ = gs.mean_.clone();
	this->cov_ = gs.cov_.clone();
	this->distri_ = gs.distri_.clone();
	this->inv_ = gs.inv_.clone();
	this->det_ = gs.det_;
	this->roi_ = gs.roi_;
}

Gussian::Gussian(const Gussian &gs) {
	copyData(gs);
}

Gussian Gussian::operator=(const Gussian &gs) {
	copyData(gs);
	return *this;
}

void Gussian::setMean() {
	int dim(0), numOfPoints(0);
	dim = getDimension();
	numOfPoints = getNumOfPoints();
	mean_.create(1, dim, CV_64FC1);
	for (int i = 0; i < dim; i++) {
		double sumOfSpeDim(0.0);
		for (int j = 0; j < numOfPoints; j++) {
			double cur_elem(0.0);
		  	cur_elem = data_.at<double>(j, i);
			sumOfSpeDim += cur_elem;
		}
		double meanOfSpeDim(0.0);
	   	meanOfSpeDim = sumOfSpeDim / numOfPoints;
		mean_.at<double>(0, i) = meanOfSpeDim;	
	}
}

void Gussian::setCov() {
	int dim(0), numOfPoints(0);	
	dim = getDimension();
	numOfPoints = getNumOfPoints();
	Mat data(numOfPoints, dim, CV_64FC1);
	data_.copyTo(data);
	cov_.create(dim, dim, CV_64FC1);
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < numOfPoints; j++) {	
			data.at<double>(j, i) -= mean_.at<double>(0, i);
		}
	}
	cov_ = data.t() * data;
	cov_ /= (numOfPoints - 1);
}

int Gussian::initParasByCov() {
	inv_ = cov_.inv();
	det_ = determinant(cov_);
	int diameter = 2 * roi_ + 1;
	distri_ = Mat::zeros(diameter, diameter, CV_32FC1);
}

double Gussian::operator() (double x, double y){
	double prefix = 1 / (2 * PI);
	double stan = sqrt(det_);
	prefix /= stan;
	double index = compIndex(x, y);
	double result(0.0);
	result = prefix * exp(index);
	return result;	
}

double Gussian::compIndex(double x, double y) {
	Mat input =	Mat::zeros(2, 1, CV_64FC1);
	input.at<double>(0 , 0) = x;
	input.at<double>(1 , 0) = y;
	input -= mean_.t();
	Mat input_t = input.t();
	double result(0.0);
	Mat res;
	res = input_t * inv_;
	res = res * input;
	result = res.at<double>(0,0);
	result /= -2;
	return result;
}

int Gussian::genDistri() {
	int width(0), height(0), center(0);
	width = distri_.cols;
	height = distri_.rows;
	assert(width == height);
	center = width / 2;
	for (int i = 0; i < height; i++) {
		float *d_ptr = distri_.ptr<float>(i);
		for (int j = 0; j < width; j++) {
			int dx(0), dy(0);
			dx = j - center;
			dy = i - center;
			d_ptr[j] = static_cast<float>(this->operator()(dx, dy));
		}
	}
	return 0;
}
