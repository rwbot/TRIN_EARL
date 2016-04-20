#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <math.h>
#include "std_msgs/Int16MultiArray.h"
#include <stdio.h>

using namespace std;

#define PI 3.14159265

void arrayCallbackJ1(const std_msgs::Int16MultiArray::ConstPtr& array){
  cout<<"ENTER"<<endl;
  int size = (array->data[0])*4;
  cout<<"Size: "<<size<<endl;
  int pointCount = 1;

  int left_x, left_y, right_x, right_y, x_val, y_val, y_offset, y_incrementor;	
  for(int i = 1; i< size; i+4){
    if(array->data[i] < array->data[i+2]){
      left_x = array->data[i+0];
      left_y = array->data[i+1];
      right_x = array->data[i+2];
      right_y = array->data[i+3];                           
    }else{
      left_x = array->data[i+2];
      left_y = array->data[i+3];
      right_x = array->data[i+0];
      right_y = array->data[i+1];
    }

    int z = abs(right_y - left_y);                        
    int lineArray[z];
    float slope_m = (float)(right_y - left_y) / (float)(right_x - left_x); 

    y_offset = 0;
    y_incrementor = (right_y-left_y) / abs(right_y-left_y);
    for(int i = 0; i <= abs(right_y-left_y); i++){
      y_val = left_y + y_offset;
      x_val = left_x + abs(y_offset/slope_m);
      y_offset += y_incrementor;
    }
 
    cout<<"J1Point "<<pointCount<<": ["<<left_x<<","<<left_y<<"]"<<endl;
    cout<<"J1Point "<<pointCount<<": ["<<right_x<<","<<right_y<<"]"<<endl;
    pointCount+=2;
  }

  cout<<"====================================================="<<endl;

}

void arrayCallbackJ2(const std_msgs::Int16MultiArray::ConstPtr& array){
  int size = (array->data[0])*4;
  cout<<"Size: "<<size<<endl;
  int pointCount = 1;

  int left_x, left_y, right_x, right_y, x_val, y_val, y_offset, y_incrementor;	
  for(int i = 1; i< size; i+4){
    if(array->data[i] < array->data[i+2]){
      left_x = array->data[i+0];
      left_y = array->data[i+1];
      right_x = array->data[i+2];
      right_y = array->data[i+3];                           
    }else{
      left_x = array->data[i+2];
      left_y = array->data[i+3];
      right_x = array->data[i+0];
      right_y = array->data[i+1];
    }

    int z = abs(right_y - left_y);                        
    int lineArray[z];
    float slope_m = (float)(right_y - left_y) / (float)(right_x - left_x); 

    y_offset = 0;
    y_incrementor = (right_y-left_y) / abs(right_y-left_y);
    for(int i = 0; i <= abs(right_y-left_y); i++){
      y_val = left_y + y_offset;
      x_val = left_x + abs(y_offset/slope_m);
      y_offset += y_incrementor;
    }
 
    cout<<"J2Point "<<pointCount<<": ["<<left_x<<","<<left_y<<"]"<<endl;
    cout<<"J2Point "<<pointCount<<": ["<<right_x<<","<<right_y<<"]"<<endl;
  }

  cout<<"====================================================="<<endl;

}




int main(int argc, char** argv){
  ros::init(argc, argv, "laser_scan_publisher");
  cout<<"START"<<endl;

  ros::NodeHandle n;
  ros::NodeHandle nh_array;
  ros::Subscriber sub_arrayJ1 = nh_array.subscribe("PointsJ1", 1, arrayCallbackJ1);
  ros::Subscriber sub_arrayJ2 = nh_array.subscribe("PointsJ2", 1, arrayCallbackJ2);
  ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 1000);
  

  unsigned int num_readings = 180;
  double laser_frequency = 40;
  double ranges[num_readings];
  double intensities[num_readings];

  int count = 0;
  ros::Rate r(2.0);
  
  ranges[0] = 1;
  while(n.ok()){
    //generate some fake data for our laser scan
    for(unsigned int i = 1; i < num_readings/2; ++i){
     
      ranges[i] = ranges[0]/cos((i*PI)/180);
      intensities[i] = 0;
    }



    for(unsigned int i = num_readings/2; i < num_readings; ++i){
     
      ranges[i] = -(ranges[0]/cos((i*PI)/180));
      intensities[i] = 0;
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
    for(unsigned int i = 0; i < num_readings; ++i){
      scan.ranges[i] = ranges[i];
      scan.intensities[i] = intensities[i];
    }

    scan_pub.publish(scan);
    ++count;
    ros::spinOnce();
    
  
    //cv::destroyWindow("J2_sub");
    ros::Rate loop_rate(1000);

    loop_rate.sleep();
  }
}
