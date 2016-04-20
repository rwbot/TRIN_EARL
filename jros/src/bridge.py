#! /usr/bin/python

import global_pose_pb2
import sys
import rospy
from earl.msg import *

def talker():
	pub = rospy.Publisher('global_pose', global_pose, queue_size = 100)
	rospy.init_node('bridge')
	rate = rospy.Rate(10)
	
	global_pose_data = global_pose_pb2.GlobalPose()

	while not rospy.is_shutdown():

		f = open("global_pose.txt")

		global_pose_data.ParseFromString(f.read())

		global_pose_msg.latitude = global_pose_data.latitude
		global_pose_msg.longitude = global_pose_data.longitude
		global_pose_msg.altitude = global_pose_data.altitude
		global_pose_msg.roll = global_pose_data.roll
		global_pose_msg.pitch = global_pose_data.pitch
		global_pose_msg.yaw = global_pose_data.yaw

		f.close()

		rospy.loginfo(global_pose_msg)
		pub.publish(global_pose_msg)
		rate.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass








