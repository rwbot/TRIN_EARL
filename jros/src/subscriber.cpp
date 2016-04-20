#include "ros/ros.h"
#include "earl/GlobalPose.h"

void globalPoseCallback(const earl::GlobalPose::ConstPtr &msg) 
{
	std::cout << "Latitude: " << msg->latitude << std::endl;
	std::cout << "Longitude: " << msg->longitude << std::endl;
}

int main(int argc, char **argv) 
{
	ros::init(argc, argv, "listener");

	ros::NodeHandle n;

	ros::Rate loop_rate(100);

	ros::Subscriber sub = n.subscribe("global_pose", 1000, globalPoseCallback);

	ros::spin();

	return 0;
}

