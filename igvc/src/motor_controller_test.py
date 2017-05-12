#!/usr/bin/python

import time
import rospy
import std_msgs.msg as msg
import serial
from collections import namedtuple
import threading
import time
import numpy

MAX_SPEED_EFFORT = 60
MAX_TURN_EFFORT = 15

left_speed_pub = rospy.Publisher('lwheel', msg.Float32, queue_size=10)
right_speed_pub = rospy.Publisher('rwheel', msg.Float32, queue_size=10)

# 0.317
# Add more commands
MotorControlCommands = namedtuple(
        'MotorControlCommands', 
        ['COM_LEFT_CH1','COM_RIGHT_CH1',
         'COM_FORW_CH2','COM_BACKW_CH2',
         'QUE_SPEED','QUE_BATTERY','QUE_VOLT'])

commands = MotorControlCommands(
        '!A', '!a',
        '!B', '!b',
        '?k', '?E','?V')

ser = serial.Serial(
    port='/dev/ttyUSB1',
    baudrate=9600,
    timeout=1,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS,
)

cur_speed_effort = 0
cur_turn_effort = 0

sem = threading.Semaphore()

def speed_callback(speed):
    #change_speed(65)
    change_speed(int(speed.data))

def turn_callback(turn):
    #change_turn(-30)
    change_turn(int(turn.data))

def change_speed(speed_effort):
    global cur_speed_effort
    message = ''

    if (speed_effort < 0):
        message = commands.COM_FORW_CH2
    else:
        message = commands.COM_BACKW_CH2

    cur_speed_effort = speed_effort
    speed_effort = abs(speed_effort)

    if (speed_effort > MAX_SPEED_EFFORT):
        #rospy.loginfo('Over max speed effort')
        speed_effort = MAX_SPEED_EFFORT

    effort_str = "{0:#0{1}x}".format(speed_effort,4)[2:]

    #rospy.loginfo('Speed command: ' + message + effort_str)

    write_byte(message + effort_str)
    

    calculate_speed()

def change_turn(turn_effort):
    global cur_turn_effort
    message = ''

    if (turn_effort < 0):
        message = commands.COM_LEFT_CH1
    else:
        message = commands.COM_RIGHT_CH1

    cur_turn_effort = turn_effort
    turn_effort = abs(turn_effort)

    if (turn_effort > MAX_TURN_EFFORT):
        #rospy.loginfo('Over max turn effort')
        turn_effort = MAX_TURN_EFFORT

    effort_str = "{0:#0{1}x}".format(abs(turn_effort),4)[2:]

    #rospy.loginfo('Turn command: ' + message + effort_str)

    write_byte(message + effort_str)
    

    

def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')
    get_response()
    get_response()

def write_byte(string, get_speed=False):
    sem.acquire()
    ser.write((string + '\r').encode())
    
    resp = get_response().strip()[2:5]
    if get_speed:
       print resp
    # print resp
    if get_speed and resp:
        try: 
            print int(resp, 16)
            print ("%d m/s \n" % (((int(resp, 16) / 127.0) * (2188.0 / 60) * 1.2))) # 1.2 estimate of circum
        except ValueError:
            sem.release()
            pass

    sem.release()

def get_speed():
    while True:
        write_byte('?k', get_speed=True)        
        time.sleep(0.5)

def get_response():
    #sem.acquire()
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
        #print(resp)
    #sem.release()
    return resp

t = None

def kill_thread():
    global t 
    #t.join()

def calculate_speed():
    global cur_speed_effort
    global cur_turn_effort

    left_effort = cur_speed_effort - cur_turn_effort
    right_effort = cur_speed_effort + cur_turn_effort

    # 0.0141 taken from effort vs speed data collected using
    # arduino and a photosensor


    left_speed = 0.0141 * left_effort
    right_speed = 0.0141 * right_effort

    print "Left velocity: " + str(left_speed) + "m/s"
    print "Right velocity: " + str(right_speed) + "m/s"

    left_speed_pub.publish(left_speed)
    right_speed_pub.publish(right_speed)



def main():
    global t 
    rospy.init_node('motor_controller_test')

    motor_speed_sub = rospy.Subscriber('motor_speed', msg.Int8, speed_callback)
    motor_turn_sub = rospy.Subscriber('motor_turn', msg.Int8, turn_callback)

    

    #rate = rospy.Rate(1)  # 10hz

    t = threading.Thread(target=get_speed)
    #t.start()

    rospy.on_shutdown(kill_thread)

    current_time = rospy.Time.from_sec(time.time())
    
    while not rospy.is_shutdown():
        prev_time = current_time
        print "in loop"

        rospy.spin()
        #

    #t.join()



if __name__ == '__main__':
    init_serial_mode()

    main()
