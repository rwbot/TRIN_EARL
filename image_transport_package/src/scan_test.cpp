#include "ros/ros.h"            //ROS
#include <stdio.h>
#include <iostream>
#include <sensor_msgs/Image.h> 
#include <sensor_msgs/image_encodings.h>    //Converting ROS to OpenCV images
#include <cv_bridge/cv_bridge.h>            //Converting ROS to OpenCV images
#include <image_transport/image_transport.h>//Publishing and subscribing to images in ROS
#include <opencv2/imgproc/imgproc.hpp>      //Converting ROS to OpenCV images
#include "opencv2/core/core.hpp"            //OpenCV Stitching
#include "opencv2/features2d/features2d.hpp"//OpenCV Stitching
#include "opencv2/highgui/highgui.hpp"      //OpenCV Stitching 
#include "std_msgs/Int16MultiArray.h"
#include "math.h"
#include <sensor_msgs/LaserScan.h>

using namespace cv;
using namespace std;


int points;
Point pt1, pt2;
int sf =2;
float lineArray[180];
int end;
bool empty = true;


bool check(int x, int y){

  if(x<17){
    return false;
  }else if(x<28){
    if(y<73)
      return false;
  }else if(x<38){
    if(y<57)
      return false;
  }else if(x<45){
    if(y<45)
      return false;
  }else if(x<51){
    if(y<35)
      return false;
  }else if(x<57){
    if(y<25)
      return false;
  }else if(x<62){
    if(y<17)
      return false;
  }


  if(y>117){
    return false;
  }else if(y>111){
    if(x<122)
      return false;
  }else if(y>105){
    if(x<93)
      return false;
  }else if(y>101){
    if(x<70)
      return false;
  }else if(y>96){
    if(x<50)
      return false;
  }else if(y>92){
    if(x<32)
      return false;
  }else if(y>90){
    if(x<18)
      return false;
  }

  if(y<7){
    return false;
  }else if(y<9){
    if(x<137)
      return false;
  }else if(y<11){
    if(x<120)
      return false;
  }else if(y<12){
    if(x<104)
      return false;
  }else if(y<14){
    if(x<89)
      return false;
  }else if(y<16){
    if(x<76)
      return false;
  }else if(y<17){
    if(x<63)
      return false;
  }
  return true;

}


