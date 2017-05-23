import rospy
from std_msgs.msg import String
from std_msgs.msg import Int16

def callback(data):
    rospy.loginfo("Data is %d", data.data)

def talker(data):
    command = rospy.Publisher("dir", Int16, queue_size=10)
	
    if data.data < 50:
	command.publish(1)
    elif data.data < 100:
	command.publish(2)
    elif data.data < 150:
	command.publish(3)
    else:
	command.publish(4)

def listener():
    rospy.init_node("subscriber_node", anonymous=True)
    rospy.Subscriber("distance", Int16, callback)
    rospy.Subscriber("distance", Int16, talker)
    rospy.spin()

if __name__ == '__main__':
    listener()
    try:
	talker()
    except rospy.ROSInterruption:
	pass
