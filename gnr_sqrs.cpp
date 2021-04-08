#include<iostream>
#include<time.h>
#include<vector>
#include<algorithm>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>

#define rad_to_deg(x) ((((x)/acos(-1))*(180.)))
#define mn(a,b) (((a)<(b))?(a):(b))
#define mx(a,b) (((a)>(b))?(a):(b))
#define abs(x) (((x)>0)?(x):(-(x)))
#define N_sqrs 100
#define Max_x 1024
#define Max_y 512
#define Max_d 40

#include"sgmnt.cpp"

cv::Mat data_img;
unsigned int tst_idx=0;

class grpr_t0{
	public:
		static unsigned int fl;
		static unsigned int fms;
		float crds[8];
		struct center{
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
		}
		void operator()(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3){
			crds[0]=x0; crds[1]=y0; crds[2]=x1; crds[3]=y1; crds[4]=x2; crds[5]=y2; crds[6]=x3; crds[7]=y3;
		}
} grpr0;

unsigned int grpr_t0::fl=100;
unsigned int grpr_t0::fms=90;

class sqr{
	public:
		float ornt;//0-2*acos(-1);
		float diag_len;
		class center{
			public:
				unsigned int x;
				unsigned int y;
				center(unsigned int x_val, unsigned int y_val): x(x_val), y(y_val){ }
				center(){ }
		} cntr;
		std::vector<sqr*> lnks;
		std::vector<sqr*> flnks;
		unsigned int top_vec_idx;
		sqr(unsigned int x_val,unsigned int y_val,float ornt_val,float diag_len_val): cntr(x_val,y_val), ornt(ornt_val),diag_len(diag_len_val){ }
		sqr() { }
		void get_vrtcs(float vrtcs[8]){
			vrtcs[0]=cntr.x-diag_len*sin(ornt); vrtcs[1]=cntr.y-diag_len*cos(ornt);
			vrtcs[2]=cntr.x-diag_len*sin(ornt+acos(-1)/2.); vrtcs[3]=cntr.y-diag_len*cos(ornt+acos(-1)/2.);
			vrtcs[4]=cntr.x-diag_len*sin(ornt+acos(-1)); vrtcs[5]=cntr.y-diag_len*cos(ornt+acos(-1));
			vrtcs[6]=cntr.x-diag_len*sin(ornt+(acos(-1)*3)/2.); vrtcs[7]=cntr.y-diag_len*cos(ornt+(acos(-1)*3)/2.);
		}
		void trfm(float vrtcs[8],float& theta_tmp){
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
		
		}
		void static pnt_trfm(float x, float y, float& x_res, float& y_res, float theta){
			x_res=x*cos(theta)+y*sin(theta);
			y_res=-1.*x*sin(theta)+y*cos(theta);
		}
		void static drw(cv::Mat& data_img,std::vector<sqr>& sqrs){
			float vrtcs[8];
			for(unsigned int i=0;i<sqrs.size();i++){
				sqrs.at(i).get_vrtcs(vrtcs);
				if(0) std::cout<<'\n'<<sqrs.at(i)<<'\n';
				////////////
				bool cntn=0;
				for(unsigned int k=0;k<sqrs.at(tst_idx).lnks.size();k++){
					if(&sqrs.at(i)==sqrs.at(tst_idx).lnks.at(k)){
						cntn=1;
						std::cout<<"\ni="<<i;
						break;
					}
				}
				if(i==tst_idx) cntn=1;
				cv::Scalar clr=cntn*cv::Scalar(255,0,0);
				for(unsigned int k=0;k<sqrs.at(tst_idx).flnks.size();k++){
					if(&sqrs.at(i)==sqrs.at(tst_idx).flnks.at(k)){
						clr=cv::Scalar(100,0,255);
						break;
					}
				}
				/////////////
				for(unsigned short int j=0;j<4;j++){
					cv::line(
						data_img,
						cv::Point(vrtcs[(2*j)%8]+0.5,vrtcs[(2*j+1)%8]+0.5),
						cv::Point((unsigned int)(vrtcs[(2*j+2)%8]+0.5),(unsigned int)(vrtcs[(2*j+3)%8]+0.5)),
						clr
					);//line(InputOutputArray img,Point pt1,Point pt2,const Scalar& color,int thickness=1,int lineType=LINE_8,int shift=0);
					if(0) std::cout<<" !"<<' '<<(unsigned int)(vrtcs[(2*j)%8]+0.5)<<' '<<(unsigned int)(vrtcs[(2*j+1)%8]+0.5)<<' ';
				
				}
				/*cv::line(data_img,cv::Point(vrtcs[0],vrtcs[1]),cv::Point(vrtcs[2],vrtcs[3]),0);
				cv::line(data_img,cv::Point(vrtcs[2],vrtcs[3]),cv::Point(vrtcs[4],vrtcs[5]),0);
				cv::line(data_img,cv::Point(vrtcs[4],vrtcs[5]),cv::Point(vrtcs[6],vrtcs[7]),0);
				cv::line(data_img,cv::Point(vrtcs[6],vrtcs[7]),cv::Point(vrtcs[0],vrtcs[1]),0);*/
				//std::cout<<"\ngen_sqrs:i="<<i;
				cv::putText(data_img,std::to_string(sqrs.at(i).top_vec_idx),cv::Point(sqrs.at(i).cntr.x,sqrs.at(i).cntr.y),cv::FONT_HERSHEY_SIMPLEX,0.5,cv::Scalar(0,0,0));
				cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
				//cv::waitKey(0);
			}
		
		}
		bool static ft_cnfg_chck(std::vector<sqr*>& wd_sbst){
			std::cout<<"\nwd_sbst:";
			for(unsigned int i=0;i<wd_sbst.size();i++){
				std::cout<<' '<<wd_sbst.at(i)->top_vec_idx;
			
			}
			sgmnt sgmnt_ft[2], sgmnt_sqr[4], sgmnt_lt_sd[2], sgmnt_cntr; float vrtcs[8];
			sgmnt_ft[0].set_data(grpr0.crds[0],grpr0.crds[1],grpr0.crds[4],grpr0.crds[5]);
			sgmnt_ft[1].set_data(grpr0.crds[2],grpr0.crds[3],grpr0.crds[6],grpr0.crds[7]);
			sgmnt_lt_sd[0].set_data(grpr0.crds[0],grpr0.crds[1],grpr0.crds[2],grpr0.crds[3]);
			sgmnt_lt_sd[1].set_data(grpr0.crds[4],grpr0.crds[5],grpr0.crds[6],grpr0.crds[7]);
			unsigned int crs_cnt=0;
			for(unsigned int j=0;j<wd_sbst.size();j++){
				crs_cnt=0;
				wd_sbst.at(j)->get_vrtcs(vrtcs);
				for(unsigned int k=0;k<4;k++){
					sgmnt_sqr[k].set_data(vrtcs[(2*k)%8],vrtcs[(2*k+1)%8],vrtcs[(2*k+2)%8],vrtcs[(2*k+3)%8]);
				}
				for(unsigned int k=0;k<4;k++){
					if(sgmnt::seg_crs_chck(sgmnt_ft[0],sgmnt_sqr[k])==cs || sgmnt::seg_crs_chck(sgmnt_ft[1],sgmnt_sqr[k])==cs){
						cv::line(data_img,cv::Point(sgmnt_sqr[k].x0,sgmnt_sqr[k].y0),cv::Point(sgmnt_sqr[k].x1,sgmnt_sqr[k].y1),cv::Scalar(0,0,255),2);
						cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
						cv::waitKey(0);
						cv::line(data_img,cv::Point(sgmnt_sqr[k].x0,sgmnt_sqr[k].y0),cv::Point(sgmnt_sqr[k].x1,sgmnt_sqr[k].y1),cv::Scalar(255,255,255),2);
						cv::line(data_img,cv::Point(sgmnt_sqr[k].x0,sgmnt_sqr[k].y0),cv::Point(sgmnt_sqr[k].x1,sgmnt_sqr[k].y1),cv::Scalar(0,0,0));
						std::cout<<"\nft_crs_chck;";
						return 1;

					}

				}
				/*sgmnt_cntr.set_data(wd_sbst.at(j)->cntr.x,wd_sbst.at(j)->cntr.y,Max_x,wd_sbst.at(j)->cntr.y);
				for(unsigned short int k=0;k<2;k++){
					if(sgmnt::seg_crs_chck(sgmnt_ft[k],sgmnt_cntr)==cs){
						crs_cnt++;
					}
				}
				for(unsigned short int k=0;k<2;k++){
					if(sgmnt::seg_crs_chck(sgmnt_lt_sd[k],sgmnt_cntr)==cs){
						crs_cnt++;
					}
				}
				std::cout<<"\ncrs_cnt="<<crs_cnt;
				if(crs_cnt%2==1){//center of neighbouring square is inside rectangle made by fingertips;
					return 1;
				
				}*/
				
				
			}
			return 0;
		
		}
		void static crs_chck(std::vector<sqr>& sqrs){
			float fr_dst, nr_dst;
			for(unsigned int i=0;i<sqrs.size();i++){
				for(unsigned int j=0;j<sqrs.size();j++){
					nr_dst=sqrt(grpr_t0::fms*grpr_t0::fms+grpr_t0::fl*grpr_t0::fl);
					fr_dst=sqrt((grpr_t0::fms+sqrs.at(j).diag_len)*(grpr_t0::fms+sqrs.at(j).diag_len)+grpr_t0::fl*grpr_t0::fl);
					if(sqrt((sqrs.at(i).cntr.x-sqrs.at(j).cntr.x)*(sqrs.at(i).cntr.x-sqrs.at(j).cntr.x)+(sqrs.at(i).cntr.y-sqrs.at(j).cntr.y)*(sqrs.at(i).cntr.y-sqrs.at(j).cntr.y))<nr_dst && i!=j){
						sqrs.at(i).lnks.push_back(&sqrs.at(j));
						if(i==tst_idx) std::cout<<"\ni11: "<<j;
					}
					else if(sqrt((sqrs.at(i).cntr.x-sqrs.at(j).cntr.x)*(sqrs.at(i).cntr.x-sqrs.at(j).cntr.x)+(sqrs.at(i).cntr.y-sqrs.at(j).cntr.y)*(sqrs.at(i).cntr.y-sqrs.at(j).cntr.y))<fr_dst && i!=j){
						sqrs.at(i).flnks.push_back(&sqrs.at(j));
					}
				
				}
				std::sort(sqrs.at(i).lnks.begin(),sqrs.at(i).lnks.end(),
					[=](sqr* sqr0, sqr* sqr1){
						float dst0=(sqrs.at(i).cntr.x-sqr0->cntr.x)*(sqrs.at(i).cntr.x-sqr0->cntr.x)+(sqrs.at(i).cntr.y-sqr0->cntr.y)*(sqrs.at(i).cntr.y-sqr0->cntr.y);
						float dst1=(sqrs.at(i).cntr.x-sqr1->cntr.x)*(sqrs.at(i).cntr.x-sqr1->cntr.x)+(sqrs.at(i).cntr.y-sqr1->cntr.y)*(sqrs.at(i).cntr.y-sqr1->cntr.y);
						return dst0<dst1;
						}
					);
				std::sort(sqrs.at(i).flnks.begin(),sqrs.at(i).flnks.end(),
					[=](sqr* sqr0, sqr* sqr1){
						float dst0=(sqrs.at(i).cntr.x-sqr0->cntr.x)*(sqrs.at(i).cntr.x-sqr0->cntr.x)+(sqrs.at(i).cntr.y-sqr0->cntr.y)*(sqrs.at(i).cntr.y-sqr0->cntr.y);
						float dst1=(sqrs.at(i).cntr.x-sqr1->cntr.x)*(sqrs.at(i).cntr.x-sqr1->cntr.x)+(sqrs.at(i).cntr.y-sqr1->cntr.y)*(sqrs.at(i).cntr.y-sqr1->cntr.y);
						return dst0<dst1;
						}
					);
			}
		
		}
		double static fn_srch(unsigned int N_tgt,std::vector<sqr>& sqrs){
			bool is_avlb=0,is_zw=0; double wght=0; float rslt_ft_crds[8];
			for(unsigned int i=0;i<sqrs.size();i++){
				if(sqrs.at(i).lnks.size()>=(N_tgt-1)){//get closest N_tgt-1 neighbours of i-th square;
					std::vector<sqr*> sbst(sqrs.at(i).lnks.begin(),sqrs.at(i).lnks.begin()+(N_tgt-1));
					sbst.push_back(&(sqrs.at(i)));//N_tgt squares;
					std::vector<sqr*> wd_sbst; wd_sbst.clear();
					if(sqrs.at(i).lnks.size()>(N_tgt-1))
						wd_sbst.insert(wd_sbst.end(),sqrs.at(i).lnks.begin()+N_tgt-1,sqrs.at(i).lnks.end());
					wd_sbst.insert(wd_sbst.end(),sqrs.at(i).flnks.begin(),sqrs.at(i).flnks.end());//squares that may intersect fingertip lines;
					if(0){
						std::cout<<"\nsqrs_lnks_tvi's:";
						for(unsigned int j=0;j<sqrs.at(i).lnks.size();j++){
							std::cout<<' '<<sqrs.at(i).lnks.at(j)->top_vec_idx;
						}
						std::cout<<"\nsbst_tvi's:";
						for(unsigned int j=0;j<sbst.size();j++){
							std::cout<<' '<<sbst.at(j)->top_vec_idx;
						}
						std::cout<<"\nwd_sbst_tvi's:";
						for(unsigned int j=0;j<wd_sbst.size();j++){
							std::cout<<' '<<wd_sbst.at(j)->top_vec_idx;
						}
					
					}
					float vrtcs[8], vrtcs_trfm[8]; sbst.at(0)->get_vrtcs(vrtcs);
					float xmin=vrtcs[0],xmax=vrtcs[0],ymin=vrtcs[1],ymax=vrtcs[1];
					/*for(unsigned int j=0;j<sbst.size();j++){
						 sbst.at(j)->get_vrtcs(vrtcs);
						 for(unsigned int k=0;k<4;k++){
						 	if(vrtcs[2*k]<xmin) xmin=vrtcs[2*k];
						 	if(vrtcs[2*k]>xmax) xmax=vrtcs[2*k];
						 	if(vrtcs[2*k+1]<ymin) ymin=vrtcs[2*k+1];
						 	if(vrtcs[2*k+1]>ymax) ymax=vrtcs[2*k+1];
						 }
					}
					cv::line(data_img,cv::Point(xmin,ymin),cv::Point(xmin,ymax),cv::Scalar(255,255,0));
					cv::line(data_img,cv::Point(xmin,ymax),cv::Point(xmax,ymax),cv::Scalar(255,255,0));
					cv::line(data_img,cv::Point(xmax,ymax),cv::Point(xmax,ymin),cv::Scalar(255,255,0));
					cv::line(data_img,cv::Point(xmin,ymin),cv::Point(xmax,ymin),cv::Scalar(255,255,0));
					cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
					cv::waitKey(0);*/
					//float x_cntr, y_cntr;
					for(float theta_tmp=0; theta_tmp<=acos(-1); theta_tmp+=acos(-1)/8){//rotate (angles (0-pi), (angles (pi-2*pi) repeate check));
						sbst.at(0)->get_vrtcs(vrtcs_trfm); sbst.at(0)->trfm(vrtcs_trfm,theta_tmp);
						xmin=vrtcs_trfm[0],xmax=vrtcs_trfm[0],ymin=vrtcs_trfm[1],ymax=vrtcs_trfm[1];
						for(unsigned int j=0;j<sbst.size();j++){
							 sbst.at(j)->get_vrtcs(vrtcs_trfm); sbst.at(j)->trfm(vrtcs_trfm,theta_tmp);
							 for(unsigned int k=0;k<4;k++){
							 	if(vrtcs_trfm[2*k]<xmin){
							 		xmin=vrtcs_trfm[2*k];
							 	}
							 	if(vrtcs_trfm[2*k]>xmax){
							 		xmax=vrtcs_trfm[2*k];
							 	}
							 	if(vrtcs_trfm[2*k+1]<ymin){
							 		ymin=vrtcs_trfm[2*k+1];
							 	}
							 	if(vrtcs_trfm[2*k+1]>ymax){
							 		ymax=vrtcs_trfm[2*k+1];
							 	}
							 }
						}
						float p0x=xmin,p0y=ymin,p1x=xmin,p1y=ymax,p2x=xmax,p2y=ymax,p3x=xmax,p3y=ymin;
						sqr::pnt_trfm(xmin,ymin,p0x,p0y,-1*theta_tmp);
						sqr::pnt_trfm(xmin,ymax,p1x,p1y,-1*theta_tmp);
						sqr::pnt_trfm(xmax,ymax,p2x,p2y,-1*theta_tmp);
						sqr::pnt_trfm(xmax,ymin,p3x,p3y,-1*theta_tmp);
						
						cv::line(data_img,cv::Point(p0x,p0y),cv::Point(p1x,p1y),cv::Scalar(0,255,0));
						cv::line(data_img,cv::Point(p1x,p1y),cv::Point(p2x,p2y),cv::Scalar(0,255,0));
						cv::line(data_img,cv::Point(p2x,p2y),cv::Point(p3x,p3y),cv::Scalar(0,255,0));
						cv::line(data_img,cv::Point(p3x,p3y),cv::Point(p0x,p0y),cv::Scalar(0,255,0));
						
						
						if(sqrt((p0x-p1x)*(p0x-p1x)+(p0y-p1y)*(p0y-p1y))>grpr_t0::fms){
							std::cout<<"\ndst(p0,p1) ("<<sqrt((p0x-p1x)*(p0x-p1x)+(p0y-p1y)*(p0y-p1y))<<") ("<<grpr_t0::fms<<")";
						
						}
						else{
							float lmbd=1.;
							unsigned int frcs=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/((grpr_t0::fl+0.)/2.)-1;
							std::cout<<"\np1="<<p1x<<' '<<p1y<<" p2="<<p2x<<' '<<p2y<<" frcs="<<frcs;
							for(unsigned short int curr_fr=0;curr_fr<(frcs+2);curr_fr++){//fractions;
								if(curr_fr==0){
									lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/(sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))-grpr_t0::fl+0.);
									grpr0.crds[0]=(p3x-p0x+0.)/lmbd+p0x;
									grpr0.crds[1]=(p3y-p0y+0.)/lmbd+p0y;
									grpr0.crds[2]=(p2x-p1x+0.)/lmbd+p1x;
									grpr0.crds[3]=(p2y-p1y+0.)/lmbd+p1y;
									grpr0.crds[4]=p3x;
									grpr0.crds[5]=p3y;
									grpr0.crds[6]=p2x;
									grpr0.crds[7]=p2y;
								}
								else if(curr_fr==frcs+1){
									lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/(grpr_t0::fl+0.);
									grpr0.crds[0]=p0x;
									grpr0.crds[1]=p0y;
									grpr0.crds[2]=p1x;
									grpr0.crds[3]=p1y;
									grpr0.crds[4]=(p3x-p0x+0.)/lmbd+p0x;
									grpr0.crds[5]=(p3y-p0y+0.)/lmbd+p0y;
									grpr0.crds[6]=(p2x-p1x+0.)/lmbd+p1x;
									grpr0.crds[7]=(p2y-p1y+0.)/lmbd+p1y;
								}
								else{
									if(curr_fr==1){
										grpr0(p0x,p0y,p1x,p1y,p0x,p0y,p1x,p1y);//p0,p1,p0_;p1_;
									}
									is_zw=0;
									for(unsigned int j=0;j<4;j++){
										grpr0.crds[j]=(grpr0.crds[j]+grpr0.crds[j+4])/2.;
									}
									lmbd=sqrt((p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y))/((grpr_t0::fl+0.)*(1.+(curr_fr-1.+0.0)/2.));
									grpr0.crds[4]=(p3x-p0x+0.)/lmbd+p0x;
									grpr0.crds[5]=(p3y-p0y+0.)/lmbd+p0y;
									grpr0.crds[6]=(p2x-p1x+0.)/lmbd+p1x;
									grpr0.crds[7]=(p2y-p1y+0.)/lmbd+p1y;
							
								
								}
								if(0) std::cout<<"\ncurr_fr="<<curr_fr<<" lmbd="<<lmbd<<" crds: ";
								if(0) for(unsigned int k=0;k<7;k++){
									std::cout<<grpr0.crds[k]<<' ';
							
								}
								std::cout<<"dst="<<sqrt((grpr0.crds[0]-grpr0.crds[4])*(grpr0.crds[0]-grpr0.crds[4])+(grpr0.crds[1]-grpr0.crds[5])*(grpr0.crds[1]-grpr0.crds[5]));
							
								cv::line(data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(255,0,255));
								cv::line(data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(255,0,255));
								cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
								cv::waitKey(0);
								cv::line(data_img,cv::Point(grpr0.crds[0],grpr0.crds[1]),cv::Point(grpr0.crds[4],grpr0.crds[5]),cv::Scalar(255,255,255));
								cv::line(data_img,cv::Point(grpr0.crds[2],grpr0.crds[3]),cv::Point(grpr0.crds[6],grpr0.crds[7]),cv::Scalar(255,255,255));
								
								
								if(0) std::cout<<"\nwd_sbst0:";
								if(0) for(unsigned int k=0;k<wd_sbst.size();k++){
									std::cout<<' '<<wd_sbst.at(k)->top_vec_idx;
			
								}
								if(sqr::ft_cnfg_chck(wd_sbst)==0){//no intersections with fingertips;
									std::cout<<"\nni!";
									
								
								}
								
							}
						}
						cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
						cv::waitKey(0);
					
					
					}
					
					
				}
				
			
			}
			return 0;
			
		
		}
		friend std::ostream& operator<<(std::ostream& os,const sqr& sqr_val){
			os<<" cntr:"<<" ("<<sqr_val.cntr.x<<','<<sqr_val.cntr.y<<") ornt="<<rad_to_deg(sqr_val.ornt)<<" diag_len="<<sqr_val.diag_len;
		}
};

