#!/usr/bin/python

# Program to be used in conjunction with gps_common to 
# generate path to odom

# Publishes longitude and latitude 

import rospy
import sys
from gps_common import GPSFix
from gps_common import GPSStatus

def main():
	longitude = rospy.get_param('longitude', 0)
	latitude = rospy.get_param('latitude', 0)
	pub = rospy.Publisher('goal_fix', GPSFix, queue_size=10)
	rospy.init_node('publish_goal_fix')
	r = rospy.Rate(10)
	while not rospy.is_shutdown():
		msg = GPSFix()
		status = GPSStatus()
		msg.status = 
		msg.time = rospy.Time.now()
		msg.latitude = latitude
		msg.longitude = longitude
		pub.publish(msg)	

if __name__ == "__main__":
	main()