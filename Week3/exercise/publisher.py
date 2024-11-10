#!/usr/bin/env python
import rospy
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('chatter', String,queue_size = 10)
    rospy.init_node('talker',anonymous = True) #talker為節點名稱
    rate = rospy.Rate(10) #
    while not rospy.is_shutdown(): 
        hello_str = 'welcome RVL %s' % rospy.get_time()
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass