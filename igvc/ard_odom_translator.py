#!/usr/bin/env python

# Disable and enable autonomous control by redirecting the odometry

import rospy
from std_msgs.msg import Int16
from std_msgs.msg import Float32


effort_to_speed_factor = 0.0141
effort_to_count_factor = 118.2752


def left_callback(msg, left_pub):
    left_speed = translate(msg.data)
    left_pub.publish(left_pub)

def right_callback(msg, right_pub):
    right_speed = translate(msg.data)
    right_pub.publish(right_pub)

# translates count per second received from arduino to speed
def translate(cps):
    speed = cps * (effort_to_speed_factor/effort_to_count_factor)

def main():
    global autonomous 
    rospy.init_node('ard_odom_tranlsator')
    
    left_pub = rospy.Publisher('lwheel', Float32, queue_size=10)
    right_pub = rospy.Publisher('rwheel', Float32, queue_size=10)

    rospy.Subscriber('ard_cpsR', Int16, left_callback, left_pub)
    rospy.Subscriber('ard_cpsL', Int16, odom_callback, right_pub)
    
    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():

        rospy.spin()
        rate.sleep()


if __name__ == '__main__':
    main() 
