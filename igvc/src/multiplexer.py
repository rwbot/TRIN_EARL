#!/usr/bin/env python

# Utilizes mux to multiplex the cmd_vel on the prompt of some controller
# so that the robot could be controlled manually and autonomously 

import rospy
from std_msgs.msg import Bool

def multiplexer():
    rospy.init_node('multiplexer')
    pub = rospy.Publisher('blink', Bool, queue_size=10)
    rate = rospy.Rate(10)
    blink = True
    while not rospy.is_shutdown():
        # TODO: Receive serial prompt and assign it to autonomous
        # Wait until received signal 
        # Likely, the button on the robot ..

        try:
           select_mux = rospy.ServiceProxy('mux/select', MuxSelect)
            if autonomous:
                select_mux('cmd_vel')
                # TODO: Change blink value 
                blink = 
            else: 
                select_mux('joystick_cmdvel')
        except rospy.ServiceException e:
            print('Service call failed: %s' % e)

        pub.publish(blink)
        
        rate.sleep()

if __name__ == '__main__':
    multiplexer() 
