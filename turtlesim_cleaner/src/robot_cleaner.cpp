#include "ros/ros.h"

//method to move the robot straight
void move(double speed, double distance, bool isForward);


int main(int argc, char **argv)
{
	//initiate new ROS node named "talker"
	ros::init(argc, argv, "robot_cleaner");
	ros::NodeHandle n;


}

/**
 *makes the robot move with a certain linear velocity for
 *certain distain forward or backward
 * */
void move(double speed, double distance, bool isForward){

	//distance = speed * time



}
