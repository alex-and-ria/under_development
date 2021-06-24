#ifndef MS_CBCK_CPP
#define MS_CBCK_CPP

#include"shape.h"
#include"2d_pnt.cpp"

void onMouseCb(int event, int x, int y, int flags, void *usr_data){
	static int prev_x, prev_y,clb_cnt=0;
	std::vector<shape*>* shps=(std::vector<shape*>*)usr_data;
	if(event==cv::EVENT_RBUTTONUP){
		std::cout<<"\nrb";
		cv::setMouseCallback("data_img",NULL,NULL);
		std::cout<<"\nshps.size()="<<shps->size();
		
		
		
		
		shps->at(0)->drw(shape::data_img,*shps);
		
		
		return;
	}
	if(event==cv::EVENT_LBUTTONUP){
		std::cout<<"\nlb";
		if(clb_cnt%2==0){
			prev_x=x; prev_y=y;
		}
		else if(clb_cnt%2==1){
			float f_hptns=sqrt((prev_x-x)*(prev_x-x)+(prev_y-y)*(prev_y-y));
			_2d_pnt cntr_val; cntr_val(prev_x,prev_y);
			shape* shape_tmp=NULL;//=new shape(cntr_val,asin(abs(prev_x-x)/(f_hptns)));//TODO
			if(0) std::cout<<"\nasin(abs(prev_x-x)/(f_hptns))="<<asin(abs(prev_x-x)/(f_hptns));
			
			bool is_crs=0;
			for(unsigned int i=0;i<shps->size();i++){
				if(shps->at(0)->chck_clsn(shape_tmp,shps->at(i))==1){
					std::cout<<"\nis_crs=="<<is_crs;
					is_crs=1; break;
			
				}
		
			}
			if(is_crs==0){
				shps->push_back(shape_tmp);
				shps->at(0)->drw(shape::data_img,*shps);
			
			}
			std::cout<<"\nshps->size()="<<shps->size();
			
		
		}
		clb_cnt++;
		return;
		
	
	}
	

}

void cg_sqrs(std::vector<shape*>& shps){
	//unsigned int clb_cnt;
	cv::setMouseCallback("data_img",onMouseCb,&shps);
	cv::waitKey(0);
	
	

}

#endif
