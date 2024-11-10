#include "ros/ros.h"          
#include "std_msgs/String.h"  
#include "std_msgs/Float32.h" 
#include "std_msgs/Int32.h" 

ros::Publisher topic_pub;
ros::Subscriber sub;
void Callback(const std_msgs::Int32::ConstPtr& msg){
    std_msgs::Int32 pub_msg;
    pub_msg.data = msg->data+1; 
    ROS_INFO("node_d : %d", msg->data);
    topic_pub.publish(pub_msg); 
}

int main(int argc, char **argv){
    ros::init(argc, argv, "node_d");  
    ros::NodeHandle nh;     
    sub = nh.subscribe("chatter3", 1000, Callback); 
    topic_pub = nh.advertise<std_msgs::Int32>("chatter4", 1000);  

    // ROS_INFO("node_d.cpp START \n");

    ros::Rate loop_rate(1);   // 1Hz = 1/1s

  while (ros::ok()){
    
    loop_rate.sleep();  
    ros::spinOnce();   

  }
  return 0;
}
