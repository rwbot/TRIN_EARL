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

using namespace cv;
using namespace std;


void onMouse( int event, int x, int y, int, void* ){
    if( event != CV_EVENT_LBUTTONDOWN )
            return;

    Point pt = Point(x,y);
    cout<<"========================================================================="<<endl;
    if(pt.x/4 <=159)
      cout<<"Left Camera"<<endl<<"Coordinates: ["<<pt.x/4<<","<<pt.y/4<<"]"<<endl;
    else
      cout<<"Right Camera"<<endl<<"Coordinates: ["<<(pt.x/4)<<","<<pt.y/4<<"]"<<endl<<endl;

int region_id=0;


if(pt.x/4 <= 62){
 if(pt.y/4<=24){
    cout<<"REGION 1L"<<endl; 
    region_id=1;
  }else if(pt.y/4<=34){
    cout<<"REGION 7L"<<endl;
    region_id=7;
  }else if(pt.y/4<=45){
    cout<<"REGION 13L"<<endl;
    region_id=13;
  }else if(pt.y/4<=58){
    if(pt.x/4 <=55){
      cout<<"REGION 19L"<<endl;
      region_id=19;
    }else{
      cout<<"REGION 20L"<<endl;
    region_id=20;
    }
  }else if(pt.y/4<=74){
    if(pt.x/4 <=47){
      cout<<"REGION 25L"<<endl;
        region_id=25;
    }else{
      cout<<"REGION 26L"<<endl;
    region_id=26;
    }
  }else if(pt.y/4<=94){
    if(pt.x/4 <=38){
      cout<<"REGION 31L"<<endl;
      region_id=31;
    }else if(pt.x/4<=55){
      cout<<"REGION 32L"<<endl;
    region_id=32;
    }else{
      cout<<"REGION 33L"<<endl;
      region_id=33;
    }
  }

}else if(pt.x/4 <= 75){
 if(pt.y/4<=23){
    cout<<"REGION 1L"<<endl;
    region_id=1;
  }else if(pt.y/4<=34){
    if(pt.x/4 <=68){
      cout<<"REGION 7L"<<endl;
    region_id=7;
    }else{
      cout<<"REGION 8L"<<endl;
      region_id=8;
    }
  }else if(pt.y/4<=45){
    cout<<"REGION 14L"<<endl;
      region_id=14;
  }else if(pt.y/4<=58){
    if(pt.x/4 <=71){
      cout<<"REGION 20L"<<endl;
      region_id=20;
    }else{
      cout<<"REGION 21L"<<endl;
      region_id=21;
    }
  }else if(pt.y/4<=77){
    if(pt.x/4 <=66){
      cout<<"REGION 26L"<<endl;
      region_id=26;
    }else{
      cout<<"REGION 27L"<<endl;
      region_id=27;
    }
  }else if(pt.y/4<=100){
    if(pt.x <=38){
      cout<<"REGION 31L"<<endl;
      region_id=31;
    }else if(pt.x/4<=92){
      cout<<"REGION 33L"<<endl;
      region_id=33;
    }else{
      cout<<"REGION 34L"<<endl;
      region_id=34;
    }
  }
     

}else if(pt.x/4 <= 89){
 if(pt.y/4<=23){
    cout<<"REGION 2L"<<endl;
    region_id=2;
  }else if(pt.y/4<=33){
    if(pt.x/4 <=83){
      cout<<"REGION 8L"<<endl;
      region_id=8;
    }else{
      cout<<"REGION 9L"<<endl;
      region_id=9;
    }
  }else if(pt.y/4<=45){
    if(pt.x/4 <=79){
      cout<<"REGION 14L"<<endl;
      region_id=14;
    }else{
      cout<<"REGION 15L"<<endl;
      region_id=15;
    }
  }else if(pt.y/4<=60){
    if(pt.x/4 <=87){
      cout<<"REGION 21L"<<endl;
      region_id=21;
    }else{
      cout<<"REGION 22L"<<endl;
      region_id=22;
    }
  }else if(pt.y/4<=78){
    if(pt.x/4 <=82){
      cout<<"REGION 27L"<<endl;
      region_id=27;
    }else{
      cout<<"REGION 28L"<<endl;
      region_id=28;
    }
  }else if(pt.y/4<=103){
      cout<<"REGION 34L"<<endl;
      region_id=34;
  }


}else if(pt.x/4 <= 104){
if(pt.y/4<=21){
    cout<<"REGION 3L"<<endl;
     region_id=3;
  }else if(pt.y/4<=33){
    if(pt.x/4 <=98){
      cout<<"REGION 9L"<<endl;
     region_id=9;
    }else{
      cout<<"REGION 10L"<<endl;
     region_id=10;
    }
  }else if(pt.y/4<=45){
    if(pt.x/4 <=94){
      cout<<"REGION 15L"<<endl;
    region_id=15;
    }else{
      cout<<"REGION 16L"<<endl;
      region_id=16;
    }
  }else if(pt.y/4<=60){
    if(pt.x/4 <=88){
      cout<<"REGION 21L"<<endl;
      region_id=21;
    }else{
      cout<<"REGION 22L"<<endl;
      region_id=22;
    }
  }else if(pt.y/4<=79){
    if(pt.x/4 <=101){
      cout<<"REGION 28L"<<endl;
      region_id=28;
    }else{
      cout<<"REGION 29L"<<endl;
      region_id=29;
    }
  }else if(pt.y/4<=103){
    if(pt.x/4 <=96){
      cout<<"REGION 34L"<<endl;
      region_id=34;
    }else{
      cout<<"REGION 35L"<<endl;
      region_id=35;
    }
  }
 
}else if(pt.x/4 <= 119){
  if(pt.y/4<=21){
    cout<<"REGION 4L"<<endl;
      region_id=4;
  }else if(pt.y/4<=32){
    if(pt.x/4 <=115){
      cout<<"REGION 10L"<<endl;
      region_id=10;
    }else{
      cout<<"REGION 11L"<<endl;
      region_id=11;
    }
  }else if(pt.y/4<=45){
    if(pt.x/4 <=112){
      cout<<"REGION 16L"<<endl;
      region_id=16;
    }else{
      cout<<"REGION 17L"<<endl;
      region_id=17;
    }
  }else if(pt.y/4<=60){
    if(pt.x/4 <=107){
      cout<<"REGION 22L"<<endl;
      region_id=22;
    }else{
      cout<<"REGION 23L"<<endl;
      region_id=23;
    }
  }else if(pt.y/4<=82){
      cout<<"REGION 29L"<<endl;
      region_id=29;
  }else if(pt.y/4<=108){
    if(pt.x/4 <=96){
      cout<<"REGION 34L"<<endl;
      region_id=34;
    }else{
      cout<<"REGION 35L"<<endl;
      region_id=34;
    }

  }


}else if(pt.x/4 <= 137){
if(pt.y/4<=20){
    cout<<"REGION 5L"<<endl;
      region_id=5;
  }else if(pt.y/4<=31){
    if(pt.x/4 <=135){
      cout<<"REGION 11L"<<endl;
      region_id=11;
    }else{
      cout<<"REGION 12L"<<endl;
      region_id=12;
    }
  }else if(pt.y/4<=45){
    if(pt.x/4 <=133){
      cout<<"REGION 17L"<<endl;
      region_id=17;
    }else{
      cout<<"REGION 18L"<<endl;
      region_id=18;
    }
  }else if(pt.y/4<=63){
    if(pt.x/4 <=130){
      cout<<"REGION 23L"<<endl;
      region_id=23;
    }else{
      cout<<"REGION 24L"<<endl;
      region_id=24;
    }
  }else if(pt.y/4<=83){
    if(pt.x/4 <=128){
      cout<<"REGION 29L"<<endl;
      region_id=29;
    }else{
      cout<<"REGION 30L"<<endl;
      region_id=30;
    }
  }else if(pt.y/4<=112){
    if(pt.x/4 <=124){
      cout<<"REGION 35L"<<endl;
      region_id=35;
    }else{
      cout<<"REGION 36L"<<endl;
      region_id=36;
    }
  }

  }else if(pt.x/4 <= 157){
    if(pt.y/4<=17){
      cout<<"REGION 6L"<<endl;
      region_id=6;
    }else if(pt.y/4<=30){
      cout<<"REGION 12L"<<endl;
      region_id=12;
    }else if(pt.y/4<=45){
      cout<<"REGION 18L"<<endl;
      region_id=18;
    }else if(pt.y/4<=64){
      cout<<"REGION 24L"<<endl;
      region_id=24;
    }else if(pt.y/4<=84){
      cout<<"REGION 30L"<<endl;
      region_id=30;
    }else if(pt.y/4<=117){
      cout<<"REGION 36L"<<endl;
      region_id=36;    
    }

  }else if(pt.x/4 <= 172){
    if(pt.y/4<=18){
      cout<<"REGION 37R"<<endl;
      region_id=37;
    }else if(pt.y/4<=30){
      cout<<"REGION 43R"<<endl;
      region_id=43;
    }else if(pt.y/4<=45){
      cout<<"REGION 49RL"<<endl;
      region_id=49;
    }else if(pt.y/4<=65){
      cout<<"REGION 55R"<<endl;
      region_id=55;
    }else if(pt.y/4<=84){
      cout<<"REGION 61R"<<endl;
      region_id=61;
    }else if(pt.y/4<=117){
      cout<<"REGION 67R"<<endl;
      region_id=67;    
    }


  }else if(pt.x/4 <= 192){
    if(pt.y/4<=20){
      cout<<"REGION 38R"<<endl;
      region_id=38;
    }else if(pt.y/4<=31){
      if(pt.x/4 <=176){
        cout<<"REGION 43R"<<endl;
        region_id=43;
      }else{
        cout<<"REGION 44R"<<endl;
        region_id=44;
      }
    }else if(pt.y/4<=45){
      if(pt.x/4 <=178){
        cout<<"REGION 49R"<<endl;
        region_id=49;
      }else{
        cout<<"REGION 50R"<<endl;
        region_id=50;
      }
    }else if(pt.y/4<=64){
      if(pt.x/4 <=182){
        cout<<"REGION 55R"<<endl;
        region_id=55;
      }else{
        cout<<"REGION 56R"<<endl;
        region_id=56;
      }
    }else if(pt.y/4<=83){
      if(pt.x/4 <=186){
        cout<<"REGION 61R"<<endl;
        region_id=61;
      }else{
        cout<<"REGION 62R"<<endl;
        region_id=62;
      }
    }else if(pt.y/4<=113){
      if(pt.x/4 <=190){
        cout<<"REGION 67R"<<endl;
        region_id=67;
      }else{
        cout<<"REGION 68R"<<endl;
        region_id=68;
      }
    }

  }else if(pt.x/4 <= 208){
    if(pt.y/4<=20){
      cout<<"REGION 39R"<<endl;
      region_id=39;
    }else if(pt.y/4<=32){
      if(pt.x/4 <=196){
        cout<<"REGION 44R"<<endl;
        region_id=44;
      }else{
        cout<<"REGION 45R"<<endl;
        region_id=45;
      }
    }else if(pt.y/4<=45){
      if(pt.x/4 <=200){
        cout<<"REGION 50R"<<endl;
        region_id=50;
      }else{
        cout<<"REGION 51R"<<endl;
        region_id=51;
      }
    }else if(pt.y/4<=63){
      if(pt.x/4 <=204){
        cout<<"REGION 56R"<<endl;
        region_id=56;
      }else{
        cout<<"REGION 57R"<<endl;
        region_id=57;
      }
    }else if(pt.y/4<=80){
      cout<<"REGION 62R"<<endl;
      region_id=62;
    }else if(pt.y/4<=109){
      if(pt.x/4 <=193){
        cout<<"REGION 67R"<<endl;
        region_id=67;
      }else{
        cout<<"REGION 68R"<<endl;
        region_id=68;
      }
    }


  }else if(pt.x/4 <= 221){
    if(pt.y/4<=22){
      if(pt.x/4 <= 210){
        cout<<"REGION 39R"<<endl;
        region_id=39;
      }else{
        cout<<"REGION 40R"<<endl;
        region_id=40;
      }
    }else if(pt.y/4<=32){
      if(pt.x/4 <=213){
        cout<<"REGION 45R"<<endl;
        region_id=45;
      }else{
        cout<<"REGION 46R"<<endl;
        region_id=46;
      }
    }else if(pt.y/4<=45){
      if(pt.x/4 <=218){
        cout<<"REGION 51R"<<endl;
        region_id=51;
      }else{
        cout<<"REGION 52R"<<endl;
        region_id=52;
      }
    }else if(pt.y/4<=62){
      cout<<"REGION 57R"<<endl;
       region_id=57;
    }else if(pt.y/4<=78){
      if(pt.x/4<=210){
        cout<<"REGION 62R"<<endl;
        region_id=62;  
      }else{
        cout<<"REGION 63R"<<endl;
        region_id=63;
      }
    }else if(pt.y/4<=106){
      if(pt.x/4 <=215){
        cout<<"REGION 68R"<<endl;
        region_id=68;
      }else{
        cout<<"REGION 69R"<<endl;
        region_id=69;
      }
    }

  }else if(pt.x/4 <= 236){
    if(pt.y/4<=23){
      if(pt.x/4 <= 223){
        cout<<"REGION 40R"<<endl;
        region_id=40;
      }else{
        cout<<"REGION 41R"<<endl;
        region_id=41;
      }
    }else if(pt.y/4<=34){
      if(pt.x/4 <=228){
        cout<<"REGION 46R"<<endl;
        region_id=46;
      }else{
        cout<<"REGION 47R"<<endl;
        region_id=47;
      }
    }else if(pt.y/4<=45){
      if(pt.x/4 <=232){
        cout<<"REGION 52R"<<endl;
        region_id=52;
      }else{
        cout<<"REGION 53R"<<endl;
        region_id=53;
      }
    }else if(pt.y/4<=61){
      if(pt.x/4<=224){
        cout<<"REGION 57R"<<endl;
        region_id=57;
      }else{
        cout<<"REGION 58R"<<endl;
        region_id=58;
     }
    }else if(pt.y/4<=76){
      if(pt.x/4<=234){
        cout<<"REGION 63R"<<endl;
        region_id=63;  
      }else{
        cout<<"REGION 64R"<<endl;
        region_id=64;
      }
    }else if(pt.y/4<=103){
      if(pt.x/4 <=237){
        cout<<"REGION 69R"<<endl;
        region_id=69;
      }else{
        cout<<"REGION 70R"<<endl;
        region_id=70;
      }
    }

  }else if(pt.x/4 <= 247){
    if(pt.y/4<=24){
      if(pt.x/4 <= 238){
        cout<<"REGION 41R"<<endl;
        region_id=41;
      }else{
        cout<<"REGION 42R"<<endl;
        region_id=42;
      }
    }else if(pt.y/4<=35){
      if(pt.x/4 <=241){
        cout<<"REGION 47R"<<endl;
        region_id=47;
      }else{
        cout<<"REGION 48R"<<endl;
        region_id=48;
      }
    }else if(pt.y/4<=45){
      cout<<"REGION 53R"<<endl;
       region_id=53;
    }else if(pt.y/4<=60){
      if(pt.x/4<=240){
        cout<<"REGION 58R"<<endl;
        region_id=58;
      }else{
        cout<<"REGION 59R"<<endl;
        region_id=59;
     }
    }else if(pt.y/4<=74){
       cout<<"REGION 64R"<<endl;
       region_id=64;
    }else if(pt.y/4<=99){
      cout<<"REGION 70R"<<endl;
       region_id=70;
    }

  }else{
    if(pt.y/4<=24){
      cout<<"REGION 42R"<<endl;
      region_id=42;
    }else if(pt.y/4<=36){
      cout<<"REGION 48R"<<endl;
      region_id=48;
    }else if(pt.y/4<=45){
      if(pt.x/4<=249){
        cout<<"REGION 53R"<<endl;
        region_id=53;
      }else{
        cout<<"REGION 54R"<<endl;
       region_id=54;
      }
    }else if(pt.y/4<=58){
      if(pt.x/4<=254){
        cout<<"REGION 59R"<<endl;
        region_id=59;
      }else{
        cout<<"REGION 60R"<<endl;
        region_id=60;
     }
    }else if(pt.y/4<=72){
      if(pt.x/4<=250){
        cout<<"REGION 64R"<<endl;
        region_id=54;
      }else if(pt.x/4<=262){
        cout<<"REGION 65R"<<endl;
        region_id=65;
      }else{
       cout<<"REGION 66R"<<endl;
       region_id=66;
     }
    }else if(pt.y/4<=99){
      if(pt.x/4<=256){
        cout<<"REGION 70R"<<endl;
        region_id=70;
      }else if(pt.x/4<=270){
        cout<<"REGION 71R"<<endl;
        region_id=71;
      }else{
       cout<<"REGION 72R"<<endl;
       region_id=72;
      }
    
  }



}






int real_x,real_y,r,angle;
  switch(region_id){

    case(1):
      real_x = ((((74-pt.x/4)/16.0)*12.0)+60);
      real_y = ((((23-pt.y/4)/8.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(2):
      real_x = ((((89-pt.x/4)/16.0)*12.0)+48);
      real_y = ((((23-pt.y/4)/8.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(3):
      real_x = ((((103-pt.x/4)/16.0)*12.0)+36);
      real_y = ((((22-pt.y/4)/9.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
     angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(4):
      real_x = ((((118-pt.x/4)/16.0)*12.0)+24);
      real_y = ((((20-pt.y/4)/10.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(5):
      real_x = ((((137-pt.x/4)/18.0)*12.0)+12);
      real_y = ((((21-pt.y/4)/10.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
     case(6):
      real_x = ((((157-pt.x/4)/20.0)*12.0)+0);
      real_y = ((((18-pt.y/4)/11.0)*12.0)+60);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;

    case(7):
      real_x = ((((71-pt.x/4)/9.0)*12.0)+60);
      real_y = ((((34-pt.y/4)/10.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(8):
      real_x = ((((85-pt.x/4)/19.0)*12.0)+48);
      real_y = ((((34-pt.y/4)/9.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(9):
      real_x = ((((101-pt.x/4)/19.0)*12.0)+36);
      real_y = ((((33-pt.y/4)/11.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(10):
      real_x = ((((117-pt.x/4)/19.0)*12.0)+24);
      real_y = ((((33-pt.y/4)/12.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(11):
      real_x = ((((135-pt.x/4)/20.0)*12.0)+12);
      real_y = ((((31-pt.y/4)/13.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(12):
      real_x = ((((157-pt.x/4)/22.0)*12.0)+0);
      real_y = ((((31-pt.y/4)/13.0)*12.0)+48);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;

    case(13):
      real_x = ((((65-pt.x/4)/19.0)*12.0)+60);
      real_y = ((((45-pt.y/4)/9.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(14):
      real_x = ((((80-pt.x/4)/20.0)*12.0)+48);
      real_y = ((((45-pt.y/4)/10.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(15):
      real_x = ((((96-pt.x/4)/21.0)*12.0)+36);
      real_y = ((((45-pt.y/4)/11.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
    case(16):
      real_x = ((((113-pt.x/4)/20.0)*12.0)+24);
      real_y = ((((45-pt.y/4)/13.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(17):
      real_x = ((((134-pt.x/4)/23.0)*12.0)+12);
      real_y = ((((45-pt.y/4)/14.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(18):
      real_x = ((((157-pt.x/4)/25.0)*12.0)+0);
      real_y = ((((45-pt.y/4)/15.0)*12.0)+36);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;

   case(19):
      real_x = ((((59-pt.x/4)/20.0)*12.0)+60);
      real_y = ((((58-pt.y/4)/13.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(20):
      real_x = ((((74-pt.x/4)/22.0)*12.0)+48);
      real_y = ((((60-pt.y/4)/15.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(21):
      real_x = ((((92-pt.x/4)/24.0)*12.0)+36);
      real_y = ((((61-pt.y/4)/16.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(22):
      real_x = ((((110-pt.x/4)/24.0)*12.0)+24);
      real_y = ((((62-pt.y/4)/17.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(23):
      real_x = ((((132-pt.x/4)/26.0)*12.0)+12);
      real_y = ((((64-pt.y/4)/19.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(24):
      real_x = ((((157-pt.x/4)/28.0)*12.0)+0);
      real_y = ((((66-pt.y/4)/21.0)*12.0)+24);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;

   case(25):
      real_x = ((((51-pt.x/4)/22.0)*12.0)+60);
      real_y = ((((58-pt.y/4)/17.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
    case(26):
      real_x = ((((68-pt.x/4)/25.0)*12.0)+48);
      real_y = ((((60-pt.y/4)/18.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(27):
      real_x = ((((85-pt.x/4)/25.0)*12.0)+36);
      real_y = ((((77-pt.y/4)/18.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(28):
      real_x = ((((105-pt.x/4)/25.0)*12.0)+24);
      real_y = ((((80-pt.y/4)/20.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(29):
      real_x = ((((129-pt.x/4)/29.0)*12.0)+12);
      real_y = ((((83-pt.y/4)/21.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(30):
      real_x = ((((157-pt.x/4)/29.0)*12.0)+0);
      real_y = ((((86-pt.y/4)/23.0)*12.0)+12);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;          
   
   case(31):
      real_x = ((((42-pt.x/4)/24.0)*12.0)+60);
      real_y = ((((92-pt.y/4)/19.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(32):
      real_x = ((((59-pt.x/4)/26.0)*12.0)+48);
      real_y = ((((96-pt.y/4)/21.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;
   case(33):
      real_x = ((((79-pt.x/4)/30.0)*12.0)+36);
      real_y = ((((100-pt.y/4)/24.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(34):
      real_x = ((((105-pt.x/4)/29.0)*12.0)+24);
      real_y = ((((103-pt.y/4)/26.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(35):
      real_x = ((((126-pt.x/4)/34.0)*12.0)+12);
      real_y = ((((110-pt.y/4)/30.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
   case(36):
      real_x = ((((157-pt.x/4)/34.0)*12.0)+0);
      real_y = ((((118-pt.y/4)/32.0)*12.0)+0);
      cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;
      break;  
       /*RIGHT CAMERA*/

        /*RIGHT CAMERA*/

   case(37):
      real_x = ((((pt.x/4-157)/16.0)*12.0)+0);
      real_y = ((((19-pt.y/4)/11.0)*12.0)+60);
      break;  
   case(38):
      real_x = ((((pt.x/4-173)/18.0)*12.0)+12);
      real_y = ((((20-pt.y/4)/11.0)*12.0)+60);
      break;
   case(39):
      real_x = ((((pt.x/4-193)/16.0)*12.0)+24);
      real_y = ((((21-pt.y/4)/9.0)*12.0)+60);
      break;  
   case(40):
      real_x = ((((pt.x/4-209)/13.0)*12.0)+36);
      real_y = ((((22-pt.y/4)/9.0)*12.0)+60);
      break;  
   case(41):
      real_x = ((((pt.x/4-222)/13.0)*12.0)+48);
      real_y = ((((24-pt.y/4)/9.0)*12.0)+60);
      break;  
   case(42):
      real_x = ((((pt.x/4-237)/34.0)*12.0)+60);
      real_y = ((((24-pt.y/4)/8.0)*12.0)+60);
      break; 

   case(43):
      real_x = ((((pt.x/4-175)/18.0)*12.0)+0);
      real_y = ((((31-pt.y/4)/12.0)*12.0)+48);
      break;  
   case(44):
      real_x = ((((pt.x/4-197)/22.0)*12.0)+12);
      real_y = ((((32-pt.y/4)/12.0)*12.0)+48);
      break;
   case(45):
      real_x = ((((pt.x/4-213)/16.0)*12.0)+24);
      real_y = ((((33-pt.y/4)/12.0)*12.0)+48);
      break;  
   case(46):
      real_x = ((((pt.x/4-227)/14.0)*12.0)+36);
      real_y = ((((34-pt.y/4)/12.0)*12.0)+48);
      break;  
   case(47):
      real_x = ((((pt.x/4-241)/14.0)*12.0)+48);
      real_y = ((((34-pt.y/4)/10.0)*12.0)+48);
      break;  
   case(48):
      real_x = ((((pt.x/4-255)/14.0)*12.0)+60);
      real_y = ((((34-pt.y/4)/10.0)*12.0)+48);
      break; 


   case(49):
      real_x = ((((pt.x/4-178)/21.0)*12.0)+0);
      real_y = ((((45-pt.y/4)/14.0)*12.0)+36);
      break;  
   case(50):
      real_x = ((((pt.x/4-200)/22.0)*12.0)+12);
      real_y = ((((45-pt.y/4)/13.0)*12.0)+36);
      break;
   case(51):
      real_x = ((((pt.x/4-218)/18.0)*12.0)+24);
      real_y = ((((45-pt.y/4)/12.0)*12.0)+36);
      break;  
   case(52):
      real_x = ((((pt.x/4-233)/15.0)*12.0)+36);
      real_y = ((((45-pt.y/4)/11.0)*12.0)+36);
      break;  
   case(53):
      real_x = ((((pt.x/4-248)/15.0)*12.0)+48);
      real_y = ((((45-pt.y/4)/11.0)*12.0)+36);
      break;  
   case(54):
      real_x = ((((pt.x/4-261)/13.0)*12.0)+60);
      real_y = ((((45-pt.y/4)/10.0)*12.0)+36);
      break; 


   case(55):
      real_x = ((((pt.x/4-182)/25.0)*12.0)+0);
      real_y = ((((64-pt.y/4)/18.0)*12.0)+24);
      break;  
   case(56):
      real_x = ((((pt.x/4-205)/23.0)*12.0)+12);
      real_y = ((((62-pt.y/4)/17.0)*12.0)+24);
      break;
   case(57):
      real_x = ((((pt.x/4-223)/18.0)*12.0)+24);
      real_y = ((((60-pt.y/4)/15.0)*12.0)+24);
      break;  
   case(58):
      real_x = ((((pt.x/4-241)/18.0)*12.0)+36);
      real_y = ((((59-pt.y/4)/14.0)*12.0)+24);
      break;  
   case(59):
      real_x = ((((pt.x/4-254)/13.0)*12.0)+48);
      real_y = ((((58-pt.y/4)/13.0)*12.0)+24);
      break;  
   case(60):
      real_x = ((((pt.x/4-269)/15.0)*12.0)+60);
      real_y = ((((57-pt.y/4)/12.0)*12.0)+24);
      break; 


   case(61):
      real_x = ((((pt.x/4-186)/29.0)*12.0)+0);
      real_y = ((((83-pt.y/4)/19.0)*12.0)+12);
      break;  
   case(62):
      real_x = ((((pt.x/4-210)/24.0)*12.0)+12);
      real_y = ((((62-pt.y/4)/18.0)*12.0)+12);
      break;
   case(63):
      real_x = ((((pt.x/4-230)/20.0)*12.0)+24);
      real_y = ((((76-pt.y/4)/16.0)*12.0)+12);
      break;  
   case(64):
      real_x = ((((pt.x/4-248)/18.0)*12.0)+36);
      real_y = ((((73-pt.y/4)/14.0)*12.0)+12);
      break;  
   case(65):
      real_x = ((((pt.x/4-262)/14.0)*12.0)+48);
      real_y = ((((71-pt.y/4)/13.0)*12.0)+12);
      break;  
   case(66):
      real_x = ((((pt.x/4-276)/15.0)*12.0)+60);
      real_y = ((((69-pt.y/4)/12.0)*12.0)+12);
      break; 


   case(67):
      real_x = ((((pt.x/4-191)/34.0)*12.0)+0);
      real_y = ((((113-pt.y/4)/30.0)*12.0)+0);
      break;  
   case(68):
      real_x = ((((pt.x/4-216)/25.0)*12.0)+12);
      real_y = ((((107-pt.y/4)/27.0)*12.0)+0);
      break;
   case(69):
      real_x = ((((pt.x/4-237)/21.0)*12.0)+24);
      real_y = ((((101-pt.y/4)/25.0)*12.0)+0);
      break;  
   case(70):
      real_x = ((((pt.x/4-256)/19.0)*12.0)+36);
      real_y = ((((96-pt.y/4)/23.0)*12.0)+0);
      break;  
   case(71):
      real_x = ((((pt.x/4-271)/15.0)*12.0)+48);
      real_y = ((((93-pt.y/4)/22.0)*12.0)+0);
      break;  
   case(72):
      real_x = ((((pt.x/4-286)/15.0)*12.0)+60);
      real_y = ((((89-pt.y/4)/20.0)*12.0)+0);
      break; 

   default:
      cout<<"INVALID REGION"<<endl;     
      break;

  }

   cout<<"Real World Coordinates: "<< real_x/12 <<"\""<<(real_x%12)<<"' Left | "<< real_y/12 <<"\""<<(real_y%12)<<"' Vertical"<<endl;
      r = sqrt(1.0*(real_x*real_x)+(1.0*(real_y+11)*(1.0*real_y+11)));
      angle =  180 - atan(1.0*(real_y+11)/(real_x))*180.0/3.14159;
      cout<<"Distance From Robot: "<<r<<" inches | Angle of Scan: "<<angle<<" degees"<<endl<<endl;


    cout<<"========================================================================="<<endl;


 }

int main(int argc, char **argv)
{
  ros::init(argc, argv, "cam_calib");
  ros::NodeHandle nh;
  
  cv::namedWindow("FINAL");
  cv::startWindowThread();

  
  //Mat final_mat = imread("raw.png", 1);
  Mat final_mat(Size(640, 240), CV_8UC3) ;
  final_mat=Scalar(0,0,0);


                         /************ACROSS LEFT**********/
      line(final_mat, Point(315,235), Point(35,180), Scalar(0,0,255), 2, 8, 0); //0
      line(final_mat, Point(315,202), Point(45,162), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,172), Point(55,145), Scalar(0,0,255), 2, 8, 0); //1
      line(final_mat, Point(315,150), Point(68,130), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(315,130), Point(80,115), Scalar(0,0,255), 2, 8, 0); //2
      line(final_mat, Point(315,110), Point(85,103), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(315,90), Point(90,90), Scalar(0,0,255), 2, 8, 0);  //3
      line(final_mat, Point(315,75), Point(98,80), Scalar(255,255,255), 1, 8, 0);  //3.5
      line(final_mat, Point(315,60), Point(105,70), Scalar(0,0,255), 2, 8, 0);  //4
      line(final_mat, Point(315,47), Point(110,60), Scalar(255,255,255), 1, 8, 0);  //4.5
      line(final_mat, Point(315,35), Point(115,50), Scalar(0,0,255), 2, 8, 0);  //5
      line(final_mat, Point(315,25), Point(120,43), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(315,15), Point(125,35), Scalar(0,0,255), 2, 8, 0);  //6


                          /************VERTICAL LEFT**********/

      line(final_mat, Point(35,180), Point(125,35), Scalar(0,0,255), 2, 8, 0);  //6
      line(final_mat, Point(50,183), Point(137,35), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(65,185), Point(150,35), Scalar(0,0,255), 2, 8, 0);  //5
      line(final_mat, Point(82,187), Point(165,32), Scalar(255,255,255), 1, 8, 0);  //4/5
      line(final_mat, Point(100,190), Point(180,30), Scalar(0,0,255), 2, 8, 0); //4
      line(final_mat, Point(120,195), Point(195,27), Scalar(255,255,255), 1, 8, 0); //3.5
      line(final_mat, Point(140,200), Point(210,25), Scalar(0,0,255), 2, 8, 0); //3
      line(final_mat, Point(160,205), Point(225,25), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(185,210), Point(240,25), Scalar(0,0,255), 2, 8, 0); //2
      line(final_mat, Point(215,215), Point(260,20), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(245,225), Point(275,20), Scalar(0,0,255), 2, 8, 0); //1
      line(final_mat, Point(275,230), Point(292,15), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,235), Point(315,15), Scalar(0,0,255), 2, 8, 0); //0


                          /************VERTICAL RIGHT**********/
      line(final_mat, Point(350,228), Point(327,18), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(390,220), Point(345,20), Scalar(0,0,255), 2, 8, 0);  //1
      line(final_mat, Point(415,213), Point(365,23), Scalar(255,255,255), 1, 8, 0);  //1.5
      line(final_mat, Point(440,205), Point(385,25), Scalar(0,0,255), 2, 8, 0);  //2
      line(final_mat, Point(462,200), Point(400,25), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(485,195), Point(415,25), Scalar(0,0,255), 2, 8, 0);  //3
      line(final_mat, Point(505,190), Point(427,28), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(525,185), Point(440,30), Scalar(0,0,255), 2, 8, 0);  //4
      line(final_mat, Point(540,185), Point(455,33), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(555,182), Point(470,35), Scalar(0,0,255), 2, 8, 0);  //5      
      line(final_mat, Point(570,178), Point(482,35), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(585,175), Point(495,35), Scalar(0,0,255), 2, 8, 0);  //6



                         /************ACROSS RIGHT**********/
      line(final_mat, Point(315,235), Point(585,175), Scalar(0,0,255), 2, 8, 0); //0
      line(final_mat, Point(315,202), Point(572,155), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,172), Point(560,135), Scalar(0,0,255), 2, 8, 0); //1
      line(final_mat, Point(315,150), Point(552,125), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(315,130), Point(545,115), Scalar(0,0,255), 2, 8, 0); //2
      line(final_mat, Point(315,110), Point(537,103), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(315,90), Point(530,90), Scalar(0,0,255), 2, 8, 0);  //3
      line(final_mat, Point(315,75), Point(522,80), Scalar(255,255,255), 1, 8, 0);  //3.5
      line(final_mat, Point(315,60), Point(515,70), Scalar(0,0,255), 2, 8, 0);  //4
      line(final_mat, Point(315,47), Point(510,60), Scalar(255,255,255), 1, 8, 0);  //4.5
      line(final_mat, Point(315,35), Point(505,50), Scalar(0,0,255), 2, 8, 0);  //5
      line(final_mat, Point(315,25), Point(500,43), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(315,15), Point(495,35), Scalar(0,0,255), 2, 8, 0);  //6


      



  resize(final_mat, final_mat, Size(), .5, .5);


/*
   int x_val = 15;
    int y_val = 5;
     for(int i = 1; i<140; i++){
       x_val+=2;
       y_val = 5;
       //cout<<"X 1st loop: "<<x_val<<endl;
       for(int j = 1; j<58; j++){
         final_mat.at<Vec3b>(Point(x_val, y_val))[0] = 255;
         y_val+=2;
       //cout<<"X 2nd loop: "<<x_val<<endl;
       }

     }


*/     
   
 



     resize(final_mat, final_mat, Size(), 4, 4);

      imshow("FINAL", final_mat);
  setMouseCallback("FINAL", onMouse, 0);

    
waitKey();
  cv::destroyWindow("FINAL");
 ros::shutdown;

}
