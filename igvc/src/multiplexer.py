#!/usr/bin/env python

# Utilizes mux to multiplex the cmd_vel on the prompt of some controller
# so that the robot could be controlled manually and autonomously 

import rospy
from std_msgs.msg import Bool
from sensor_msgs.msg import Joy

is_autonomous = True

def control_callback(button):
    global is_autonomous
    if (button[7]):
        is_autonomous = !(is_autonomous)

def blink(cond):
    rospy.wait_for_service('blink')
    try:
       blink = rospy.ServiceProxy('blink', Bool)
       resp = blink(cond)
       return resp.Sucess
    except rospy.ServiceException e:
        print 'Service call failed %s' % e

def multiplexer():
    global autonomous 
    rospy.init_node('multiplexer')
    rospy.Subscriber('joy_start', Joy, control_callback)
    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        try:
           select_mux = rospy.ServiceProxy('mux/select', MuxSelect)
            if is_autonomous:
                select_mux('cmd_vel')
                blink(True)
            else: 
                select_mux('joystick_cmdvel')
                blink(False)
        except rospy.ServiceException e:
            print('Service call failed: %s' % e)

        rate.sleep()

if __name__ == '__main__':
    multiplexer() 
