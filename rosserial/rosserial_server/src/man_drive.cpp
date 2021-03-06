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
using namespace std;

#define _SWITCH        100 /*START AUTONOMOUS MODE*/

int i = 0;
int command;

// %Tag(CALLBACK)%
void chatterCallback(const sensor_msgs::Joy::ConstPtr& msg){
  /*Middle Xbox-Button Pressed --Kill The Program*/
  if(msg->buttons[8] == 1){
    command = 0;
    ros::shutdown();
  }


  /*Right Trigger Must be pressed for any drive command to be sen --done for safety*/
  if(msg->axes[4] == -1.0){
    /*A-Button --switch to autonomous mode*/
    if(msg->buttons[0] == 1){
      command = _SWITCH;
    /*FORWARD TO THE LEFT: Right Trigger Down -> Left Analog SticK Upper Left Corner*/
    }else if((msg->axes[0] >= 0.7) && (msg->axes[1] >= 0.6)){
      command = 7;
  /*FORWARD TO THE RIGHT: Right Trigger Down -> Left Analog SticK Upper Right Corner*/
    }else if((msg->axes[0] <= -0.7) && (msg->axes[1] >= 0.6)){
      command = 9;   
    /*SPIN LEFT Right Trigger Down -> Left Analog Stick All The way left*/
    }else if(msg->axes[0] == 1.0){
      command = 4;
    /*SPIN LEFT Right Trigger Down -> Left Analog Stick All The way Right*/
    }else if(msg->axes[0] == -1.0){
      i = 0;
      command = 6;
  /*FORWARD: Right Trigger Down -> Right Analog SticK Pushed Up*/
    }else if(msg->axes[3] >= 0.1){
      command = ((msg->axes[3])*15)+70;//+30;
  /*REVERSE: Right Trigger Down -> Right Analog SticK Pushed Down*/
    }else if(msg->axes[3] <= -0.1){
      command = ((msg->axes[3])*15)-70;//+30;
  /*DO NOTHING: Right Trigger Down But No Drive Command Entered --Robot Stay*/
    }else{
     command=0;
    }
  /*DO NOTHING: Right Trigger Not Pressed --Robot Stay*/
  }else{
    command = 0;
  }
}

int main(int argc, char **argv){
  ros::init(argc, argv, "man_drive"); //Initialize our node
  std::string port; //variable for our arduino port --usually /dev/ttyACM0: to see list of available ports use terminal command ls /dev/tty*
  ros::param::param<std::string>("~port", port, "/dev/ttyACM0"); //setting our port
  int baud; //variable for our baud rate --we must set the baud rate so the arduino and the host do not lose sync
  ros::param::param<int>("~baud", baud, 9600); //setting our baud
  boost::asio::io_service io_service; //necessary junk
  new rosserial_server::SerialSession(io_service, port, baud); //initializing our serial communication
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));  //beginning our serial thread

  NodeHandle n;//subscriber node handler
  NodeHandle out; //publisher node handler

  Publisher chatter_pub = out.advertise<std_msgs::Int8>("Direction", 1);  //publish our drive commands
  ros::Subscriber sub = n.subscribe("joy", 1000, chatterCallback); //subscribe to our xbox controller

  Rate loop_rate(100); //loop rate in Hz

  /*Run until program is manually stopped*/
  while (ros::ok()){
    std_msgs::Int8 msg; //sending an 8 bit integer message
    msg.data = command; //populating our message with our command
    if(command!=0) //output command to a terminal window if there is anything to show
      ROS_INFO("PUBLISH:: %d", msg.data);
    if(command == _SWITCH)
      cout<<endl<<"+++++++++++++++SWITCH TO AUTONOMOUS MODE+++++++++++++++"<<endl<<endl;
    chatter_pub.publish(msg); //publish our message
    ros::spinOnce(); //and when that loop is over we'll do it all over again!
    loop_rate.sleep();
  }
  return 0;
}
