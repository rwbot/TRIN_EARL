#!/usr/bin/env python

import rospy
from math import sqrt
from geometry_msgs.msg import Twist
from std_msgs.msg import String

def callmove(move):
	print('Move '+ move.data),

def callvel(vel):
        x = vel.linear.x
        z = vel.linear.z
        acc = sqrt( (x*x) + (z*z) )
        print('@ ' + str(acc) + 'm/s')

def translate():
        rospy.init_node('translate', anonymous=True)
        rospy.Subscriber('cmd_vel', Twist, callvel)
        rospy.Subscriber('move_command', String, callmove)
	rospy.spin()

if __name__ == '__main__':
    translate()

