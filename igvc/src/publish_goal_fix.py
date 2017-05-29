#!/usr/bin/python

# Program to be used in conjunction with gps_common to 
# generate path to odom

# Publishes longitude and latitude 

import rospy
import sys
from gps_common import GPSFix
from gps_common import GPSStatus
from geographic_msgs.msg import WayPoint

def main():
	longitude = rospy.get_param('longitude', 0)
	latitude = rospy.get_param('latitude', 0)
	pub = rospy.Publisher('goal_fix', GPSFix, queue_size=10)
	rospy.init_node('publish_goal_fix')
	r = rospy.Rate(10)
	while not rospy.is_shutdown():
		msg = WayPoint()
		msg.latitude = latitude
		msg.longitude = longitude
		msg.altitude = 0
		pub.publish(msg)	

if __name__ == "__main__":
	main()