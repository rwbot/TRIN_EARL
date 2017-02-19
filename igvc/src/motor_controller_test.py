#!/usr/bin/python

import time
import rospy
import std_msgs.msg as msg
import serial
from collections import namedtuple

MAX_SPEED_EFFORT = 60
MAX_TURN_EFFORT = 15

# Add more commands
MotorControlCommands = namedtuple(
        'MotorControlCommands', 
        ['COM_LEFT_CH1','COM_RIGHT_CH1',
         'COM_FORW_CH2','COM_BACKW_CH2',
         'QUE_SPEED','QUE_BATTERY','QUE_VOLT'])

commands = MotorControlCommands(
        '!A', '!a',
        '!B', '!b',
        '?K', '?E','?V')

def speed_callback(speed):
    rospy.loginfo('Speed received: ' + str(int(speed.data)))
    change_speed(int(speed.data))
    pass

def turn_callback(turn):
    rospy.loginfo('Turn received: ' + str(int(turn.data)))
    change_turn(int(turn.data))
    pass

def change_speed(speed_effort):
    message = ''

    if (speed_effort < 0):
        message = commands.COM_FORW_CH2
    else:
        message = commands.COM_BACKW_CH2

    if (abs(speed_effort) > MAX_SPEED_EFFORT):
        rospy.loginfo('Over max speed effort')
        effort_str = '7F'
    else:
        effort_str = hex(speed_effort)[2:]

    rospy.loginfo('Speed command: ' + message + effort_str)

    #ser.write(message + effort_str)


def change_turn(turn_effort):
    message = ''

    if (turn_effort < 0):
        message = commands.COM_LEFT_CH1
    else:
        message = commands.COM_RIGHT_CH1

    if (abs(turn_effort) > MAX_TURN_EFFORT):
        rospy.loginfo('Over max speed effort')
        effort_str = '7F'
    else:
        effort_str = hex(turn_effort)[2:]

    rospy.loginfo('Turn command: ' + message + effort_str)

    #ser.write(message + effort_str)

def main():
    rospy.init_node('motor_controller_test')

    motor_speed_sub = rospy.Subscriber('motor_speed', msg.Int8, speed_callback)
    motor_turn_sub = rospy.Subscriber('motor_turn', msg.Int8, turn_callback)

    #rate = rospy.Rate(10)  # 10hz

    current_time = rospy.Time.from_sec(time.time())
    while not rospy.is_shutdown():
        prev_time = current_time
        rospy.spin()
        #rate.sleep()


if __name__ == '__main__':
    main()