void onMouse( int event, int x, int y, int flags, void* param ){
  if( event == CV_EVENT_LBUTTONDOWN )  {  
    Mat &final_mat = *((Mat*)(param));

    if(check(x/sf,y/sf) == false){
      cout<<"========================================================================="<<endl;
      cout<<endl<<"++++INVALID PIXEL -- OUTSIDE OF MAPPING REGION ++++"<<endl<<endl; 
      cout<<"========================================================================="<<endl;
      rectangle(final_mat, Point(0,0), Point(1280,480), Scalar(0,0,0),-1,8, 0);
      line(final_mat, Point(315,235), Point(35,180), Scalar(255,0,0), 2, 8, 0); //0TL
      line(final_mat, Point(315,15), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6TL
      line(final_mat, Point(35,180), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6L
      line(final_mat, Point(315,235), Point(315,15), Scalar(255,0,0), 2, 8, 0); //0
      line(final_mat, Point(585,175), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6R
      line(final_mat, Point(315,235), Point(585,175), Scalar(255,0,0), 2, 8, 0); //0TR
      line(final_mat, Point(315,15), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6TR
     line(final_mat, Point(315,90), Point(90,90), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(140,200), Point(210,25), Scalar(255,0,0), 2, 8, 0); //3
      line(final_mat, Point(485,195), Point(415,25), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(315,90), Point(530,90), Scalar(255,0,0), 2, 8, 0);  //3
      imshow("FINAL", final_mat);
      points = 0;
      return;
    }else if(x/sf>157){
      cout<<"========================================================================="<<endl;
      cout<<endl<<"++++RIGHT CAMERA NOT CALIBRATED YET++++"<<endl<<endl; 
      cout<<"========================================================================="<<endl;
      rectangle(final_mat, Point(0,0), Point(1280,480), Scalar(0,0,0),-1,8, 0);
      line(final_mat, Point(315,235), Point(35,180), Scalar(255,0,0), 2, 8, 0); //0TL
      line(final_mat, Point(315,15), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6TL
      line(final_mat, Point(35,180), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6L
      line(final_mat, Point(315,235), Point(315,15), Scalar(255,0,0), 2, 8, 0); //0
      line(final_mat, Point(585,175), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6R
      line(final_mat, Point(315,235), Point(585,175), Scalar(255,0,0), 2, 8, 0); //0TR
      line(final_mat, Point(315,15), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6TR
     line(final_mat, Point(315,90), Point(90,90), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(140,200), Point(210,25), Scalar(255,0,0), 2, 8, 0); //3
      line(final_mat, Point(485,195), Point(415,25), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(315,90), Point(530,90), Scalar(255,0,0), 2, 8, 0);  //3

     imshow("FINAL", final_mat);
      points = 0;
      return;
    }


    if(points ==2){
      points=0;
      rectangle(final_mat, Point(0,0), Point(1280,480), Scalar(0,0,0),-1,8, 0);
      imshow("FINAL", final_mat);  
    }


   points++;
   circle(final_mat, Point(x,y), 3, Scalar(0,0,255), -1, 8, 0);  //3.5ft center
   if(points ==1){
     putText(final_mat, "P1", Point(x-10,y-15), FONT_HERSHEY_COMPLEX_SMALL, .75, Scalar(0,0,255), 1.5, false);
     pt1.x=x/sf;
     pt1.y=y/sf;
   }else if(points ==2){
     putText(final_mat, "P2", Point(x-10,y-15), FONT_HERSHEY_COMPLEX_SMALL, .75, Scalar(0,0,255), 1.5, false);
     pt2.x=x/sf;
     pt2.y=y/sf;
   }


      line(final_mat, Point(315,235), Point(35,180), Scalar(255,0,0), 2, 8, 0); //0TL
      line(final_mat, Point(315,15), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6TL
      line(final_mat, Point(35,180), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6L
      line(final_mat, Point(315,235), Point(315,15), Scalar(255,0,0), 2, 8, 0); //0
      line(final_mat, Point(585,175), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6R
      line(final_mat, Point(315,235), Point(585,175), Scalar(255,0,0), 2, 8, 0); //0TR
      line(final_mat, Point(315,15), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6TR
     line(final_mat, Point(315,90), Point(90,90), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(140,200), Point(210,25), Scalar(255,0,0), 2, 8, 0); //3
      line(final_mat, Point(485,195), Point(415,25), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(315,90), Point(530,90), Scalar(255,0,0), 2, 8, 0);  //3


     imshow("FINAL", final_mat);
    
   }

}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "laser_scan");
  ros::NodeHandle nh;

 ros::Publisher scan_pub = nh.advertise<sensor_msgs::LaserScan>("scan", 1000);
  

  unsigned int num_readings = 180;
  double laser_frequency = 40;
  double ranges[num_readings];
  double intensities[num_readings];
int real_x,real_y,r_val,angle;

  int count = 0;
  ros::Rate r(2.0);
  
 
  
  cv::namedWindow("FINAL");
  //cv::startWindowThread();


    Mat final_mat(Size(640, 240), CV_8UC3) ;

      line(final_mat, Point(315,235), Point(35,180), Scalar(255,0,0), 2, 8, 0); //0TL
      line(final_mat, Point(315,15), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6TL
      line(final_mat, Point(35,180), Point(125,35), Scalar(255,0,0), 2, 8, 0);  //6L
      line(final_mat, Point(315,235), Point(315,15), Scalar(255,0,0), 2, 8, 0); //0
      line(final_mat, Point(585,175), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6R
      line(final_mat, Point(315,235), Point(585,175), Scalar(255,0,0), 2, 8, 0); //0TR
      line(final_mat, Point(315,15), Point(495,35), Scalar(255,0,0), 2, 8, 0);  //6TR
     line(final_mat, Point(315,90), Point(90,90), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(140,200), Point(210,25), Scalar(255,0,0), 2, 8, 0); //3
      line(final_mat, Point(485,195), Point(415,25), Scalar(255,0,0), 2, 8, 0);  //3
      line(final_mat, Point(315,90), Point(530,90), Scalar(255,0,0), 2, 8, 0);  //3

  while(nh.ok()){

    if(points ==2){
    int left_x, left_y, right_x, right_y, x_val, y_val, y_offset, y_incrementor;	
    if(pt1.x < pt2.x){
      left_x = pt1.x;
      left_y = pt1.y;
      right_x = pt2.x;
      right_y = pt2.y;
    }else{
      left_x = pt2.x;
      left_y = pt2.y;
      right_x = pt1.x;
      right_y = pt1.y;
    }

    y_offset = 0;
    int z;                        
    float slope_m; 

    if((right_y != left_y)&&(right_x!=left_x)){
      z = sqrt((right_x-left_x)*(right_x-left_x) + (right_y-left_y)*(right_y-left_y));                        
      slope_m = (float)(right_y - left_y) / (float)(right_x - left_x); 
      y_offset = 0;
      y_incrementor = (right_y-left_y) / abs(right_y-left_y);
    }else if(right_y == left_y){
      z = abs(right_x - left_x);                        
    }else if(right_x == left_x){
      z = abs(right_y - left_y);                        
    }

	for(int i=0; i<z; i++){

		if(right_y == left_y){
			y_val = left_y;
			x_val = left_x + i;   
		}else if(right_x == left_x){
			y_val = left_y+i;
			x_val = left_x;
		}else if(right_y != left_y){
		if(abs(right_y - left_y)>abs(left_x-right_x)){
			y_val = left_y + y_offset;
			x_val = left_x + abs(y_offset/slope_m);
			y_offset += y_incrementor;
		if(x_val>=right_x)
			i=z+1;
		}else{
			x_val = left_x+i;
			y_val = (i*slope_m)+left_y;
		if(x_val>=right_x)
			i=z+1;
		}
	}


	Point pt = Point(x_val,y_val);


	int region_id=0;


if(pt.x <= 62){
 if(pt.y<=24){
    region_id=1;
  }else if(pt.y<=34){
    region_id=7;
  }else if(pt.y<=45){
    region_id=13;
  }else if(pt.y<=58){
    if(pt.x <=55){
      region_id=19;
    }else{
    region_id=20;
    }
  }else if(pt.y<=74){
    if(pt.x <=47){
        region_id=25;
    }else{
    region_id=26;
    }
  }else if(pt.y<=94){
    if(pt.x <=38){
      region_id=31;
    }else if(pt.x<=55){
    region_id=32;
    }else{
      region_id=33;
    }
  }

}else if(pt.x <= 75){
 if(pt.y<=23){
    region_id=1;
  }else if(pt.y<=34){
    if(pt.x <=68){
    region_id=7;
    }else{
      region_id=8;
    }
  }else if(pt.y<=45){
      region_id=14;
  }else if(pt.y<=58){
    if(pt.x <=71){
      region_id=20;
    }else{
      region_id=21;
    }
  }else if(pt.y<=77){
    if(pt.x <=66){
      region_id=26;
    }else{
      region_id=27;
    }
  }else if(pt.y<=100){
    if(pt.x <=38){
      region_id=31;
    }else if(pt.x<=92){
      region_id=33;
    }else{
      region_id=34;
    }
  }
     

}else if(pt.x <= 89){
 if(pt.y<=23){
    region_id=2;
  }else if(pt.y<=33){
    if(pt.x <=83){
      region_id=8;
    }else{
      region_id=9;
    }
  }else if(pt.y<=45){
    if(pt.x <=79){
      region_id=14;
    }else{
      region_id=15;
    }
  }else if(pt.y<=60){
    if(pt.x <=87){
      region_id=21;
    }else{
      region_id=22;
    }
  }else if(pt.y<=78){
    if(pt.x <=82){
      region_id=27;
    }else{
      region_id=28;
    }
  }else if(pt.y<=103){
      region_id=34;
  }


}else if(pt.x <= 104){
if(pt.y<=21){
     region_id=3;
  }else if(pt.y<=33){
    if(pt.x <=98){
     region_id=9;
    }else{
     region_id=10;
    }
  }else if(pt.y<=45){
    if(pt.x <=94){
    region_id=15;
    }else{
      region_id=16;
    }
  }else if(pt.y<=60){
    if(pt.x <=88){
      region_id=21;
    }else{
      region_id=22;
    }
  }else if(pt.y<=79){
    if(pt.x <=101){
      region_id=28;
    }else{
      region_id=29;
    }
  }else if(pt.y<=103){
    if(pt.x <=96){
      region_id=34;
    }else{
      region_id=35;
    }
  }
 
}else if(pt.x <= 119){
  if(pt.y<=21){
      region_id=4;
  }else if(pt.y<=32){
    if(pt.x <=115){
      region_id=10;
    }else{
      region_id=11;
    }
  }else if(pt.y<=45){
    if(pt.x <=112){
      region_id=16;
    }else{
      region_id=17;
    }
  }else if(pt.y<=60){
    if(pt.x <=107){
      region_id=22;
    }else{
      region_id=23;
    }
  }else if(pt.y<=82){
      region_id=29;
  }else if(pt.y<=108){
    if(pt.x <=96){
      region_id=34;
    }else{
      region_id=34;
    }

  } 


}else if(pt.x <= 137){
if(pt.y<=20){
      region_id=5;
  }else if(pt.y<=31){
    if(pt.x <=135){
      region_id=11;
    }else{
      region_id=12;
    }
  }else if(pt.y<=45){
    if(pt.x <=133){
      region_id=17;
    }else{
      region_id=18;
    }
  }else if(pt.y<=63){
    if(pt.x <=130){
      region_id=23;
    }else{
      region_id=24;
    }
  }else if(pt.y<=83){
    if(pt.x <=128){
      region_id=29;
    }else{
      region_id=30;
    }
  }else if(pt.y<=112){
    if(pt.x <=124){
      region_id=35;
    }else{
      region_id=36;
    }
  }
  }else if(pt.x <= 157){
    if(pt.y<=17){
      region_id=6;
    }else if(pt.y<=30){
      region_id=12;
    }else if(pt.y<=45){
      region_id=18;
    }else if(pt.y<=64){
      region_id=24;
    }else if(pt.y<=84){
      region_id=30;
    }else if(pt.y<=117){
      region_id=36;    
    }
  }


  switch(region_id){

    case(1):
      real_x = ((((74-pt.x)/16.0)*12.0)+60);
      real_y = ((((23-pt.y)/8.0)*12.0)+60);
      break;
    case(2):
      real_x = ((((89-pt.x)/16.0)*12.0)+48);
      real_y = ((((23-pt.y)/8.0)*12.0)+60);
      break;
    case(3):
      real_x = ((((103-pt.x)/16.0)*12.0)+36);
      real_y = ((((22-pt.y)/9.0)*12.0)+60);
      break;
    case(4):
      real_x = ((((118-pt.x)/16.0)*12.0)+24);
      real_y = ((((20-pt.y)/10.0)*12.0)+60);
      break;
    case(5):
      real_x = ((((137-pt.x)/18.0)*12.0)+12);
      real_y = ((((21-pt.y)/10.0)*12.0)+60);
      break;
     case(6):
      real_x = ((((157-pt.x)/20.0)*12.0)+0);
      real_y = ((((18-pt.y)/11.0)*12.0)+60);
      break;

    case(7):
      real_x = ((((71-pt.x)/9.0)*12.0)+60);
      real_y = ((((34-pt.y)/10.0)*12.0)+48);
      break;
    case(8):
      real_x = ((((85-pt.x)/19.0)*12.0)+48);
      real_y = ((((34-pt.y)/9.0)*12.0)+48);
      break;
    case(9):
      real_x = ((((101-pt.x)/19.0)*12.0)+36);
      real_y = ((((33-pt.y)/11.0)*12.0)+48);
      break;
    case(10):
      real_x = ((((117-pt.x)/19.0)*12.0)+24);
      real_y = ((((33-pt.y)/12.0)*12.0)+48);
      break;
    case(11):
      real_x = ((((135-pt.x)/20.0)*12.0)+12);
      real_y = ((((31-pt.y)/13.0)*12.0)+48);
      break;
    case(12):
      real_x = ((((157-pt.x)/22.0)*12.0)+0);
      real_y = ((((31-pt.y)/13.0)*12.0)+48);
      break;

    case(13):
      real_x = ((((65-pt.x)/19.0)*12.0)+60);
      real_y = ((((45-pt.y)/9.0)*12.0)+36);
      break;
    case(14):
      real_x = ((((80-pt.x)/20.0)*12.0)+48);
      real_y = ((((45-pt.y)/10.0)*12.0)+36);
      break;
    case(15):
      real_x = ((((96-pt.x)/21.0)*12.0)+36);
      real_y = ((((45-pt.y)/11.0)*12.0)+36);
      break;
    case(16):
      real_x = ((((113-pt.x)/20.0)*12.0)+24);
      real_y = ((((45-pt.y)/13.0)*12.0)+36);
      break;
   case(17):
      real_x = ((((134-pt.x)/23.0)*12.0)+12);
      real_y = ((((45-pt.y)/14.0)*12.0)+36);
      break;
   case(18):
      real_x = ((((157-pt.x)/25.0)*12.0)+0);
      real_y = ((((45-pt.y)/15.0)*12.0)+36);
      break;

   case(19):
      real_x = ((((59-pt.x)/20.0)*12.0)+60);
      real_y = ((((58-pt.y)/13.0)*12.0)+24);
      break;
   case(20):
      real_x = ((((74-pt.x)/22.0)*12.0)+48);
      real_y = ((((60-pt.y)/15.0)*12.0)+24);
      break;
   case(21):
      real_x = ((((92-pt.x)/24.0)*12.0)+36);
      real_y = ((((61-pt.y)/16.0)*12.0)+24);
      break;  
   case(22):
      real_x = ((((110-pt.x)/24.0)*12.0)+24);
      real_y = ((((62-pt.y)/17.0)*12.0)+24);
      break;  
   case(23):
      real_x = ((((132-pt.x)/26.0)*12.0)+12);
      real_y = ((((64-pt.y)/19.0)*12.0)+24);
      break;  
   case(24):
      real_x = ((((157-pt.x)/28.0)*12.0)+0);
      real_y = ((((66-pt.y)/21.0)*12.0)+24);
      break;

   case(25):
      real_x = ((((51-pt.x)/22.0)*12.0)+60);
      real_y = ((((58-pt.y)/17.0)*12.0)+12);
      break;  
    case(26):
      real_x = ((((68-pt.x)/25.0)*12.0)+48);
      real_y = ((((60-pt.y)/18.0)*12.0)+12);
      break;
   case(27):
      real_x = ((((85-pt.x)/25.0)*12.0)+36);
      real_y = ((((77-pt.y)/18.0)*12.0)+12);
      break;  
   case(28):
      real_x = ((((105-pt.x)/25.0)*12.0)+24);
      real_y = ((((80-pt.y)/20.0)*12.0)+12);
      break;  
   case(29):
      real_x = ((((129-pt.x)/29.0)*12.0)+12);
      real_y = ((((83-pt.y)/21.0)*12.0)+12);
      break;  
   case(30):
      real_x = ((((157-pt.x)/29.0)*12.0)+0);
      real_y = ((((86-pt.y)/23.0)*12.0)+12);
      break;          
   
   case(31):
      real_x = ((((42-pt.x)/24.0)*12.0)+60);
      real_y = ((((92-pt.y)/19.0)*12.0)+0);
      break;  
   case(32):
      real_x = ((((59-pt.x)/26.0)*12.0)+48);
      real_y = ((((96-pt.y)/21.0)*12.0)+0);
      break;
   case(33):
      real_x = ((((79-pt.x)/30.0)*12.0)+36);
      real_y = ((((100-pt.y)/24.0)*12.0)+0);
      break;  
   case(34):
      real_x = ((((105-pt.x)/29.0)*12.0)+24);
      real_y = ((((103-pt.y)/26.0)*12.0)+0);
      break;  
   case(35):
      real_x = ((((126-pt.x)/34.0)*12.0)+12);
      real_y = ((((110-pt.y)/30.0)*12.0)+0);
      break;  
   case(36):
      real_x = ((((157-pt.x)/34.0)*12.0)+0);
      real_y = ((((118-pt.y)/32.0)*12.0)+0);
      break;     
   default:
      cout<<"INVALID REGION"<<endl;     
      break;

  }




/*  if(pt.x/10 <=159)
      cout<<"Point "<<i<<": [-"<< real_x/12 <<"'"<<(real_x%12)<<"\","<< real_y/12 <<"'"<<(real_y%12)<<"\"]"<<endl;
    else
      cout<<"Point "<<i<<": ["<< real_x/12 <<"'"<<(real_x%12)<<"\","<< real_y/12 <<"'"<<(real_y%12)<<"\"]"<<endl;
*/

      r_val = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      ranges[angle] = r_val*0.0254;

  
   line(final_mat, Point(320,240), Point(pt.x*sf,pt.y*sf), Scalar(255,255,255), 1, 8, 0);
     line(final_mat, Point(pt1.x*sf,pt1.y*sf), Point(pt2.x*sf,pt2.y*sf), Scalar(0,0,255), 3, 8, 0);

    if(i==0)
      end = angle;

  }
}

    ros::Time scan_time = ros::Time::now();
    //populate the LaserScan message
    sensor_msgs::LaserScan scan;
    scan.header.stamp = scan_time;
    scan.header.frame_id = "laser_frame";
    scan.angle_min = 0;
    scan.angle_max = 3.14;
    scan.angle_increment = 3.14 / num_readings;
    scan.time_increment = (1 / laser_frequency) / (num_readings);
    scan.range_min = 0.0;
    scan.range_max = 100.0;
    scan.ranges.resize(num_readings);
    scan.intensities.resize(num_readings);
    for(unsigned int i = 90; i < 180; ++i){
      if((ranges[i]==0)&&(i<end))
        ranges[i] = ranges[i-1];
      scan.ranges[i] = ranges[i];
      scan.intensities[i] = 1;
    }
      scan_pub.publish(scan);
    ros::spinOnce();
      empty = true;
    if(waitKey(1)>0){
      cout<<"=========================SCAN DATA========================="<<endl;
      for(int i = 0; i<180; i++){
        if(ranges[i] != 0)
          cout<<"Angle: "<<i<<" | Range: "<<scan.ranges[i]<<endl;
      }
      cout<<"==========================================================="<<endl<<endl;
    }
    imshow("FINAL", final_mat);
    setMouseCallback("FINAL", onMouse, &final_mat);
     
   //waitKey(); 
   for(unsigned int i = 0; i < num_readings; ++i){
      ranges[i] = 0;
    }
    empty = true;


    ros::Rate loop_rate(100);
    loop_rate.sleep();

  }
       cv::destroyWindow("FINAL");
      ros::shutdown;

}
