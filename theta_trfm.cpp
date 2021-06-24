#ifndef THETA_TRFM_CPP
#define THETA_TRFM_CPP
#include<math.h>
#include<cstdlib>

/*void trfm(float vrtcs[8],float& theta_tmp){
	if(0) std::cout<<"\n vrtcs=";
	if(0) for(unsigned short int i=0;i<8;i++){
		std::cout<<vrtcs[i]<<' ';
	}
	float flt_tmp=0;
	for(unsigned short int i=0;i<4;i++){
		flt_tmp=vrtcs[2*i];
		vrtcs[2*i]=flt_tmp*cos(theta_tmp)+vrtcs[2*i+1]*sin(theta_tmp);
		vrtcs[2*i+1]=-1.*flt_tmp*sin(theta_tmp)+vrtcs[2*i+1]*cos(theta_tmp);
	}
	if(0) std::cout<<"\ntvrtcs=";
	if(0) for(unsigned short int i=0;i<8;i++){
		std::cout<<vrtcs[i]<<' ';
	}
	if(0) std::cout<<"\nmatrix:\n"<<cos(theta_tmp)<<' '<<sin(theta_tmp)<<'\n'<<-1.*sin(theta_tmp)<<' '<<cos(theta_tmp);
	if(0) cv::waitKey(0);

}*/

void pnt_trfm(float& x, float& y,float theta, float* x_res=NULL, float* y_res=NULL){
	if(x_res==NULL || y_res==NULL){
		float fx_tmp=x*cos(theta)+y*sin(theta), fy_tmp=-1.*x*sin(theta)+y*cos(theta);
		x=fx_tmp; y=fy_tmp;
	
	}
	else{
		(*x_res)=x*cos(theta)+y*sin(theta);
		(*y_res)=-1.*x*sin(theta)+y*cos(theta);
	
	}
	
}

#endif