bool chck_clsn(sqr sqr0, sqr sqr1){
	float cntr_dst=sqrt((sqr0.cntr.x-sqr1.cntr.x)*(sqr0.cntr.x-sqr1.cntr.x)+(sqr0.cntr.y-sqr1.cntr.y)*(sqr0.cntr.y-sqr1.cntr.y));
	if(cntr_dst>(sqr0.diag_len+sqr1.diag_len)){	
		return 0;//no intersection;
	}
	//return 1;
	float cntr_hfhts=sqr0.diag_len*cos(acos(-1)/4.)+sqr1.diag_len*cos(acos(-1)/4.);
	if(cntr_dst<cntr_hfhts){
		return 1;//intersection;
	}
	sgmnt sqr0_sgts[4],sqr1_sgts[4];
	float vrtcs0[8],vrtcs1[8];
	sqr0.get_vrtcs(vrtcs0); sqr1.get_vrtcs(vrtcs1);
	for(unsigned short int i=0;i<4;i++){
		sqr0_sgts[i].set_data(vrtcs0[(2*i)%8],vrtcs0[(2*i+1)%8],vrtcs0[(2*i+2)%8],vrtcs0[(2*i+3)%8]);
		sqr1_sgts[i].set_data(vrtcs1[(2*i)%8],vrtcs1[(2*i+1)%8],vrtcs1[(2*i+2)%8],vrtcs1[(2*i+3)%8]);
	}
	for(unsigned short int i=0;i<4;i++){
		for(unsigned short int j=0;j<4;j++){
			if(sgmnt::seg_crs_chck(sqr0_sgts[i],sqr1_sgts[j])==cs){
				return 1;//intersection;
			}
		}
	}
	return 0;//else -- no intersection;
}

