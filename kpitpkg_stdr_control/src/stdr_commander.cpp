#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "navigationmodule/open_loop_dirve.h"
#include "navigationmodule/close_loop_drive.h"


// global navigation control with saved actual positions.
CloseLoopDrive* closeLoopDrive;


void callBackOdometry(const nav_msgs::Odometry::ConstPtr& msg){
    ROS_INFO("Position-> x: [%f], y: [%f;, orientation z: [%f]", msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.orientation.z);

    closeLoopDrive->act_pos_x =  msg->pose.pose.position.x;
    closeLoopDrive->act_angle_z = msg->pose.pose.orientation.z;
}


//node to send Twist commands to the Simple 2-Dimensional Robot Simulator via cmd_vel
int main(int argc, char **argv) {
    ros::init(argc, argv, "stdr_commander");
    ros::NodeHandle nodeHandle; // two lines to create a publisher object that can talk to ROS

    OpenLoopDrive openLoopDrive( nodeHandle );
    //openLoopDrive.TestMove();

    closeLoopDrive = new CloseLoopDrive( nodeHandle );

    double go_east_m = 8.0;  // m

    ros::Subscriber subscriberToOdometry = nodeHandle.subscribe("/robot0/odom", 1, callBackOdometry);

    double sample_rate_read_odometry = 5; // Hz
    ros::Rate ros_update_time(sample_rate_read_odometry);

    while(ros::ok() && !closeLoopDrive->bGPSdestReached ) {
        ros::spinOnce();
        ros_update_time.sleep();
        closeLoopDrive->goGPS_east(go_east_m);
    }

    return 0;
}
