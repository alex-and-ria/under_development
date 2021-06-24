#include"shape.h"
#include"theta_trfm.h"

cv::Mat shape::data_img=cv::Mat(Max_y,Max_x,CV_8UC3);

/*shape::shape(_2d_pnt cntr_val, float theta_val): cntr(cntr_val), ornt_z(theta_val){
	nvs=nvs_val;
	vrtcs=new float[2*nvs];
	for(usi i=0;i<2*nvs;i++){
		vrtcs[i]=vrtcs_val[i];
	}
	//set nvs and vrtcs;
	//float mx_dst=0,curr_dst=0;
	//_2d_pnt _2d_pnt_tmp;
	double x_sum=0,y_sum=0;
	for(usi i=0;i<nvs;i++){
		 x_sum+=vrtcs[2*i];
		 y_sum+=vrtcs[2*i+1];
	}
	cntr(x_sum/(nvs+0.),y_sum/(nvs+0.));
	

}*/

void shape::set_dsts(){//maximum distance from center is a distance from center point to one of the vertices; minimum distance is set to 0 but can be updated in constructor of corresponding inherited class;
	min_c_dst=0; max_c_dst=0;
	for(usi i=0;i<nvs;i++){
		if(sqrt((cntr.x-vrtcs[2*i])*(cntr.x-vrtcs[2*i])+(cntr.y-vrtcs[2*i+1])*(cntr.y-vrtcs[2*i+1]))>max_c_dst){
			max_c_dst=sqrt((cntr.x-vrtcs[2*i])*(cntr.x-vrtcs[2*i])+(cntr.y-vrtcs[2*i+1])*(cntr.y-vrtcs[2*i+1]));
		}
	
	}

}

void shape::drw(cv::Mat& data_img,std::vector<shape*>& shps,unsigned int tst_idx/*=0*/){
	cv::Scalar clr=cv::Scalar(0,0,0);
	for(unsigned int i=0;i<shps.size();i++){
		clr=cv::Scalar(0,0,0);
		for(unsigned int j=0;j<shps.at(tst_idx)->lnks.size();j++){//nearest neighbourhoods of tst_idx;
			if(shps.at(i)==shps.at(tst_idx)->lnks.at(j)){
				 clr=cv::Scalar(255,0,0);
				 break;
			}
		
		}
		for(unsigned int j=0;j<shps.at(tst_idx)->flnks.size();j++){//more distant neighbourhoods of tst_idx;
			if(shps.at(i)==shps.at(tst_idx)->flnks.at(j)){
				 clr=cv::Scalar(0,200,200);
				 break;
			}
		
		}
		if(i==tst_idx){
			clr=cv::Scalar(150,0,150);
		}
		for(unsigned int j=0;j<shps.at(i)->nvs;j++){
			cv::line(
				data_img,
				cv::Point(shps.at(i)->vrtcs[(2*j)%(2*shps.at(i)->nvs)],shps.at(i)->vrtcs[(2*j+1)%(2*shps.at(i)->nvs)]),
				cv::Point(shps.at(i)->vrtcs[(2*j+2)%(2*shps.at(i)->nvs)],shps.at(i)->vrtcs[(2*j+3)%(2*shps.at(i)->nvs)]),
				clr
			);//line(InputOutputArray img,Point pt1,Point pt2,const Scalar& color,int thickness=1,int lineType=LINE_8,int shift=0);
		
		}
		//cv::putText(data_img,std::to_string(shps.at(i)->top_vec_idx),cv::Point(shps.at(i)->cntr.x,shps.at(i)->cntr.y),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0));
		
	}
	cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);

}

