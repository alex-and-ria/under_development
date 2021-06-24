#ifndef GTPR_T0_CPP
#define GRPR_T0_CPP

#include"grpr_t0.h"

void grpr_t0::operator()(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3){
	crds[0]=x0; crds[1]=y0; crds[2]=x1; crds[3]=y1; crds[4]=x2; crds[5]=y2; crds[6]=x3; crds[7]=y3;
}

void grpr_t0::get_sgmnts(sgmnt ft_sgmnts[2]){
	ft_sgmnts[0].set_data(crds[0],crds[1],crds[4],crds[5]);
	ft_sgmnts[1].set_data(crds[2],crds[3],crds[6],crds[7]);

}

/*std::ostream& operator<<(std::ostream& os,const grpr_t0& grpr0){
	os<<"\ncrds=[";
	for(unsigned short int i=0;i<8;i++){
		os<<grpr0.crds[i]<<' ';
	}
	os<<']';

}*/

unsigned int grpr_t0::fl=75;
unsigned int grpr_t0::fms=85;
grpr_t0 grpr0;

/*
0,1		2,3
 |		|
 |		|
 |		|
4,5		6,7

*/
#endif
