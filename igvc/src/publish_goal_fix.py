#!/usr/bin/python

# Program to be used in conjunction with gps_common to 
# generate path to odom

# Publishes longitude and latitude 

import rospy
import sys
from geographic_msgs.msg import WayPoint
from std_srvs.srv import Trigger

queue = []

def next_waypoint(req):
	queue.pop(0)

def main():
	global queue

	rospy.init_node('publish_goal_fix')

	with open('config/gps_waypoints.txt') as f:
		latitude = float(f.readlines())
		longitude = float(f.readlines())
		queue.push((latitude, longitude))

	pub = rospy.Publisher('goal_fix', WayPoint, queue_size=10)
	
	s  = rospy.Service('next_waypoint_srv', Trigger, next_waypoint)

	r = rospy.Rate(10)
	while not rospy.is_shutdown():
		msg = WayPoint()
		current_waypoint = queue[0]
		msg.latitude = current_waypoint.latitude
		msg.longitude = current_waypoint.longitude
		msg.altitude = 0
		pub.publish(msg)	
		rospy.spin()

if __name__ == "__main__":
	main()