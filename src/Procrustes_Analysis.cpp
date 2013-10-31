#include "Procrustes_Analysis.h"
#include <cassert>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
void translate(Mat &, double, double);
void alignTransformation(const Mat &,const Mat &,
		double &, double &, double &, double &);
void transform(const Mat &, Mat &, double,
		double, double, double);
Point2d getCenterOfShape(const Mat &);

void GPA::simTransform(const Mat &src_,
					   const Mat &dst_,
					   Mat &rt_) {
	//cout << src_.rows << " " << dst_.rows << endl;
	assert(src_.rows == dst_.rows);
	Mat src = src_.clone();
	int size = src.rows;
	double a(0.0), b(0.0), tx(0.0), ty(0.0);
	Point2d center;
	center = getCenterOfShape(src);
	translate(src,
			-center.x,
			-center.y);
	alignTransformation(src, dst_, 
						 a, b, tx, ty);
	transform(src, rt_, a, -b, b, a);
	translate(rt_, tx, ty);
}

void GPA::calcDistance(const Mat &src_,
					   const Mat &dst_,
					   Mat &rt_) {
	assert(src_.size() == dst_.size());
	int size(0);
	double tmpx(0.f), tmpy(0.f);
	size  = src_.rows;
	for(int i = 0; i < size; i++) { 
		tmpx = src_.at<double>(i, 0) 
			- dst_.at<double>(i, 0);
		tmpy =  src_.at<double>(i, 1) 
			- dst_.at<double>(i, 1);
		rt_.at<double>(i, 0) = tmpx;
		rt_.at<double>(i, 1) = tmpy;
	}
}
void translate(Mat &shape, double x, double y) {
	int shapesize = shape.rows;
	double xx(0.0), yy(0.0);

	for (int i = 0; i < shapesize; i++) {
		xx = shape.at<double>(i, 0) + x;
		yy = shape.at<double>(i, 1) + y;
		shape.at<double>(i, 0) = xx;
		shape.at<double>(i, 1) = yy;
	}
}

void transform(const Mat &s_shape, Mat &r_shape, 
			   double c00, double c01, 
			   double c10, double c11) {
	double x(0.0), y(0.0), xx(0.0), yy(0.0);
	int shapesize = s_shape.rows * s_shape.cols /2 ;
	for (int i = 0; i < shapesize; i++) {
		x = s_shape.at<double> (0,2 * i);
		y = s_shape.at<double> (0,2 * i + 1);
		xx = c00 * x + c01 * y;
		yy = c10 * x + c11 * y;
		r_shape.at<double> (0, 2 * i) = xx;
		r_shape.at<double> (0, 2 * i + 1) = yy;
	}
}

void alignTransformation(const Mat &s_shape, const Mat &d_shape,
		double &a_, double &b_, double &tx_, double &ty_) {
	double X1(0.0), Y1(0.0), X2(0.0), Y2(0.0), Z(0.0), C1(0.0), C2(0.0);
	int shapeSize = (s_shape.rows*s_shape.cols)>>1;
	double W = (double)shapeSize;
	double x1, y1, x2, y2;
	double a, b, tx, ty;

	for(int i = 0, size = shapeSize; i < size; i++)
	{
		x1 = s_shape.at<double>(0, i*2);
		y1 = s_shape.at<double>(0, i*2+1);
		x2 = d_shape.at<double>(0, i*2);
		y2 = d_shape.at<double>(0, i*2+1);
		Z  += x2 * x2 + y2 * y2;
		X1 += x1;
		Y1 += y1;
		X2 += x2;
		Y2 += y2;
		C1 += x1 * x2 + y1 * y2;
		C2 += y1 * x2 - x1 * y2;
	}

	{
		double SolnA[] = {X2, -Y2, W, 0, Y2, X2, 0, W, Z, 0, X2, Y2, 0, Z, -Y2, X2};
		Mat A(4, 4, CV_64FC1, SolnA);
		double SolnB[] = {X1, Y1, C1, C2};
		Mat B(4, 1, CV_64FC1, SolnB);
		Mat Soln(4, 1, CV_64FC1);
		solve(A, B, Soln, DECOMP_SVD);
		a	= Soln.at<double>(0, 0);  b	= Soln.at<double>(1, 0);
		tx	= Soln.at<double>(2, 0);	 ty	= Soln.at<double>(3, 0);
	}

	double norm = a * a + b * b; 
	a_ = a / norm, b_ = -b / norm;
	tx_ = (-a*tx - b*ty) / norm, ty_ = (b*tx - a*ty) / norm;
}

Point2d getCenterOfShape(const Mat& src_) {
	double cx(0.0), cy(0.0);
	Point2d center;
	int size = src_.rows;
	for (int i = 0; i < size; i++) {
		cx += src_.at<double>(i, 0);
		cy += src_.at<double>(i, 1);
	}
	cx /= size;
	cy /= size;
	center.x = cx;
	center.y = cy;
	return center;	
}
