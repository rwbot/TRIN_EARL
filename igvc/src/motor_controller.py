#!/usr/bin/python

import time
import rospy
import serial
from collections import namedtuple
import threading

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
    MAX_RPM = 2188.0 
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
        '?k', '?E','?V')

sem = threading.Semaphore()

def get_response():
    sem.acquire()
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
        #print(resp)
    sem.release()
    return resp

def write_byte(string):
    sem.acquire()
    ser.write((string + '\r').encode())
    #get_response()
    sem.release()

left_wheel_speed = 0
right_wheel_speed = 0

# Calculate speed based on encoder data 
def get_speed():
    while True:
        write_byte('?k')
        rpm = int(get_response(), 16)
        left_wheel_speed = Vehicle.WHEEL_CIRCUM * Vehicle.MAX_RPM * 
                            (rpm * 1.0 / Vehicle.MAX_EFFORT) 
        right_wheel_speed = Vehicle.WHEEL_CIRCUM * Vehicle.MAX_RPM * 
                            (rpm * 1.0 / Vehicle.MAX_EFFORT) 
        time.sleep(1)

# TODO: Check what we read from encoders
# TODO: Differetnial drive
# Translates speed to motor controller command 
def change_speed(speed):
    message = '' 
    needed_effort = (speed * Vehicle.MAX_EFFORT) / 
                    (Vehicle.WHEEL_CIRCUM * Vehicle.MAX_RPM)
    
    if (speed_effort < 0):
        message = commands.COM_FORW_CH2
    else:
        message = commands.COM_BACKW_CH2
    
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

# TODO: Change every calculations to differential drive mode calculations
#       Refer to closed-loop, differential drive

def main():
    global left_wheel_speed, right_wheel_speed
    rospy.init_node('motor_controller')

    cmd_vel_sub = rospy.Subscriber('mux_cmdvel', cmd_vel, vel_callback)
    left_whl_pub = rospy.Publisher('lwheel', Float32, queue_size=10)
    right_whl_pub = rospy.Publisher('rwheel', Float32, queue_size=10)
    
    t = threading.Thread(target=get_speed)
    t.start()

    prev_time = rospy.Time.from_sec(time.time())
    current_time = rospy.Time.from_sec(time.time())
    while not rospy.is_shudown():
        current_time = rospy.Time.from_sec(time.time())
        
        left_wheel_vel = left_wheel_speed / (current_time - prev_time)
        right_wheel_vel = right_wheel_speed / (current_time - prev_time)
        
        prev_time = current_time
        
        left_whl_pub.publish(left_wheel_vel)
        right_whl_pub.publish(right_wheel_vel)

        rospy.spin()

    t.join()


def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')

if __name__ == '__main__':
    init_serial_mode()
    main()
