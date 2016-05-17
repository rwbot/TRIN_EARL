#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_listener.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include <sstream>
#include <termios.h>
#include <ros/ros.h>
#include "rosserial_server/serial_session.h"
#include <opencv2/imgproc/imgproc.hpp>      //Converting ROS to OpenCV images
#include "opencv2/core/core.hpp"            //OpenCV Stitching
#include "opencv2/features2d/features2d.hpp"//OpenCV Stitching
#include "opencv2/highgui/highgui.hpp"      //OpenCV Stitching 
#include <string>

using namespace cv;

float vR;
float vL;
float wheel_base = 75;
float wheel_radius = 33.02;

void wheelR_cb(const std_msgs::Int16& msg){
  vR = (1.0*(msg.data))/100.0;
}

void wheelL_cb(const std_msgs::Int16& msg){
  vL = (1.0*(msg.data))/100.0;  
  std::ostringstream ss;
  ss<<vL<<" m/s";
  std::string output(ss.str());
  Mat odom_left(Size(640, 640), CV_8UC3);
  rectangle(odom_left, Point(0,0), Point(640,640), Scalar(0,0,0), -1, 8, 0);
  putText(odom_left, output, Point(320,320), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,0,255), 1.5, false);
  imshow("OdomL", odom_left);

}

int main(int argc, char** argv){
  namedWindow("OdomL");
  ros::init(argc, argv, "odometry_publisher");
  std::string port; //variable for our arduino port --usually /dev/ttyACM0: to see list of available ports use terminal command ls /dev/tty*
  ros::param::param<std::string>("~port", port, "/dev/ttyACM1"); //setting our port
  int baud; //variable for our baud rate --we must set the baud rate so the arduino and the host do not lose sync
  ros::param::param<int>("~baud", baud, 4800); //setting our baud
  boost::asio::io_service io_service; //necessary junk
  new rosserial_server::SerialSession(io_service, port, baud); //initializing our serial communication
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));  //beginning our serial thread


  ros::NodeHandle n, node, ard;
  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);
  tf::TransformBroadcaster odom_broadcaster;

  ros::Subscriber subV1 = ard.subscribe("ard_odomR", 10, wheelR_cb); //subscribe to our xbox controller
  ros::Subscriber subV2 = ard.subscribe("ard_odomL", 10, wheelL_cb);

  tf::TransformListener listener;
  tf::StampedTransform tfTransform;
  tf::Vector3 origin;
  tf::Vector3 axis;
  tf::Quaternion rotation;

  static ros::Publisher publisher = 
    node.advertise<geometry_msgs::TransformStamped>("/odom2map_transform",
                            10);

  geometry_msgs::TransformStamped geoTransform;

  int seq = 0;
  geoTransform.header.frame_id = "map";
  geoTransform.child_frame_id = "odom";


  double x = 0.0;
  double y = 0.0;
  double th = 0.0;

  double vx = 0;//0.1;
  double vy = 0;
  double vth = 0;//0.1;

  ros::Time current_time, last_time;
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  ros::Rate r(100);
  while(ros::ok()){
    current_time = ros::Time::now();

    // Changing scanmatcher_frame to odom after geoTransform
  /*  try { 
      listener.waitForTransform(geoTransform.header.frame_id,"/odom", ros::Time(0), ros::Duration(1.0));
      listener.lookupTransform(geoTransform.header.frame_id,
                   "/odom",       
                   ros::Time(0), 
                   tfTransform);
    }
    catch(tf::TransformException &exception) { 
      ROS_ERROR("%s", exception.what());
    }*/

    origin = tfTransform.getOrigin();
    rotation = tfTransform.getRotation();
    axis = rotation.getAxis();

    geoTransform.header.seq = seq;
    geoTransform.header.stamp = tfTransform.stamp_;

    geoTransform.transform.translation.x = origin.x();
    geoTransform.transform.translation.y = origin.y();
    geoTransform.transform.translation.z = origin.z();

    geoTransform.transform.rotation.x = axis.x();
    geoTransform.transform.rotation.y = axis.y();
    geoTransform.transform.rotation.z = axis.z();
    geoTransform.transform.rotation.w = rotation.getW();

    seq++;
    publisher.publish(geoTransform);

    

    //compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
  //  double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
  //  double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
  //  double delta_th = vth * dt;

    

    double delta_th = ((wheel_radius)/(wheel_base))*(1.0*vR - 1.0*vL);
    th += delta_th;

    double delta_x = ((wheel_radius)/2.0)*(1.0*vR+1.0*vL)*(cos(th));
    double delta_y = ((wheel_radius)/2.0)*(1.0*vR+1.0*vL)*(sin(th));

    x += delta_x;
    y += delta_y;

    //since all odometry is 6DOF we'll need a quaternion created from yaw
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

    //first, we'll publish the transform over tf
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;//current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_link";

    odom_trans.transform.translation.x = x;
    odom_trans.transform.translation.y = y;
    odom_trans.transform.translation.z = 0.0;
    odom_trans.transform.rotation = odom_quat;

    //send the transform
    odom_broadcaster.sendTransform(odom_trans);

    //next, we'll publish the odometry message over ROS
    nav_msgs::Odometry odom;
    odom.header.stamp = current_time;
    odom.header.frame_id = "odom";

    //set the position
    odom.pose.pose.position.x = x;
    odom.pose.pose.position.y = y;
    odom.pose.pose.position.z = 0.0;
    odom.pose.pose.orientation = odom_quat;

    //set the velocity
    odom.child_frame_id = "base_link";
    odom.twist.twist.linear.x = vx;
    odom.twist.twist.linear.y = vy;
    odom.twist.twist.angular.z = vth;

    //publish the message
    odom_pub.publish(odom);
    ros::spinOnce();               // check for incoming messages
    //std::cout<<odom<<std::endl;

    r.sleep();
    last_time = current_time;

     }
}