bool shape::chck_clsn(shape* shp0, shape* shp1){
	float cntr_dst=sqrt((shp0->cntr.x-shp1->cntr.x)*(shp0->cntr.x-shp1->cntr.x)+(shp0->cntr.y-shp1->cntr.y)+(shp0->cntr.y-shp1->cntr.y));
	if(cntr_dst>(shp0->max_c_dst+shp1->max_c_dst)){
		return 0;//no intersection;
	
	}
	if(cntr_dst<(shp0->min_c_dst+shp1->min_c_dst)){
		return 1;//intersection;
	
	}
	sgmnt sgmnts0[shp0->nvs], sgmnts1[shp1->nvs];
	for(uint i=0;i<shp0->nvs;i++){
		sgmnts0[i].set_data(shp0->vrtcs[(2*i)%(2*shp0->nvs)],shp0->vrtcs[(2*i+1)%(2*shp0->nvs)],shp0->vrtcs[(2*i+2)%(2*shp0->nvs)],shp0->vrtcs[(2*i+3)%(2*shp0->nvs)]);
	
	}
	for(uint i=0;i<shp1->nvs;i++){
		sgmnts1[i].set_data(shp1->vrtcs[(2*i)%(2*shp1->nvs)],shp1->vrtcs[(2*i+1)%(2*shp1->nvs)],shp1->vrtcs[(2*i+2)%(2*shp1->nvs)],shp1->vrtcs[(2*i+3)%(2*shp1->nvs)]);
	}
	for(uint i=0;i<shp0->nvs;i++){
		for(uint j=0;j<shp1->nvs;j++){
			if(sgmnt::seg_crs_chck(sgmnts0[i],sgmnts1[j])==cs){
				return 1;//intersection;
			
			}
		
		}
	
	}
	return 0;//else -- no intersection;
	

}

void shape::crs_chck(std::vector<shape*>& shps){//nr_dst<fr_dst should hold;
	float fr_dst=0, nr_dst=0;
	float curr_cntr_dst=0;
	for(uint i=0;i<shps.size();i++){
		for(uint j=0;j<shps.size();j++){
			nr_dst=mx(*(shps.at(i)->get_param(0)),*(shps.at(j)->get_param(0)));
			fr_dst=mx(*(shps.at(i)->get_param(1)),*(shps.at(j)->get_param(1)));
			curr_cntr_dst=sqrt((shps.at(i)->cntr.x-shps.at(j)->cntr.x)*(shps.at(i)->cntr.x-shps.at(j)->cntr.x)+(shps.at(i)->cntr.y-shps.at(j)->cntr.y)*(shps.at(i)->cntr.y-shps.at(j)->cntr.y));
			if(curr_cntr_dst<nr_dst && i!=j){
				shps.at(i)->lnks.push_back(shps.at(j));
			}
			else if(curr_cntr_dst<fr_dst && i!=j){
				shps.at(i)->flnks.push_back(shps.at(j));
			}
			std::sort(shps.at(i)->lnks.begin(),shps.at(i)->lnks.end(),
				[=](shape* shp0, shape* shp1){
					float dst0=(shps.at(i)->cntr.x-shp0->cntr.x)*(shps.at(i)->cntr.x-shp0->cntr.x)+(shps.at(i)->cntr.y-shp0->cntr.y)*(shps.at(i)->cntr.y-shp0->cntr.y);
					float dst1=(shps.at(i)->cntr.x-shp1->cntr.x)*(shps.at(i)->cntr.x-shp1->cntr.x)+(shps.at(i)->cntr.y-shp1->cntr.y)*(shps.at(i)->cntr.y-shp1->cntr.y);
					return dst0<dst1;
				}
			);
			std::sort(shps.at(i)->flnks.begin(),shps.at(i)->flnks.end(),
				[=](shape* shp0, shape* shp1){
					float dst0=(shps.at(i)->cntr.x-shp0->cntr.x)*(shps.at(i)->cntr.x-shp0->cntr.x)+(shps.at(i)->cntr.y-shp0->cntr.y)*(shps.at(i)->cntr.y-shp0->cntr.y);
					float dst1=(shps.at(i)->cntr.x-shp1->cntr.x)*(shps.at(i)->cntr.x-shp1->cntr.x)+(shps.at(i)->cntr.y-shp1->cntr.y)*(shps.at(i)->cntr.y-shp1->cntr.y);
					return dst0<dst1;
				}
			);
		
		}
	
	}



}

