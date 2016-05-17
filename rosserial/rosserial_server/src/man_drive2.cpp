#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include <sstream>
#include <termios.h>

#include <sensor_msgs/Joy.h>

#include <ros/ros.h>

#include "rosserial_server/serial_session.h"


using namespace ros;

int i = 0;
int c;

int speed;

// %Tag(CALLBACK)%
void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg)
{
 /* if(msg->axes[4] == -1.0){
    i = 4;
    ROS_INFO("Right Trigger Pressed [RT]: Axis %d", i);
      if(msg->axes[0] >= 0.1) &&( msg->axes[0] >= 0.99)){
        i = 0;
        ROS_INFO("Left Analog Stick Right [L-Stick]: Axis %d", i);
        c = 6;
        speed = ((msg->axes[0])*60)+145;//+30;
      }else if(msg->axes[0] <= -0.1){
        i = 0;
        ROS_INFO("Left Analog Stick Left [L-Stick]: Axis %d", i);
        c = 4;
        speed = ((msg->axes[1])*60)+145;//+30;
      }else if(msg->axes[3] >= 0.1){
        i = 3;
          ROS_INFO("Right Analog Stick: Axis %f", msg->axes[3]);
          speed = ((msg->axes[3])*60)+145;//+30;
      }
  }else{
    c = 0;
  }
*/

/*Button Commands*/
  /*A-Button*/
  if(msg->buttons[0] == 1){
    i = 0;
    ROS_INFO("A-Button Pressed Down [Green]: Button %d", i);
    c = 0;
  }
  /*B-Button*/
  if(msg->buttons[1] == 1){
    i = 1;
    ROS_INFO("B-Button Pressed Down [RED]: Button %d", i);
    c = 0;
  }
  /*X-Button*/
  if(msg->buttons[2] == 1){
    i = 2;
    ROS_INFO("X-Button Pressed Down [Blue]: Button %d", i);
   c = 0;
  }
  /*Y-Button*/
  if(msg->buttons[3] == 1){
    i = 3;
    ROS_INFO("Y-Button Pressed Down [Yellow]: Button %d", i);
   c = 0;
  }

}

int main(int argc, char **argv)
{


  ros::init(argc, argv, "man_drive");
  std::string port;
  ros::param::param<std::string>("~port", port, "/dev/ttyACM0");
  int baud;
  ros::param::param<int>("~baud", baud, 4800);


  boost::asio::io_service io_service;
  new rosserial_server::SerialSession(io_service, port, baud);
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));

  //init(argc, argv, "c_listener");
  NodeHandle n;//subscriber
  NodeHandle out; //publisher

  Publisher chatter_pub = out.advertise<std_msgs::Int8>("Direction", 1000);
  Publisher chatter_pub2 = out.advertise<std_msgs::Int8>("Speed", 1000);

  ros::Subscriber sub = n.subscribe("joy", 1000, chatterCallback);
  
  Rate loop_rate(100);


  while (ros::ok())
  {

    std_msgs::Int8 msg, msg2;
    msg.data = c;//ss.str();
    msg2.data = speed;

    //ROS_INFO("%s", msg.data.c_str());

    chatter_pub.publish(msg);
    chatter_pub2.publish(msg2);
    ros::spinOnce();

    loop_rate.sleep();



}

  return 0;
}
// %EndTag(FULLTEXT)%

