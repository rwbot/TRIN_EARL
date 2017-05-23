#!/usr/bin/env python

# Disable and enable autonomous control by redirecting the odometry

import rospy
from std_msgs.msg import Bool
from nav_msgs.msg import Odometry
from std_srvs.srv import SetBool

is_autonomous = False

def set_autonomous(req):
    is_autonomous = req.data
    rospy.loginfo("is_autonomous is set to %r" % is_autonomous)
    resp = SetBool()
    resp.success = True
    return resp

def odom_callback(msg, odom_pub):
    if is_autonomous:
        odom_pub.publish(msg)

def multiplexer():
    global autonomous 
    rospy.init_node('multiplexer')
    
    pub = rospy.Publisher('is_autonomous', Bool, queue_size=10)
    odom_pub = rospy.Publisher('odom', Odometry, queue_size=10)

    rospy.Subscriber('nav_odom', Odometry, odom_callback, odom_pub)
    
    s = rospy.Service('redirect_odom', SetBool, set_autonomous)

    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        if is_autonomous:
            pub.publish(True)
        else:
            pub.publish(False)

        rospy.spin()
        rate.sleep()


if __name__ == '__main__':
    multiplexer() 
