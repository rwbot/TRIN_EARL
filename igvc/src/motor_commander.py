#!/usr/bin/python

# Subscribes to /motor_speed and /motor_turn to 
# send commands to the motor controller

import time
import rospy
import std_msgs.msg as msg
from sensor_msgs.msg import Joy
import serial
from collections import namedtuple
import threading
import time
import numpy
from std_srvs.srv import SetBool
from std_srvs.srv import Empty


MAX_SPEED_EFFORT = 45
MAX_TURN_EFFORT = 15

is_autonomous = False

left_speed_pub = rospy.Publisher('lwheel', msg.Float32, queue_size=10)
right_speed_pub = rospy.Publisher('rwheel', msg.Float32, queue_size=10)


# TODO: Implement Subscriber to Joy that listens to start key 
# and enable/disable motor output from this node 

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
    # port='/dev/ttyUSB1',
    port=rospy.get_param('~port', '/dev/ttyUSB0'),
    baudrate=9600,
    timeout=1,
    parity=serial.PARITY_EVEN,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.SEVENBITS,
)


cur_speed_effort = 0
cur_turn_effort = 0
is_autonomous = False
sem = threading.Semaphore()

# def joy_callback(msg):
#     global is_autonomous
#     if (msg.buttons[7]):
#         is_autonomous = not is_autonomous
#         rospy.loginfo("Autonomous mode is set to %r" % is_autonomous)
#         rospy.wait_for_service('redirect_odom')
#         redirect_odom = rospy.ServiceProxy('redirect_odom', SetBool)
#         try:
#             resp = redirect_odom(is_autonomous)
#         except rospy.ServiceException:
#             pass 
            
            # Find way to ensure service call doesn't fail 
            # The service call works despite the exception

            # rospy.logerr("Redirect odom service call failed") 

def speed_callback(speed):
    change_speed(int(speed.data))

def turn_callback(turn):
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

    #rospy.logerr('Speed command: ' + message + effort_str)

    write_byte(message + effort_str)
    #get_response()
    
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

    #rospy.logerr('Turn command: ' + message + effort_str)

    write_byte(message + effort_str)  
    #get_response()  

# Consider also resetting the motor controller if the motor controller does not work
def init_serial_mode():
    for i in range(0, 10):
        ser.write('\r')


def reset_controller():
    write_byte('^FF') # Restart controller to apply params
    #print(get_response())

def set_differential_mode():
    write_byte('^01 05') # Mixed mode, closed loop
 

def write_byte(string, get_speed=False):
    #sem.acquire()
    ser.write((string + '\r').encode())
    
    #print string

    # resp = get_response().strip()[2:5]
    # #if get_speed:
    #    # print resp
    # if get_speed and resp:
    #     try: 
    #         print int(resp, 16)
    #         print ("%d m/s \n" % (((int(resp, 16) / 127.0) * (2188.0 / 60) * 1.2))) # 1.2 estimate of circum
    #     except ValueError:
    #         sem.release()
    #         pass

    #sem.release()

def get_speed():
    while True:
        write_byte('?k', get_speed=True)        
        time.sleep(0.5)

def get_response():
    #sem.acquire()
    resp = ''
    while ser.inWaiting() > 0:
        resp += ser.read(1)
        
    #sem.release()
    rospy.logerr('Response: ' + resp)
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

    # NOTE: only enable for debugging purposes
    # print "Left velocity: " + str(left_speed) + "m/s"
    # print "Right velocity: " + str(right_speed) + "m/s"

    #left_speed_pub.publish(left_speed)
    #right_speed_pub.publish(right_speed)



def main():
    global t 
    rospy.init_node('motor_controller_test')

    motor_speed_sub = rospy.Subscriber('motor_speed', msg.Int8, speed_callback)
    motor_turn_sub = rospy.Subscriber('motor_turn', msg.Int8, turn_callback)  

    #joy_sub = rospy.Subscriber('joy', Joy, joy_callback)  
    services_available = [x[0] for x in rospy.get_services()]
    if '/imu/zeroout' in services_available:
        rospy.loginfo('Discovered IMU service - Now trying to remove IMU drift')
        rospy.wait_for_service('/imu/zeroout')
        try:
            zeroout_imu = rospy.ServiceProxy('/imu/zeroout', Empty)
            resp = zeroout_imu()
            rospy.loginfo('Zeroed out IMU drift')
        except rospy.ServiceException, e: 
            rospy.logerr('Failed the service call to zeroout IMU')


    #rate = rospy.Rate(1)  # 10hz

    #t = threading.Thread(target=get_speed)
    #t.start()

    rospy.on_shutdown(kill_thread)

    current_time = rospy.Time.from_sec(time.time())
    
    while not rospy.is_shutdown():
        prev_time = current_time
        print "Running motor commander"
        rospy.spin()

    #t.join()



if __name__ == '__main__':
    init_serial_mode()
    reset_controller()
    set_differential_mode()
    main()
