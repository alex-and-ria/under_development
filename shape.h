#ifndef SHAPE_H
#define SHAPE_H

#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include"2d_pnt.cpp"
#include"sgmnt.cpp"
#include"grpr_t0.h"

#define Max_x 1024
#define Max_y 512

typedef unsigned short int usi;
typedef unsigned int uint;

class shape{
	public:
		usi nvs;//number of vertices in shape;
		float* vrtcs;
		_2d_pnt cntr;
		float ornt_z;
		float max_c_dst;
		float min_c_dst;
		uint top_vec_idx;
		static cv::Mat data_img;
		std::vector<shape*> lnks;
		std::vector<shape*> flnks;
		struct tgt_elem{
			public:
				shape* shape_elem;
				bool vtx_bx[4];//vertex is in gripper box;
				float vtx_lf_dst[4];//distance of vertex to the left fingertip;
				float vtx_rf_dst[4];//distance of vertex to the right fingertip;

		};
		shape() { }
		void set_dsts();
		
		//shape(_2d_pnt cntr_val, float theta_val);
		void static drw(cv::Mat& data_img,std::vector<shape*>& shps,unsigned int tst_idx=0);
		bool static chck_clsn(shape* shp0, shape* shp1);
		virtual float* get_param(uint param_num)=0;
		void static crs_chck(std::vector<shape*>& shps);
		double static fn_srch(unsigned int N_tgt,std::vector<shape*>& shps,unsigned int tst_idx=0);
		bool static ft_cnfg_chck(grpr_t0& grpr0,std::vector<shape*>& wd_sbst);
		unsigned int static set_tgts(grpr_t0& grpr0,std::vector<shape*>& sbst,std::vector<shape*>& wd_sbst);//TODO change to pridiction and colision detection method;
		/*
		
		
		
		
		friend std::ostream& operator<<(std::ostream& os,const shape* shape_val);*/
	
	
	



};








#endif