double shape::fn_srch(unsigned int N_tgt,std::vector<shape*>& shps,unsigned int tst_idx/*=0*/){
	for(uint i=0;i<shps.size();i++){
		if(shps.at(i)->lnks.size()>=(N_tgt-1)){//get closest N_tgt-1 neighbours of i-th shape;
			std::vector<shape*> sbst(shps.at(i)->lnks.begin(),shps.at(i)->lnks.begin()+(N_tgt-1)); sbst.push_back(shps.at(i));
			std::vector<shape*> wd_sbst; wd_sbst.clear();
			if(shps.at(i)->lnks.size()>(N_tgt-1)){
				wd_sbst.insert(wd_sbst.end(),shps.at(i)->lnks.begin()+N_tgt-1,shps.at(i)->lnks.end());
			
			}
			wd_sbst.insert(wd_sbst.end(),shps.at(i)->flnks.begin(),shps.at(i)->flnks.end());//shapes that may intersect fingertip lines;
			for(float theta_tmp=0; theta_tmp<=acos(-1); theta_tmp+=acos(-1)/8){//rotate (angles (0-pi), (angles (pi-2*pi) repeate check));
				float fx_min,fx_max,fy_min,fy_max,fx_curr,fy_curr;
				pnt_trfm(sbst.at(0)->vrtcs[0],sbst.at(0)->vrtcs[1],theta_tmp,&fx_min,&fy_min);
				fx_max=fx_min;fy_max=fy_min;
				for(uint j=0;j<sbst.size();j++){
					for(uint k=0;k<sbst.at(j)->nvs;k++){
						pnt_trfm(sbst.at(j)->vrtcs[2*k],sbst.at(j)->vrtcs[2*k+1],theta_tmp,&fx_curr,&fy_curr);
						if(fx_max<fx_curr){
							fx_max=fx_curr;
						}
						if(fx_min>fx_curr){
							fx_min=fx_curr;
						}
						if(fy_max<fy_curr){
							fy_max=fy_curr;
						}
						if(fy_min>fy_curr){
							fy_min=fy_curr;
						}
					}
				
				}
				float p0x=fx_min,p0y=fy_min,p1x=fx_min,p1y=fy_max,p2x=fx_max,p2y=fy_max,p3x=fx_max,p3y=fy_min;
				pnt_trfm(p0x,p0y,-theta_tmp,NULL,NULL);
				pnt_trfm(p1x,p1y,-theta_tmp,NULL,NULL);
				pnt_trfm(p2x,p2y,-theta_tmp,NULL,NULL);
				pnt_trfm(p3x,p3y,-theta_tmp,NULL,NULL);
				if(sqrt((p0x-p1x)*(p0x-p1x)+(p0y-p1y)*(p0y-p1y))>grpr_t0::fms){
					std::cout<<"\nfms="<<grpr_t0::fms<<"; dst="<<sqrt((p0x-p1x)*(p0x-p1x)+(p0y-p1y)*(p0y-p1y)); continue;
				}
				cv::line(shape::data_img,cv::Point(p0x,p0y),cv::Point(p1x,p1y),cv::Scalar(0,255,0));
				cv::line(shape::data_img,cv::Point(p1x,p1y),cv::Point(p2x,p2y),cv::Scalar(0,255,0));
				cv::line(shape::data_img,cv::Point(p2x,p2y),cv::Point(p3x,p3y),cv::Scalar(0,255,0));
				cv::line(shape::data_img,cv::Point(p3x,p3y),cv::Point(p0x,p0y),cv::Scalar(0,255,0));
				cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",shape::data_img);
   				cv::waitKey(0);
   				cv::line(shape::data_img,cv::Point(p0x,p0y),cv::Point(p1x,p1y),cv::Scalar(255,255,255));
				cv::line(shape::data_img,cv::Point(p1x,p1y),cv::Point(p2x,p2y),cv::Scalar(255,255,255));
				cv::line(shape::data_img,cv::Point(p2x,p2y),cv::Point(p3x,p3y),cv::Scalar(255,255,255));
				cv::line(shape::data_img,cv::Point(p3x,p3y),cv::Point(p0x,p0y),cv::Scalar(255,255,255));
				
				float lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/(grpr_t0::fl+0.0);
				unsigned int frcs=(lmbd+0.5);
				for(unsigned int curr_fr=2;curr_fr<=(2*frcs);curr_fr++){
					if(curr_fr==2){
						float lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/(grpr_t0::fl+0.0);
						grpr0(p0x,p0y,p1x,p1y,(p0x+(p3x-p0x)/lmbd),(p0y+(p3y-p0y)/lmbd),(p1x+(p2x-p1x)/lmbd),(p1y+(p2y-p1y)/lmbd));
					}
					else if(curr_fr==(2*frcs)){
						lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/(sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))-(grpr_t0::fl+0.0));
						grpr0(p0x+(p3x-p0x)/lmbd,p0y+(p3y-p0y)/lmbd,p1x+(p2x-p1x)/lmbd,p1y+(p2y-p1y)/lmbd,p3x,p3y,p2x,p2y);
					}
					else{
						for(usi j=0;j<4;j++){
							grpr0.crds[j]=(grpr0.crds[j]+grpr0.crds[j+4])/2.;
						}
						lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/((grpr_t0::fl+0.0)*(curr_fr+0.0)/2.);
						grpr0.crds[4]=p0x+(p3x-p0x)/lmbd;
						grpr0.crds[5]=p0y+(p3y-p0y)/lmbd;
						grpr0.crds[6]=p1x+(p2x-p1x)/lmbd;
						grpr0.crds[7]=p1y+(p2y-p1y)/lmbd;
					}
					if(0) std::cout<<"\nlmbd="<<lmbd;
					if(ft_cnfg_chck(grpr0,wd_sbst)==1){
						cv::line(shape::data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(0,0,255));
						cv::line(shape::data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(0,0,255));
						cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",shape::data_img);
						cv::waitKey(0);
						cv::line(shape::data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(255,255,255));
						cv::line(shape::data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(255,255,255));
						continue;
					
					}
					cv::line(shape::data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(200,0,200));
					cv::line(shape::data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(200,0,200));
					cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",shape::data_img);
					cv::waitKey(0);
					cv::line(shape::data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(255,255,255));
					cv::line(shape::data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(255,255,255));
					
				}
				
			
			}
			
			
		
		}
	
	}

}

