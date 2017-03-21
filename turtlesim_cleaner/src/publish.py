import rospy
from std_msgs.msg import String


def publish():
    # Starts a new node
    rospy.init_node('publish', anonymous=True)
    pub = rospy.Publisher('/move_command', String, queue_size=10)

    # Receiveing the user's input
    direction = input("Choose your direction(up/down/left/right): ")

    while not rospy.is_shutdown():
        rospy.loginfo(direction)
        pub.publish(direction)
        rate.sleep()


if __name__ == '__main__':
    try:
        # Testing our function
        publish()
    except rospy.ROSInterruptException:
        pass
