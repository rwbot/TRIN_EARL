#!/usr/bin/env python

# Takes in /joy messages and publishes /motor_speed and /motor_turn
# for motor_commander.py to use to send command to the motor_controller

import rospy
from std_msgs.msg import Int8
from sensor_msgs.msg import Joy
from std_srvs.srv import SetBool

# constants taken from Q's labview code
speed_scale = 0.01220703125
turn_scale = 0.06103515625

# other constants
speed_deadzone = 0.25
turn_deadzone = 0.25

speed = 0
turn = 0

is_autonomous = False

# signum function


def sgn(x):
  return x / abs(x)

# Call back for topics recived from joy node. calculated speed is in range
# -55 to 55. calculated turn is in range -11 to 11


def joy_callback(msg, pub):
  global is_autonomous
  speed_pub = pub[0]
  turn_pub = pub[1]

  y_axis = msg.axes[1];
  x_axis_rotation = msg.axes[2];
  switch_autonomous = msg.buttons[7];
  right_trigger = msg.axes[4];

  # calculate speed
  if (abs(y_axis) < speed_deadzone):
    speed = 0
  else:
    speed = (y_axis - (speed_deadzone * sgn(y_axis))) / speed_scale

  # calculate turn
  if (abs(x_axis_rotation) < turn_deadzone):
    turn = 0
  else:
    turn = (x_axis_rotation - (turn_deadzone *
            sgn(x_axis_rotation))) / turn_scale

  # check if autonomous switch is pressed
  if (switch_autonomous):
    is_autonomous = not is_autonomous;
    rospy.loginfo("Autonomous mode is set to %r" % is_autonomous)
    rospy.wait_for_service('redirect_odom')
    redirect_odom = rospy.ServiceProxy('redirect_odom', SetBool)
    try:
        resp = redirect_odom(is_autonomous)
    except rospy.ServiceException:
        pass
        # Find way to ensure service call doesn't fail
        # The service call works despite the exception
        # rospy.logerr("Redirect odom service call failed")

  # publish xbox commands only if not autonomous
  if not is_autonomous and right_trigger == -1:
    speed_pub.publish(speed)
    turn_pub.publish(turn)


def main():
    # initialize node and handles
    rospy.init_node("xbox_drive")

    speed_pub = rospy.Publisher('motor_speed', Int8, queue_size=10)
    turn_pub = rospy.Publisher('motor_turn', Int8, queue_size=10)

    # initialize subscriber and register callback
    joy_sub = rospy.Subscriber('joy', Joy, joy_callback, (speed_pub, turn_pub))

    while not rospy.is_shutdown():
        rospy.spin()

if __name__ == '__main__':
    main()



# /*

# to connect xbox controller: sudo rmmod xpad then sudo xboxdrv --silent


# Data from Q
# SPEED
# y-Axis : left analog up and down, when all the way up it is -32768, when all the way down it is 32767
# -32768 scales to 61,  32767 scales to -62
# speed cale factor: 400   speed deadzone: 10240


# TURN
# x-axis rotation: right analog left and right, when all the way to the left -32768, when all the way to the right 32767
# -32768 scales to -17, 32767 scales to 16
# turn scale factor 2000    turn deadzone 10240

# SCALING
# scale deadzone (value is either y-axis or x-asis rotation)

# if abs(value) <= deadzone:
#     return 0

# else
#     ((value - deadzone*signum(value)) / scale_factor
# */
