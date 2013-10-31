#include <fstream>
#include <iostream>
#include <sstream>
#include "Label_Infos.h"

using namespace std;

void LabelInfos::LabelInfosLoad() {
	ifstream fp(label_file_.c_str());
	if (!fp) {
		cerr << "The label file maybe damaged \
			or not complete!" << endl;
	  exit(-2); //is exit() useful while it's not in main function?
	}
	int tempUI, NbOfSamples;
	string line;
	fp >> tempUI;
	NbOfSamples = tempUI;
	fp >> p_num_;
 	getline(fp, line);	
	LabelInfo temp_li(p_num_);
	labels_.resize(NbOfSamples);
	int cnt(0);
	while (!fp.eof() && tempUI != 0) {
		tempUI--;
		int index = NbOfSamples - tempUI - 1;
	   	getline(fp, line);
		istringstream iss(line);
		iss >> temp_li;
		if(fp.fail()) {
			break;
		}
		labels_[index] = temp_li;
		cnt ++;
	}
	if (!fp.eof()) {
		int actual_size = NbOfSamples - tempUI;
		labels_.resize(actual_size);
	}	else {
		labels_.resize(cnt);
	}
}

void LabelInfos::copyData(const LabelInfos &lis) {
	this->label_file_ = lis.label_file_;
	this->p_num_ = lis.p_num_;
	this->labels_.assign(lis.labels_.begin(), lis.labels_.end());	
}