void gen_sqrs(std::vector<sqr>& sqrs){
	sqrs.clear(); sqrs.resize(N_sqrs);
	bool is_crs=1; sqr sqr_tmp;
	for(unsigned int i=0;i<N_sqrs;i++){
	
	
		if(i==17){
			std::cout<<i;
		
		}
	
	
		is_crs=1;
		while(is_crs==1){
			sqr_tmp=sqr(rand()%Max_x,rand()%Max_y,((rand()%(unsigned int)(2*acos(-1)*100))/100.),/*rand()%Max_d*/Max_d/2);
			is_crs=0;
			for(unsigned int j=0;j<i;j++){
				if(chck_clsn(sqr_tmp,sqrs.at(j))==1){
					is_crs=1; break;
			
				}
		
			}
		
		}
		sqr_tmp.top_vec_idx=i;
		sqrs.at(i)=sqr_tmp;
		//sqrs.at(i)=sqr(Max_x/2,Max_y/2,0,20);
		
	}	
	
}

void onMouseCb(int event, int x, int y, int flags, void *usr_data){
	static int prev_x, prev_y,clb_cnt=0;
	std::vector<sqr>* sqrs=(std::vector<sqr>*)usr_data;
	if(event==cv::EVENT_RBUTTONUP){
		std::cout<<"\nrb";
		cv::setMouseCallback("data_img",NULL,NULL);
		std::cout<<"\nsqrs.size()="<<sqrs->size();
		sqr::drw(data_img,*sqrs);
		return;
	}
	if(event==cv::EVENT_LBUTTONUP){
		std::cout<<"\nlb";
		if(clb_cnt%2==0){
			prev_x=x; prev_y=y;
		}
		else if(clb_cnt%2==1){
			float f_hptns=sqrt((prev_x-x)*(prev_x-x)+(prev_y-y)*(prev_y-y));
			sqr sqr_tmp=sqr(prev_x,prev_y,asin(abs(prev_x-x)/(f_hptns)),Max_d/2);
			std::cout<<"\nasin(abs(prev_x-x)/(Max_d/2.))="<<asin(abs(prev_x-x)/(Max_d/2.));
			
			bool is_crs=0;
			for(unsigned int i=0;i<sqrs->size();i++){
				if(chck_clsn(sqr_tmp,sqrs->at(i))==1){
					std::cout<<"\nis_crs=="<<is_crs;
					is_crs=1; break;
			
				}
		
			}
			if(is_crs==0){
				sqrs->push_back(sqr_tmp);
				sqr::drw(data_img,*sqrs);
			
			}
			std::cout<<"\nsqrs.size()="<<sqrs->size();
			
		
		}
		clb_cnt++;
		return;
		
	
	}
	

}

