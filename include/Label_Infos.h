#ifndef Label_Infos_H
#define Label_Infos_H
#include <iostream>
#include <vector>
#include <string>
#include "Label_Info.h"

class LabelInfos {
friend std::ostream& operator<<(std::ostream &os, const LabelInfos &lis);
friend std::istream& operator>>(std::istream &is, LabelInfos &lis);
public:
	LabelInfos() {}
	explicit LabelInfos(std::string lfn):label_file_(lfn) {}
	LabelInfos(const LabelInfos &lis) {this->copyData(lis);}
	LabelInfos& operator=(const LabelInfos &lis) {
		this->copyData(lis);
		return *this;
	}
	void LabelInfosLoad();
	unsigned int labelCount() { return labels_.size();}
	unsigned int getPNum() {return p_num_;}
   	LabelInfo getOneLabel(unsigned int index) {return labels_[index];}	
	~LabelInfos() {labels_.clear();}	
private:
	void copyData(const LabelInfos &lis);
	int	p_num_;
	std::vector<LabelInfo> labels_;
	std::string label_file_;	
};
#endif
