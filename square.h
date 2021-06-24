#ifndef SQUARE_H
#define SQUARE_H

#include"shape.h"
#include"grpr_t0.h"

class square: public shape{
	public:
		float static mx_nr_dst;
		float static mx_fr_dst;		
		float static const side_len;
		float static half_diag_len;
		square() { }
		square(_2d_pnt cntr_val, float theta_val);
		float* get_param(uint param_num);//should return some internal parameters of inherited class which is not part of base class;
		~square();

};

#endif
