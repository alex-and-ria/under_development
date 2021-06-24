#include<iostream>
#include<time.h>
#include<fstream>
#include<vector>
#include<algorithm>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

#define rad_to_deg(x) ((((x)/acos(-1))*(180.)))
#define mn(a,b) (((a)<(b))?(a):(b))
#define mx(a,b) (((a)>(b))?(a):(b))
#define abs(x) (((x)>0)?(x):(-(x)))
#define N_recs 100
#define ifn "q"

#include"sgmnt.cpp"
#include"shape.h"
#include"2d_pnt.cpp"
#include"square.h"

unsigned int tst_idx=0;

void gen_shapes(std::vector<shape*>& shapes){
	shapes.clear(); shapes.resize(N_recs);
	bool is_crs=1; shape* shp_tmp;
	_2d_pnt _2d_pnt_tmp;
	for(unsigned int i=0;i<N_recs;i++){
	
		is_crs=1;
		while(is_crs==1){
			_2d_pnt_tmp(rand()%Max_x,rand()%Max_y);
			shp_tmp=new square(_2d_pnt_tmp,(rand()%(unsigned int)(2*acos(-1)*100))/100.);
			is_crs=0;
			for(unsigned int j=0;j<i;j++){
				if(shape::chck_clsn(shp_tmp,shapes.at(j))==1){
					is_crs=1;
					delete shp_tmp;
					break;
			
				}
		
			}
		
		}
		shp_tmp->top_vec_idx=i;
		shapes.at(i)=shp_tmp;
		//sqrs.at(i)=sqr(Max_x/2,Max_y/2,0,20);
		
	}	
	
}

bool get_shapes(std::vector<shape*>& shapes, float flp[4]){//flp[0] -- file data x_min, flp[1] -- file data x_max, flp[2] -- file data y_min,flp[3] -- file data y_max;
	std::ifstream ifo(ifn,std::ios::in|std::ios::binary);
	if(ifo.is_open()){
		_2d_pnt _2d_pnt_tmp;
		float file_ln[6];
		for(unsigned int i=0;i<6;i++){//x,y,z,roll,pitch,yaw;
			ifo>>file_ln[i];
		}
		flp[0]=file_ln[0]; flp[1]=flp[0]; flp[2]=file_ln[1]; flp[3]=flp[2];
		ifo.seekg(0,std::ios::beg);
		while(1){
			ifo>>file_ln[0];
			if(ifo.eof()){
				break;
			}
			for(unsigned int i=1;i<6;i++){//x,y,z,roll,pitch,yaw;
				ifo>>file_ln[i];
			}
			if(file_ln[0]<flp[0]) flp[0]=file_ln[0];//min_x;
			if(file_ln[0]>flp[1]) flp[1]=file_ln[0];//max_x;
			if(file_ln[1]<flp[2]) flp[2]=file_ln[1];//min_y;
			if(file_ln[1]>flp[3]) flp[3]=file_ln[1];//max_y;
		}
		ifo.clear(); ifo.seekg(0,std::ios::beg);
		while(1){
			ifo>>file_ln[0];
			if(ifo.eof()){
				break;
			}
			for(unsigned int i=1;i<6;i++){//x,y,z,roll,pitch,yaw;
				ifo>>file_ln[i];
			}
			_2d_pnt_tmp((file_ln[0]-flp[0])*1000,(file_ln[1]-flp[2])*1000);
			shapes.push_back(new square(_2d_pnt_tmp,file_ln[5]));
		}
		ifo.close();
		std::cout<<"\nflp="<<flp[0]<<' '<<flp[1]<<' '<<flp[2]<<' '<<flp[3];
		if(0) for(unsigned int i=0;i<shapes.size();i++){
			std::cout<<"\ncntr=("<<shapes.at(i)->cntr.x<<","<<shapes.at(i)->cntr.y<<")";
		
		}
		shape::drw(shape::data_img,shapes);
		cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",shape::data_img);
	   	cv::waitKey(0);
		return 1;
	}
	else{
		std::cout<<'\n'<<ifn<<"ifo.is_open()";
		return 0;	
	}
	

}

int main(){
	srand(time(NULL));
	unsigned int sseed=rand();
	std::cout<<"\nsseed="<<sseed;
	srand(/*(unsigned int)161786284217./**/sseed/**/);
	//cv::Mat data_img(Max_y,Max_x,CV_8UC1,255);//cv::Mat::Mat(int rows,int cols,int type,const Scalar& s);
	//shape::data_img=cv::Mat(Max_y,Max_x,CV_8UC3);
	for(unsigned int i=0;i<shape::data_img.rows;i++){
		for(unsigned int j=0;j<shape::data_img.cols;j++){
			cv::Vec3b &pxl = shape::data_img.at<cv::Vec3b>(i,j);
			pxl[0]=pxl[1]=pxl[2]=255;
		
		}
	
	}
	
	std::vector<shape*> shapes;
	gen_shapes(shapes);
	//float flp[4]; if(get_shapes(shapes,flp)==0) return 1;
	//cv::resize(shape::data_img,shape::data_img,cv::Size((int)((flp[1]-flp[0])*1000+0.5),(int)((flp[3]-flp[2])*1000+0.5)),1,1);
	
	//cg_sqrs(sqrs);
	shape::crs_chck(shapes);
	//shape::fn_srch(2,shapes);
	
    shape::drw(shape::data_img,shapes);
    cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",shape::data_img);
    cv::waitKey(0);
	
	shape::fn_srch(2,shapes);
	
	return 0;    






}
