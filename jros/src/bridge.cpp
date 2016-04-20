#include "ros/ros.h"
#include "global_pose.pb.h"
#include "global_pose.pb.cc"
#include <earl/GlobalPose.h>
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv) 
{	

	ros::init(argc, argv, "bridge");

	ros::NodeHandle n;

	earl::GlobalPose global_pose_msg;

	ros::Publisher global_pose_pub = n.advertise<earl::GlobalPose>("global_pose", 1000);

	ros::Rate loop_rate(100);

	GlobalPose global_pose_data;

	int count = 0;

	while (ros::ok()) 
	{
		
		std::fstream input("global_pose", std::ios::in | std::ios::binary);

		if (global_pose_data.ParseFromIstream(&input)) 
		{

			global_pose_msg.latitude = global_pose_data.latitude();
			global_pose_msg.longitude = global_pose_data.longitude();
			global_pose_msg.altitude = global_pose_data.altitude();
			global_pose_msg.roll = global_pose_data.roll();
			global_pose_msg.pitch = global_pose_data.pitch();
			global_pose_msg.yaw = global_pose_data.yaw();

			global_pose_pub.publish(global_pose_msg);

			ros::spinOnce();

			loop_rate.sleep();
			++count;
		}

		else {

			std::cout << "Could not find data!" << std::endl;

			ros::spinOnce();

			loop_rate.sleep();
			++count;
		}
	}

	return 0;
}
