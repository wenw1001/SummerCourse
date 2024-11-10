#include "ros/ros.h"          
#include "std_msgs/String.h"  // not used
#include "std_msgs/Float32.h" //not used
#include "std_msgs/Int32.h" 

ros::Publisher topic_pub;
ros::Subscriber sub;
void Callback(const std_msgs::Int32::ConstPtr& msg){
    std_msgs::Int32 pub_msg;
    pub_msg.data = msg->data+1; 
    ROS_INFO("node_a : %d", msg->data);
    topic_pub.publish(pub_msg); 
}

int main(int argc, char **argv){
    ros::init(argc, argv, "node_a");  
    ros::NodeHandle nh;     
    sub = nh.subscribe("chatter4", 1000, Callback); 
    topic_pub = nh.advertise<std_msgs::Int32>("chatter1", 1000);

    // ROS_INFO("node_a.cpp START \n");

    ros::Rate loop_rate(1);   // 1Hz = 1/1s

    std_msgs::Int32 msg;
    msg.data = 1;
    loop_rate.sleep(); //make sure the code above are excuted
    topic_pub.publish(msg); //publish first message
    ROS_INFO("node_a published!"); 

  //int count = 0;
  while (ros::ok()){
    loop_rate.sleep();  // wait 1 second
    ros::spinOnce();   // call function Callback once

  }
  return 0;
}
