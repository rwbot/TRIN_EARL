#!/usr/bin/python

import time
import rospy
import serial
from collections import namedtuple

# TODO: Verify the calculations made
# TODO: Fully implement the C++ version

def vel_callback(cmd_vel):
    linear_vel = cmd_vel.linear.x
    angular_vel = cmd_vel.angular.x

    left_speed = linear_vel + angular_vel * Vehicle.WHEEL_SEPARATION / 2.0
    right_speed = linear_vel - angular_vel * Vehicle.WHEEL_SEPARATION / 2.0
    
    change_speed(Vehicle.LEFT, left_speed)
    change_speed(Vehicle.RIGHT, right_speed)

class Vehicle:
    LEFT = 0
    RIGHT = 1
    WHEEL_DIA = 0.42                    # all measurements are in meters
    WHEEL_CIRCUM = math.pi * WHEEL_DIA
    WHEEL_SEPARTION = 0.69
    MAX_RPM = 3000.0 / 26.0
    MAX_EFFORT = 127

ser = serial.Serial(
    port='/dev/ttyUSB0', # TODO: Set as param
    baudrate=9600,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS)

# Add more commands
MotorControlCommands = namedtuple(
        'MotorControlCommands', 
        ['COM_FOR_CH1','COM_BAC_CH1',
         'COM_FOR_CH2','COM_BAC_CH2',
         'QUE_SPEED','QUE_BATTERY','QUE_VOLT'])

commands = MotorControlCommands(
        '!A', '!a',
        '!B', '!b',
        '?K', '?E','?V')

def get_response(self):
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
    return resp

def write_byte(string):
    ser.write((string + '\r').encode())

def get_speed(self, wheel_side):
    write_byte(commands.QUE_SPEED)
    effort = int(resp, 16)
    speed = Vehicle.WHEEL_CIRCUM * Vehicle.MAX_RPM * 
            (effort * 1.0 / Vehicle.MAX_EFFORT) 
    return speed

def change_speed(self, wheel_side, speed):
    message = ''
    needed_effort = (speed * Vehicle.MAX_EFFORT) / 
                    (Vehicle.WHEEL_CIRCUM * Vehicle.MAX_RPM)
    
    if (needed_effort < 0):
        if (wheel_side == Vehicle.LEFT) message = commands.COM_BAC_CH1
        else if (wheel_side == Vehicle.RIGHT) message = commands.COM_FOR_CH1
    else:
        if (wheel_side == Vehice.LEFT) message = commands.COM_FOR_CH1
        else if (wheel_side == Vehicle.RIGHT) message = commands.COM_FOR_CH2
    
    if (abs(needed_effort) > Vehicle.MAX_EFFORT):
        rospy.loginfo('Over max effort')
        effort_str = '7F'
    else:
        effort_str = hex(round(needed_effort))[2:]

    write_byte(message + effort_str) 

def vel_callback(msg):
    linear_vel = msg.linear.x
    angular_vel = msg.angular.x

    left_speed = linear_vel + angular_vel * Vehicle.wheel_separation / 2 
    right_speed = linear_Vel - angular_vel * Vehicle.wheel_separation / 2

    change_speed(left_speed)
    change_speed(right_speed)

def main():
    rospy.init_node('motor_controller')

    cmd_vel_sub = rospy.Subscriber('cmd_vel_sub', cmd_vel, vel_callback)
    left_whl_pub = rospy.Publisher('lwheel', Float32, queue_size=10)
    right_whl_pub = rospy.Publisher('rwheel', Float32, queue_size=10)
    
    prev_time = rospy.Time.from_sec(time.time())
    current_time = rospy.Time.from_sec(time.time())
    while not rospy.is_shudown():

        # TODO: Calculate speed using time differential
        left_wheel_speed = get_speed(Vehicle.LEFT);
        right_wheel_speed = get_speed(Vehicle.RIGHT);
        
        current_time = rospy.Time.from_sec(time.time())
        
        left_wheel_vel = left_wheel_speed / (current_time - prev_time)
        right_wheel_vel = right_wheel_speed / (current_time - prev_time)
        
        prev_time = current_time
        
        left_whl_pub.publish(left_wheel_vel)
        right_whl_pub.publish(right_wheel_vel)

        rospy.spin()


def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')

if __name__ == '__main__':
    init_serial_mode()
    main()
