#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
from std_msgs.msg import Int8
from std_msgs.msg import Float32
import serial

def sign(num):
    return 0 if num == 0 else num/abs(num)

# The established factors - revert to them if necessary
# effort_to_speed_factor = 0.0141
# effort_to_count_factor = 118.2752

effort_to_speed_factor = 0.013
effort_to_count_factor = 118.2752

speed = 0
turn = 0

left_speed = 0
right_speed = 0

def speed_callback(msg, pub):
    global right_speed, left_speed, speed, turn
    # global speed
    left_pub, right_pub = pub
    speed = msg.data

    if (sign(speed) == 0):
        left_pub.publish(left_speed * (-1) * sign(turn))
        right_pub.publish(right_speed * sign(turn))
    else:
        left_pub.publish(left_speed * sign(speed))
        right_pub.publish(right_speed * sign(speed))

def turn_callback(msg, pub):
    # global turn
    global left_speed, right_speed, speed, turn
    left_pub, right_pub = pub
    turn = msg.data

    if (sign(speed) == 0):
        left_pub.publish(left_speed * (-1) * sign(turn))
        right_pub.publish(right_speed * sign(turn))
    else:
        left_pub.publish(left_speed * sign(speed))
        right_pub.publish(right_speed * sign(speed))

def left_callback(msg, pub):
    global right_speed, left_speed, speed, turn
    left_pub, right_pub = pub

    left_speed = translate(msg.data)

    if (sign(speed) == 0):
        left_pub.publish(left_speed * (-1) * sign(turn))
        right_pub.publish(right_speed * sign(turn))
    else:
        left_pub.publish(left_speed * sign(speed))
        right_pub.publish(right_speed * sign(speed))

def right_callback(msg, pub):
    global right_speed, left_speed, speed, turn
    left_pub, right_pub = pub

    right_speed = translate(msg.data)
    
    if (sign(speed) == 0):
        left_pub.publish(left_speed * (-1) * sign(turn))
        right_pub.publish(right_speed * sign(turn))
    else:
        left_pub.publish(left_speed * sign(speed))
        right_pub.publish(right_speed * sign(speed))


# translates count per second received from arduino to speed
def translate(cps):
    # speed = cps * (effort_to_speed_factor/effort_to_count_factor)
    gear_ratio = 25.8
    spokes_per_rotation = 8 * 200 # 1 encoder spoke => 8 Arduino count (quad encoder, interrupt by level change)
    wheel_circumference = 1.34 # meters
    meters_per_second = cps * (wheel_circumference / (gear_ratio * spokes_per_rotation))
    return meters_per_second

def main():
    global autonomous 
    rospy.init_node('ard_odom_translator')

    left_pub = rospy.Publisher('lwheel', Float32, queue_size=10)
    right_pub = rospy.Publisher('rwheel', Float32, queue_size=10)
    
    motor_speed_sub = rospy.Subscriber('motor_speed', Int8, speed_callback, (left_pub, right_pub))
    motor_turn_sub = rospy.Subscriber('motor_turn', Int8, turn_callback, (left_pub, right_pub))  

    rospy.Subscriber('ard_cpsR', Float32, left_callback,(left_pub, right_pub))
    rospy.Subscriber('ard_cpsL', Float32, right_callback, (left_pub, right_pub))
    
    rate = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        rospy.spin()


if __name__ == '__main__':
    main() 
