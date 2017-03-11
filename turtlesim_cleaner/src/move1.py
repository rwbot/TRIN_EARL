import rospy
from geometry_msgs.msg import Twist
PI = 3.1415926535897


def move():
    # Starts a new node
    rospy.init_node('robot_cleaner', anonymous=True)
    velocity_publisher = rospy.Publisher(
        '/turtle1/cmd_vel', Twist, queue_size=10)
    vel_msg = Twist()

    # Receiveing the user's input
    speed = 10
    distance = 50
    angle = 45

    # Calculate angles
    angular_speed = speed * 2 * PI / 360
        relative_angle = angle * 2 * PI / 360

    # Since we are moving just in x-axis
    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    vel_msg.angular.z = 0

    vel_msg.linear.x = abs(speed)  # forward
    vel_msg.angular.z = -abs(angular_speed)  # clockwise

    while not rospy.is_shutdown():

        # Setting the current time for distance calculus
        t0 = rospy.Time.now().to_sec()
        current_distance = 0
        current_angle = 0

        # Loop to move the turtle in an specified distance
        while(current_distance < distance & & current_angle < relative_angle):
            # Publish the velocity
            velocity_publisher.publish(vel_msg)
            # Takes actual time to velocity calculus
            t1 = rospy.Time.now().to_sec()
            # Calculates distancePoseStamped
            current_distance = speed * (t1 - t0)
            current_angle = angular_speed * (t1 - t0)

        # After the loop, stops the robot
        vel_msg.linear.x = 0

        vel_msg.angular.z = 0
        # Force the robot to stop
        velocity_publisher.publish(vel_msg)
        rospy.spin()


if __name__ == '__main__':
    try:
        # Testing our function
        move()
    except rospy.ROSInterruptException:
        pass
