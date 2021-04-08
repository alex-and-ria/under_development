#ifndef SGMNT
#define SGMNT

#define mn(a,b) (((a)<(b))?(a):(b))
#define mx(a,b) (((a)>(b))?(a):(b))
#define abs(x) (((x)>0)?(x):(-(x)))

enum cross_spec {n_cs=0,cs=1,eq=2};

class sgmnt{
		public:
			float x0;
			float x1;
			float y0;
			float y1;
			float k;
			float b;
			bool is_eq;
			sgmnt(){ }
			void set_data(float x0_data,float y0_data,float x1_data,float y1_data){
				float eps=0.001;
				if(x0_data<x1_data){
					x0=x0_data; x1=x1_data; y0=y0_data; y1=y1_data;
				}
				else{
					x1=x0_data; x0=x1_data; y1=y0_data; y0=y1_data;
				}
				if(abs(x0-x1)<eps){
					is_eq=1;
				}
				else{
					k=((y1-y0)/(x1-x0)+0.);
					b=y1-k*x1;
					is_eq=0;
				}
				//if(0) std::cout<<"\nx0="<<x0<<" y0="<<y0<<" x1="<<x1<<" y1="<<y1<<" is_eq="<<is_eq<<" k="<<k<<" b="<<b;
			}
			cross_spec static seg_crs_chck(sgmnt& sgmnt0,sgmnt& sgmnt1){
				float eps=0.001;
				if(sgmnt0.is_eq==1 && sgmnt1.is_eq==1){//both lines are vertical;
					if(abs(sgmnt0.x0-sgmnt1.x0)>eps){//parallel;
						return n_cs;
					}
					else{//same line;
						if(sgmnt0.x1<sgmnt1.x0 || sgmnt1.x1<sgmnt0.x0){//separate segments;
							return n_cs;
						}
						else{//segments have close parts;
							return cs;
						}
					}	
				}
				else if(sgmnt0.is_eq==1){//sgmnt0 is vertical;
					float x_crs,y_crs;
					x_crs=sgmnt0.x0;
					y_crs=sgmnt1.k*x_crs+sgmnt1.b;
					if(mn(sgmnt0.y0,sgmnt0.y1)<y_crs && y_crs<mx(sgmnt0.y0,sgmnt0.y1) && sgmnt1.x0<x_crs && x_crs<sgmnt1.x1){//both segments include crossing point;
						return cs;
					}
					else{
						return n_cs;
					}
				}
				else if(sgmnt1.is_eq==1){//sgmnt1 is vertical;
					float x_crs,y_crs;
					x_crs=sgmnt1.x0;
					y_crs=sgmnt0.k*x_crs+sgmnt0.b;
					if(mn(sgmnt1.y0,sgmnt1.y1)<y_crs && y_crs<mx(sgmnt1.y0,sgmnt1.y1) && sgmnt0.x0<x_crs && x_crs<sgmnt0.x1){//both segments include crossing point;
						return cs;
					}
					else{
						return n_cs;
					}
				}
				else{//both segments defined as y=k*x+b;
					if(abs(sgmnt0.k-sgmnt1.k)<eps){
						if(abs(sgmnt0.b-sgmnt1.b)>eps){//parellel;
							return n_cs;
						}
						else{
							if(sgmnt0.x1<sgmnt1.x0 || sgmnt1.x1<sgmnt0.x0){//separate segments;
								return n_cs;
							}
							else{//segments have close parts;
								return cs;
							}
						}
					}
					else{
						float x_crs,y_crs;
						x_crs=-1.*((sgmnt1.b-sgmnt0.b)/(sgmnt1.k-sgmnt0.k));
						y_crs=sgmnt1.k*x_crs+sgmnt1.b;
						if(sgmnt0.x0<x_crs && x_crs<sgmnt0.x1 && sgmnt1.x0<x_crs && x_crs<sgmnt1.x1){//both segments include x coordinate of crossing point;
							return cs;
						}
						else{
							return n_cs;
						}
					}
				}
			}
	};

#endif
