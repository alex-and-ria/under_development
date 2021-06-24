#ifndef _2D_PNT_CPP
#define _2D_PNT_CPP

struct _2d_pnt{
	float x, y;
	void operator()(float x_val=0,float y_val=0){
		x=x_val; y=y_val;
	}
};
#endif
