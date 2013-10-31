#ifndef Label_Info_H
#define Label_Info_H
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
class LabelInfo {
friend std::ostream& operator<<(std::ostream &os, 
						const LabelInfo &li);
friend std::istream& operator>>(std::istream &is,
						LabelInfo &li);
public:
	LabelInfo():ocular_(0.0),is_pos_(1) {}
	LabelInfo(int p_num):ocular_(0.0),is_pos_(1),p_num_(p_num) {}
	LabelInfo(const LabelInfo &li) {this->copyData(li);}
	LabelInfo& operator=(const LabelInfo &li) {
		this->copyData(li);
		return *this;
	}
	//void vectorClean() {keypoints_.clear();}
	std::vector<LabelInfo> genNegLabel(int); 
  	double calcOcular(); 
	void setOcular();
	void setOcular(double);
	double getOcular() {return ocular_;}
	void setSampna(const std::string& na) {sampname_ = na;}
	std::string getSampna() {return sampname_;}
	void setKeyPos(const cv::Mat &pos)	{
		pos.copyTo(keypoints_); //Maybe is a bug.
	}
	void setPNum(int pnum) {p_num_ = pnum;}
	int getPNum() {return p_num_;}
	void setIsPos(bool sg) {is_pos_ = sg;}
	bool getIsPos() {return is_pos_;}
	int getKeyPointsCount() {return keypoints_.rows;}
	cv::Mat getKeyPoints() {return keypoints_;}
	std::string getSamplePath() {return sampname_;}
	~LabelInfo() {}
	/*enum {
		LEYELCOR = 0,
		LEYERCOR,
		REYELCOR,
		REYERCOR,	
		NOSETIP,
		LMOUTHCOR,
		RMOUTHCOR,
	};*/
private:
	void copyData(const LabelInfo &li);
	bool is_pos_;
	std::string sampname_;
	int p_num_;
	double ocular_;
	cv::Mat keypoints_;
};
#endif
