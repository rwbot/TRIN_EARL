#!/usr/bin/env python

import rospy
from std_msgs.msg import Bool

def blink_rate():
    pub = rospy.Publisher('blink', Bool, queue_size=10)
    rospy.init_node('blink_rate', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        hello_str = rospy.get_param("blink", True)
        pub.publish(hello_str)
        rate.sleep()

if __name__ == '__main__':
    try:
        blink_rate()
    except rospy.ROSInterruptException:
        pass