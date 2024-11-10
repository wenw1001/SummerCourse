import rospy
from std_msgs.msg import String

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)

def listener():
    rospy.init_node('listener', anonymous=True) #listener為節點名稱
    rospy.Subscriber("chatter", String, callback)# receive publisher message
    rospy.spin()

if __name__ =="__main__":
    listener()