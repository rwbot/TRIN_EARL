#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include <termios.h>
#include <ros/ros.h>
#include "rosserial_server/serial_session.h"

using namespace ros;
using namespace std;

/* Data points measured - motor signal sent vs actual speed of robot
100 - 1 km/h - 0.62 mph - 0.30 mps 
110 - 1.7 km/h - 1.06 mph - 0.47 mps
120 - 2.3 km/h - 1.43 mph - 0.64 mps
130 - 2.9 km/h - 1.80 pmh - 0.80 mps
*/

// constants
float speed_min = 0.5; // in mps
float speed_max = 0.8; 
int voltage_min = 110; 
int voltage_max = 130; 

// store speed in m/s 
float leftWheel;
float rightWheel;

/* Linear mapping from speed to voltage signal to be sent to motors */
int speed_to_voltage(float speed) {
  // if speed out of range
  if(speed > speed_max) {
    speed = speed_max;
  }
  if(speed < speed_min) {
    speed = speed_min;
  }

  // calculate and return voltage
  return int(voltage_min + (((speed - speed_min)/(speed_max - speed_min)) * (voltage_max - voltage_min)));
}

/* Callback to store left speed data to global variable */
void leftCB(const std_msgs::Float32& msg) {
  leftWheel = msg.data;
}

/* Call back to store right speed data to global variable */
void rightCB(const std_msgs::Float32& msg) {
  rightWheel = msg.data; 
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "auto_drive"); // initialize node
  
  std::string port; // variable for arduino port -- usually /dev/ttyACM0: to see list of available ports use terminal command ls /dev/tty*
  ros::param::param<std::string>("~port", port, "/dev/ttyACM0"); // setting arduino port
ea
  int baud; // variable for baud rate --we must set the baud rate so the arduino and the host do not lose sync
  ros::param::param<int>("~baud", baud, 9600); // setting baud rate

  boost::asio::io_service io_service; // necessary junk
  new rosserial_server::SerialSession(io_service, port, baud); // initializing serial communication
  boost::thread(boost::bind(&boost::asio::io_service::run, &io_service)); // beginning serial thread

  // setup publishers
  NodeHandle out; // node handler  
  Publisher chatter_left = out.advertise<std_msgs::Int16>("Left", 1);  // publish left drive commands
  Publisher chatter_right = out.advertise<std_msgs::Int16>("Right", 1);  // publish right drive commands
  Publisher chatter_blink = out.advertise<std_msgs::Bool>("Blink", 1);

  // subscribe to speed (in m/s) topic from twist_to_motors differetial drive node
  ros::Subscriber<std_msgs::Float32> subL("lwheel_vtarget", &leftCB );
  ros::Subscriber<std_msgs::Float32> subR("rwheel_vtarget", &rightCB );

  Rate loop_rate(8); // loop rate in Hz

  // run until program is manually stopped
  while (ros::ok()){
    std_msgs::Int16 left, right; // voltage is an 8 bit integer message
    
    // convert speed data to voltage and populate message
    left.data = speed_to_voltage(leftWheel);
    right.data = speed_to_voltage(rightWheel);
    
    if(leftWheel != 0) // output command to a terminal window if there is anything to show
      ROS_INFO("PUBLISH Left: %d | Right: %d", left.data, right.data);

    // publish messages
    chatter_left.publish(left);
    chatter_right.publish(right);
    chatter_blink.publish(true); //always make light blink

    ros::spinOnce(); //and when that loop is over we'll do it all over again!
    loop_rate.sleep();
  }
  return 0;
}
