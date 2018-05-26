#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/PointCloud.h"
#include "sensor_msgs/Image.h"
#include "opencv2/imgproc.hpp"

#include <sstream>

ros::Publisher line_pub;

void calculate_lane_positions(const sensor_msgs::Image::ConstrPtr& msg){



  line_pub.publish()

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "barrel_removal");

  ros::NodeHandle n;

  line_pub = n.advertise<sensor_msgs::PointCloud>("line", 1000);
  ros::Subscriber image_sub = n.subscribe("image", 1000, calculate_lane_positions);

  ros::spin();

  return 0;
}