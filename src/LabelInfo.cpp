#include "Label_Info.h"
#include <cstdlib>
#include <ctime>
#include <sys/time.h>
using namespace std;
using namespace cv;
istream& operator>>(istream &is, LabelInfo &li) {
	is >> li.sampname_;
	double temx(0.0), temy(0.0);
	int row_seq(0);
	int p_num = li.p_num_;
	li.keypoints_ = Mat::zeros(p_num, 2, CV_64FC1);
	while(row_seq < p_num && is >> temx && is >> temy) {
		li.keypoints_.at<double>(row_seq, 0) = temx;
		li.keypoints_.at<double>(row_seq, 1) = temy;
		row_seq++;
	}
	li.setOcular();
	return is;
}
ostream& operator<<(ostream &os, const LabelInfo &li) {
	os << li.sampname_ << "\t" << "positive:" 
		<< li.is_pos_ << "\t" << "pnum:" 
		<< li.p_num_ << "\t" << "ocular:"
		<< li.ocular_ << endl;
	os << li.keypoints_;
	return os;
}
void LabelInfo::copyData(const LabelInfo &li) {
	this->sampname_ = li.sampname_;
	this->p_num_ = li.p_num_;
	this->keypoints_ = li.keypoints_.clone();
	this->is_pos_ = li.is_pos_;
	this->ocular_ = li.ocular_;
}
double LabelInfo::calcOcular() {
	return (keypoints_.at<double>(9, 0) 
			+ keypoints_.at<double>(8, 0)
			- keypoints_.at<double>(7, 0)
			- keypoints_.at<double>(6, 0))/2;
}
void LabelInfo::setOcular() {
	ocular_ = calcOcular();
}
void LabelInfo::setOcular(double t) {
	ocular_ = t;
}
vector<LabelInfo> LabelInfo::genNegLabel(int negCount) {
	if (ocular_ == 0) {
		setOcular();
	}
	vector<LabelInfo> negs;
	Mat im;
	im = imread(sampname_); // shall we consider the im equal null?
	int pic_w = im.cols;
	int pic_h = im.rows;
	int pre_w = pic_w - static_cast<int>(ocular_ / 4);
	int pre_h = pic_h - static_cast<int>(ocular_ / 4);
	for (int i = 0; i < negCount; i++) {
		LabelInfo cur_neg;
		Mat cur_keys = Mat::zeros(p_num_, 2, CV_64FC1);
		for (int j = 0; j < p_num_; j++) {
				int cur_x = static_cast<int>(keypoints_.at<double>(j, 0));
				int cur_y = static_cast<int>(keypoints_.at<double>(j, 1));
				double cur_neg_x, cur_neg_y;
				struct timeval tv;
				gettimeofday(&tv, NULL);
				srand(tv.tv_usec);
				int off_x = rand() % pre_w;
				int off_y = rand() % pre_h;
				cur_neg_x = static_cast<double>((off_x + cur_x) % pic_w);
				cur_neg_y = static_cast<double>((off_y + cur_y) % pic_h);
				cur_keys.at<double>(j, 0) = cur_neg_x;
				cur_keys.at<double>(j, 1) = cur_neg_y;
		}
		cur_neg.setSampna(sampname_);
		cur_neg.setKeyPos(cur_keys);
		cur_neg.setOcular(ocular_);
		cur_neg.setPNum(p_num_);
		cur_neg.setIsPos(0);
		negs.push_back(cur_neg);
	}
	return negs;	
}
