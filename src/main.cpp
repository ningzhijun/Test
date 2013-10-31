#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Gussian_Evaluation.h"
#include "Label_Info.h"
#include "Label_Infos.h"
#include "Procrustes_Analysis.h"
using namespace cv;
using namespace std;
int sleFileName(int, string &);
int main(int argc, char** argv) {
	if(argc != 3) {
		cout<<"Usage:./cmd label out_dir"<<endl;
		exit(-1);
	}
	string label_file_name(argv[1]);
	string out_put_dir(argv[2]);
	LabelInfos all_labels(label_file_name);
	all_labels.LabelInfosLoad();
	int psize(0);
	int lsize = all_labels.labelCount(); 
	vector<Mat> all_dst;
	for (int i = 0; i < lsize; i++) {
		LabelInfo lsrc = all_labels.getOneLabel(i);
		Mat src = lsrc.getKeyPoints().clone();
		printf("Processing the %dth label\n", i);
		for (int j = 0; j < lsize; j++) {
			if ( i == j)
				continue;
			LabelInfo ldst = all_labels.getOneLabel(j);
			GPA gpa;
	   		Mat	dst = ldst.getKeyPoints().clone(), rt;
			psize = src.rows;
			rt = Mat::zeros(psize, 2, CV_64FC1);
			gpa.simTransform(src, dst, rt);
			assert(psize == rt.rows);
			for (int k = 0; k < psize; k++) {
				rt.at<double>(k, 0) -= src.at<double>(k, 0);
				rt.at<double>(k, 1) -= src.at<double>(k, 1);
			}
			all_dst.push_back(rt);
		}
	}
	int dsize = all_dst.size();
	//cout << dsize << endl;
	ofstream writer;
	for (int k = 0; k < psize; k++) {
		Mat mean(1, 2, CV_64FC1);
		//mean.at<double>(0, 0) = src[k].x;
		//mean.at<double>(0, 1) = src[k].y;
		Mat ds_data(dsize, 2, CV_64FC1);
		for (int j = 0; j < dsize; j++) {
			ds_data.at<double>(j ,0) = all_dst[j].at<double>(k, 0);
			ds_data.at<double>(j ,1) = all_dst[j].at<double>(k, 1);
		}
		Gussian *cur_gs = new Gussian(k, ds_data, mean, 120);
		cur_gs->setMean();
		cur_gs->setCov();
		Mat mean_a = cur_gs->getMean();
		Mat cov_a = cur_gs->getCov();
		stringstream ss;
 		ss << k;
		string p_label = ss.str();
		string fn, prefix = "part_";
		string suf_gs, suf_dis, fn_gs, fn_dis;
		suf_gs = ".gs";
		suf_dis = ".dis";
		prefix = out_put_dir + prefix;
		fn = prefix + p_label;
		fn_gs = fn + suf_gs;
		fn_dis = fn + suf_dis;
		writer.open(fn_gs.c_str());
		if (!writer.is_open()) {
			cout << "something is wrong when writing "
				<< fn << endl;
			return -2;
		}
		writer << mean_a.at<double>(0, 0) << " "
		   << mean_a.at<double>(0, 1) << endl;
		writer << cov_a.at<double>(0, 0) << " "
			<< cov_a.at<double>(0, 1) << " "
			<< cov_a.at<double>(1, 0) << " "
			<< cov_a.at<double>(1, 1) << endl;
		writer << fn_dis << endl;
		writer.close();
		cur_gs->initParasByCov();
		cur_gs->genDistri();
		Mat distr = cur_gs->getDis();
		CvMat dis_old_fo = distr;
		cvSave(fn_dis.c_str(), &dis_old_fo);	
		delete  cur_gs;
		}
	return 0;
}

int sleFileName(int index, string & file_name) {
	switch(index) {
		case 0:
			file_name = "leyelc.gs";
			break;
		case 1:
			file_name = "leyerc.gs";
			break;
		case 2:
			file_name = "reyelc.gs";
			break;
		case 3:
			file_name = "reyerc.gs";
			break;
		case 4:
			file_name = "nose.gs";
			break;
		case 5:
			file_name = "mthlc.gs";
			break;
		case 6:
			file_name = "mthrc.gs";
			break;
		default:
			cout << "wrong index input"
				<< endl;
			break;
	}
}
