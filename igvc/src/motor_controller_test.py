#!/usr/bin/python

import time
import rospy
import std_msgs.msg as msg
import serial
from collections import namedtuple

MAX_SPEED_EFFORT = 40
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

ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=9600,
    timeout=1,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS,
)

def speed_callback(speed):
    #rospy.loginfo('Speed received: ' + str(int(speed.data)))
    change_speed(int(speed.data))
    pass

def turn_callback(turn):
    #rospy.loginfo('Turn received: ' + str(int(turn.data)))
    change_turn(int(turn.data))
    pass

def change_speed(speed_effort):
    message = ''

    if (speed_effort < 0):
        message = commands.COM_FORW_CH2
    else:
        message = commands.COM_BACKW_CH2

    speed_effort = abs(speed_effort)

    if (speed_effort > MAX_SPEED_EFFORT):
        rospy.loginfo('Over max speed effort')
        speed_effort = MAX_SPEED_EFFORT

    effort_str = "{0:#0{1}x}".format(speed_effort,4)[2:]

    #rospy.loginfo('Speed command: ' + message + effort_str)

    write_byte(message + effort_str)


def change_turn(turn_effort):
    message = ''

    if (turn_effort < 0):
        message = commands.COM_LEFT_CH1
    else:
        message = commands.COM_RIGHT_CH1

    turn_effort = abs(turn_effort)

    if (turn_effort > MAX_TURN_EFFORT):
        rospy.loginfo('Over max turn effort')
        turn_effort = MAX_TURN_EFFORT

    effort_str = "{0:#0{1}x}".format(abs(turn_effort),4)[2:]

    #rospy.loginfo('Turn command: ' + message + effort_str)

    write_byte(message + effort_str)

def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')

def write_byte(string):
    ser.write((string + '\r').encode())

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
    init_serial_mode()
    main()
