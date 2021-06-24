#ifndef SQUARE_CPP
#define SQUARE_CPP

#include"square.h"

square::square(_2d_pnt cntr_val, float theta_val){
	nvs=4; vrtcs=new float[2*nvs];
	cntr(cntr_val.x,cntr_val.y);
	//float half_diag_len=side_len*sqrt(2.)/2);
	vrtcs[0]=cntr.x+half_diag_len*sin(theta_val+acos(-1)/4.); vrtcs[1]=cntr.y-half_diag_len*cos(theta_val+acos(-1)/4.);
	vrtcs[2]=cntr.x+half_diag_len*sin(theta_val+acos(-1)*3/4.); vrtcs[3]=cntr.y-half_diag_len*cos(theta_val+acos(-1)*3/4.);
	vrtcs[4]=cntr.x+half_diag_len*sin(theta_val+acos(-1)*5/4); vrtcs[5]=cntr.y-half_diag_len*cos(theta_val+acos(-1)*5/4.);
	vrtcs[6]=cntr.x+half_diag_len*sin(theta_val+(acos(-1)*7)/4.); vrtcs[7]=cntr.y-half_diag_len*cos(theta_val+(acos(-1)*7)/4.);
	set_dsts();
	min_c_dst=half_diag_len*cos(acos(-1)/4);
}

float* square::get_param(uint param_num){//should return some internal parameters of inherited class which is not part of base class;
	float* param=NULL;
	switch(param_num){
		case 0:
			param=&mx_nr_dst;
			break;
		case 1:
			param=&mx_fr_dst;
			break;
	}
	return param;
}

square::~square(){
	delete[] vrtcs;

}

float const square::side_len=25;
float square::half_diag_len=side_len*sqrt(2.)/2;
float square::mx_nr_dst=sqrt(grpr_t0::fms*grpr_t0::fms+grpr_t0::fl*grpr_t0::fl);
float square::mx_fr_dst=sqrt((grpr_t0::fms+square::half_diag_len)*(grpr_t0::fms+square::half_diag_len)+grpr_t0::fl*grpr_t0::fl);

#endif