void cg_sqrs(std::vector<sqr>& sqrs){
	unsigned int clb_cnt;
	cv::setMouseCallback("data_img",onMouseCb,&sqrs);
	cv::waitKey(0);
	
	

}

int main(){
	sqr sqr_tmp(5,6,7,8);
	std::cout<<"\nsqr_tmp.cntr.x="<<sqr_tmp.cntr.x;
	//srand(time(NULL));
	unsigned int sseed=(unsigned int)161786284217.;//time(NULL);
	std::cout<<"\nsseed="<<sseed;
	srand((unsigned int)161786284217./*sseed*/);
	//cv::Mat data_img(Max_y,Max_x,CV_8UC1,255);//cv::Mat::Mat(int rows,int cols,int type,const Scalar& s);
	data_img.create(Max_y,Max_x,CV_8UC3);
	for(unsigned int i=0;i<data_img.rows;i++){
		for(unsigned int j=0;j<data_img.cols;j++){
			cv::Vec3b &pxl = data_img.at<cv::Vec3b>(i,j);
			pxl[0]=pxl[1]=pxl[2]=255;
		
		}
	
	}
	
	std::vector<sqr> sqrs;
	gen_sqrs(sqrs);
	cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
	//cg_sqrs(sqrs);
	if(0) std::cout<<"\nhere0";
	sqr::crs_chck(sqrs);
	
    sqr::drw(data_img,sqrs);
    sqr::fn_srch(2,sqrs);
    
    
    
    
    cv::namedWindow("data_img", cv::WINDOW_AUTOSIZE ); cv::imshow("data_img",data_img);
	cv::waitKey(0);
	
	//std::cout<<mn(5.2,3)<<' '<<mx(5.2,1);
	
	
	return 0;    






}
