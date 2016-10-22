#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

int main(int argc, char** argv) {
	ros::init(argc, argv, "signal_speed");

	ros::NodeHandle n;
	ros::Subscriber sub = n.subscriber("joy", 1000, joyCallback);

	Rate loop_rate(10);

	while (ros::ok()) {

		
	}
}