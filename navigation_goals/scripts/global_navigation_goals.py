#! /usr/bin/env python

# Sends out navigation goals based on GPS coordinates

import rospy
from actionlib import SimpleActionClient, GoalStatus
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from nav_msgs.srv import GetPlan
from nav_msgs.msg import Plan

# Implementing action server that will send out simple target pose
# Service with plan info (pose) could be sent from JROS bridge or 
# a tester program

def set_global_nav_goal(req):

    client = actionlib.SimpleActionClient('act_simple_nav_goals', move_base_msgs.msg.MoveBaseAction)
    client.wait_for_server()

    goal = MoveBaseGoal()
    
    goal.target_pose.header.frame_id = "base_link"
    goal.target_pose.header.stamp = rospy.Time.now()

    # Subtract or just the goal? Find out how the service is called
    # Also, is the goal angular or xyz?
    goal.target_pose.pose.position.x = req.goal.pose.position.x 
    goal.target_pose.pose.orientation.w = req.goal.pose.Quaternion.w

    client.send_goal(goal)

    client.wait_for_result()
    
    if client.get_state() == GoalStatus.SUCCEEDED:
        rospy.loginfo('Succesfully executed global nav goal')
    else:
        rospy.loginfo('Failed to execute global nav goal')
    

if __name__ == '__main__':
    try:
        rospy.init_node('global_nav_goals')
        s = rospy.Service('global_nav_goals', 
                          GetPlan, 
                          set_global_nav_goal)
        rospy.spin()
    except rospy.ROSInterruptException:
        print('Program interrupted before completion', file=sys.stderr)
