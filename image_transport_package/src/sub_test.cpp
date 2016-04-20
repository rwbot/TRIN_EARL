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
#include "opencv2/calib3d/calib3d.hpp"      //OpenCV Stitching
#include "opencv2/imgproc/imgproc.hpp"      //OpenCV Stitching
#include "opencv2/stitching/stitcher.hpp"
#include "std_msgs/Int16MultiArray.h"

using namespace cv;
using namespace std;
  bool try_use_gpu = false;
  bool captured1 = false;
  bool captured2 = false;
  sensor_msgs::Image cam1, cam2;
  cv_bridge::CvImagePtr ptrJ1, ptrJ2;
  Mat jetson1, jetson2;
  sensor_msgs::ImagePtr msg;




void imageCallbackJ1(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {

    //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);

    jetson1 = cv_bridge::toCvCopy(msg, "bgr8")->image;
    //resize(jetson1, jetson1, Size(), 5, 5);
    
    captured1 = true;
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void imageCallbackJ2(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    //cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    jetson2 = cv_bridge::toCvCopy(msg, "bgr8")->image;
    //resize(jetson2, jetson2, Size(), 5, 5);
    captured2 = true;

  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}


int main(int argc, char **argv)
{
		cout << "START"<<endl;
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  
cv::namedWindow("FINAL");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);

  //Mat final_mat = imread("raw.png", 1);

  
  Mat final_mat(Size(640, 240), CV_8UC3) ;
  final_mat=Scalar(0,0,0);


  
  image_transport::Subscriber sub_j1 = it.subscribe("videoJ1", 1, imageCallbackJ1);
  image_transport::Subscriber sub_j2 = it.subscribe("videoJ2", 1, imageCallbackJ2);


vector<Mat> imgs;

  //while(ros::ok){

   // if(captured1 && captured2 ){

      //cv::imshow("viewJ1", jetson1);
      //cv::imshow("viewJ2", jetson2);

      //Mat final_mat(Size(jetson2.cols + jetson1.cols, jetson2.rows),CV_8UC3);

      //Mat roi1(final_mat, Rect(0,0, jetson1.cols, jetson1.rows));
      //Mat roi2(final_mat, Rect(jetson1.cols,0, jetson2.cols, jetson2.rows));


      //jetson2.copyTo(roi2);
      //jetson1.copyTo(roi1);

      //line(final_mat, Point(0,119), Point(639,119), Scalar(0,0,255), 2, 8, 0); //center line accross
      //line(final_mat, Point(320,0), Point(320,239), Scalar(0,0,255), 2, 8, 0); //center line down


                         /************ACROSS LEFT**********/
      line(final_mat, Point(315,235), Point(35,180), Scalar(255,255,255), 1, 8, 0); //0
      line(final_mat, Point(315,202), Point(45,162), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,172), Point(55,145), Scalar(255,255,255), 1, 8, 0); //1
      line(final_mat, Point(315,150), Point(68,130), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(315,130), Point(80,115), Scalar(255,255,255), 1, 8, 0); //2
      line(final_mat, Point(315,110), Point(85,103), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(315,90), Point(90,90), Scalar(255,255,255), 1, 8, 0);  //3
      line(final_mat, Point(315,75), Point(98,80), Scalar(255,255,255), 1, 8, 0);  //3.5
      line(final_mat, Point(315,60), Point(105,70), Scalar(255,255,255), 1, 8, 0);  //4
      line(final_mat, Point(315,47), Point(110,60), Scalar(255,255,255), 1, 8, 0);  //4.5
      line(final_mat, Point(315,35), Point(115,50), Scalar(255,255,255), 1, 8, 0);  //5
      line(final_mat, Point(315,25), Point(120,43), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(315,15), Point(125,35), Scalar(255,255,255), 1, 8, 0);  //6


                          /************VERTICAL LEFT**********/

      line(final_mat, Point(35,180), Point(125,35), Scalar(255,255,255), 1, 8, 0);  //6
      line(final_mat, Point(50,183), Point(137,35), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(65,185), Point(150,35), Scalar(255,255,255), 1, 8, 0);  //5
      line(final_mat, Point(82,187), Point(165,32), Scalar(255,255,255), 1, 8, 0);  //4/5
      line(final_mat, Point(100,190), Point(180,30), Scalar(255,255,255), 1, 8, 0); //4
      line(final_mat, Point(120,195), Point(195,27), Scalar(255,255,255), 1, 8, 0); //3.5
      line(final_mat, Point(140,200), Point(210,25), Scalar(255,255,255), 1, 8, 0); //3
      line(final_mat, Point(160,205), Point(225,25), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(185,210), Point(240,25), Scalar(255,255,255), 1, 8, 0); //2
      line(final_mat, Point(215,215), Point(260,20), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(245,225), Point(275,20), Scalar(255,255,255), 1, 8, 0); //1
      line(final_mat, Point(275,230), Point(292,15), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,235), Point(315,15), Scalar(255,255,255), 1, 8, 0); //0


                          /************VERTICAL RIGHT**********/
      line(final_mat, Point(350,228), Point(327,18), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(390,220), Point(345,20), Scalar(255,255,255), 1, 8, 0);  //1
      line(final_mat, Point(415,213), Point(365,23), Scalar(255,255,255), 1, 8, 0);  //1.5
      line(final_mat, Point(440,205), Point(385,25), Scalar(255,255,255), 1, 8, 0);  //2
      line(final_mat, Point(462,200), Point(400,25), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(485,195), Point(415,25), Scalar(255,255,255), 1, 8, 0);  //3
      line(final_mat, Point(505,190), Point(427,28), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(525,185), Point(440,30), Scalar(255,255,255), 1, 8, 0);  //4
      line(final_mat, Point(540,185), Point(455,33), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(555,182), Point(470,35), Scalar(255,255,255), 1, 8, 0);  //5      
      line(final_mat, Point(570,178), Point(482,35), Scalar(255,255,255), 1, 8, 0);  //0.5
      line(final_mat, Point(585,175), Point(495,35), Scalar(255,255,255), 1, 8, 0);  //6

                         /************ACROSS RIGHT**********/
      line(final_mat, Point(315,235), Point(585,175), Scalar(255,255,255), 1, 8, 0); //0
      line(final_mat, Point(315,202), Point(572,155), Scalar(255,255,255), 1, 8, 0); //0.5
      line(final_mat, Point(315,172), Point(560,135), Scalar(255,255,255), 1, 8, 0); //1
      line(final_mat, Point(315,150), Point(552,125), Scalar(255,255,255), 1, 8, 0); //1.5
      line(final_mat, Point(315,130), Point(545,115), Scalar(255,255,255), 1, 8, 0); //2
      line(final_mat, Point(315,110), Point(537,103), Scalar(255,255,255), 1, 8, 0); //2.5
      line(final_mat, Point(315,90), Point(530,90), Scalar(255,255,255), 1, 8, 0);  //3
      line(final_mat, Point(315,75), Point(522,80), Scalar(255,255,255), 1, 8, 0);  //3.5
      line(final_mat, Point(315,60), Point(515,70), Scalar(255,255,255), 1, 8, 0);  //4
      line(final_mat, Point(315,47), Point(510,60), Scalar(255,255,255), 1, 8, 0);  //4.5
      line(final_mat, Point(315,35), Point(505,50), Scalar(255,255,255), 1, 8, 0);  //5
      line(final_mat, Point(315,25), Point(500,43), Scalar(255,255,255), 1, 8, 0);  //5.5
      line(final_mat, Point(315,15), Point(495,35), Scalar(255,255,255), 1, 8, 0);  //6



  resize(final_mat, final_mat, Size(), .5, .5);



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


  


      resize(final_mat, final_mat, Size(), 6, 6);

      imshow("FINAL", final_mat);

    
waitKey();
    //}
    //if(waitKey())
       //break;
    ros::spinOnce();
    
  
    //cv::destroyWindow("J2_sub");
    ros::Rate loop_rate(1000);

    loop_rate.sleep();


   //}
  ros::shutdown;
 cv::destroyWindow("viewJ1");
  cv::destroyWindow("viewJ2");


}
