#!/usr/bin/env python

import rospy
from earl.msg import GlobalPose

def callback(data):
	rospy.loginfo(rospy.get_caller_id() + "I heard %d", data.latitude)

def listener():

	rospy.init_node('listener')

	rospy.Subscriber('global_pose', GlobalPose, callback)

	rospy.spin()

if __name__ == '__main__':
	listener()
  