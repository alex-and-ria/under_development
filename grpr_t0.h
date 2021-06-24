#ifndef GRPR_T0_H
#define GRPR_T0_H

#include<iostream>
#include"sgmnt.cpp"

class grpr_t0{
	public:
		static unsigned int fl;
		static unsigned int fms;
		float crds[8];
		/*struct center{
			float x, y;
			void operator()(float x_val,float y_val){
				x=x_val; y=y_val;
			}
		} cntr;
		float ornt;
		unsigned int fs;
		grpr_t0(){};
		void set_cnfgr(unsigned int x_val, unsigned int y_val, float ornt_val, unsigned int fs_val){
			cntr(x_val,y_val);
			ornt=ornt_val; fs=fs_val;
		}*/
		void operator()(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3);
		void get_sgmnts(sgmnt ft_sgmnts[2]);
		//friend std::ostream& operator<<(std::ostream& os,const grpr_t0& grpr0);
};

extern grpr_t0 grpr0;

#endif