bool shape::ft_cnfg_chck(grpr_t0& grpr0,std::vector<shape*>& wd_sbst){
	sgmnt ft_sgmnts[2]; grpr0.get_sgmnts(ft_sgmnts);
	sgmnt* shape_sgmnts;
	for(unsigned int i=0;i<wd_sbst.size();i++){
		shape_sgmnts=new sgmnt[wd_sbst.at(i)->nvs];
		for(unsigned int j=0;j<wd_sbst.at(i)->nvs;j++){
			shape_sgmnts[j].set_data(wd_sbst.at(i)->vrtcs[(2*j)%(2*wd_sbst.at(i)->nvs)],wd_sbst.at(i)->vrtcs[(2*j+1)%(2*wd_sbst.at(i)->nvs)],wd_sbst.at(i)->vrtcs[(2*j+2)%(2*wd_sbst.at(i)->nvs)],wd_sbst.at(i)->vrtcs[(2*j+3)%(2*wd_sbst.at(i)->nvs)]);
			if(sgmnt::seg_crs_chck(ft_sgmnts[0],shape_sgmnts[j])==cs || sgmnt::seg_crs_chck(ft_sgmnts[1],shape_sgmnts[j])==cs){
				return 1;
			}
		
		}
		
		delete[] shape_sgmnts;
	
	}
	return 0;
	

}

unsigned int virtual set_tgts(std::vector<shape*>& sbst,std::vector<shape*>& wd_sbst){
	std::vector<shape*> cbnd_sbst; cbnd_sbst.clear();
	
	
	
	wd_sbst.insert(wd_sbst.end(),shps.at(i)->lnks.begin()+N_tgt-1,shps.at(i)->lnks.end());
	
	for()

}
