// open_loop_drive.cpp

#include "open_loop_dirve.h"

OpenLoopDrive::OpenLoopDrive(ros::NodeHandle& rosNodeHandleArg) {
    this->rosNodeHandle = &rosNodeHandleArg;
    topic_twist = rosNodeHandle->advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1);

    sample_dt =0.01;
    speed = 1.0; // 1m/s speed command
    yaw_rate = 0.5; //0.5 rad/sec yaw rate command
    time_3_sec = 3.0; // should move 3 meters or 1.5 rad in 3 seconds
    time_var_sec = 2.3;


    // start with all zeros in the command message; should be the case by default, but just to be safe..
    twist_cmd.linear.x=0.0;
    twist_cmd.linear.y=0.0;
    twist_cmd.linear.z=0.0;
    twist_cmd.angular.x=0.0;
    twist_cmd.angular.y=0.0;
    twist_cmd.angular.z=0.0;


}

void OpenLoopDrive::TestMove() {
    ROS_INFO("BEGIN test move");

    ros::Rate loop_timer(1/sample_dt); //create a ros object from the ros “Rate” class; set 100Hz rate
    double timer=0.0;
    //start sending some zero-velocity commands, just to warm up communications with STDR
    for (int i=0;i<10;i++) {
      topic_twist.publish(twist_cmd);
      loop_timer.sleep();
    }
    twist_cmd.linear.x=speed; //command to move forward
    while(timer<time_3_sec) {
          topic_twist.publish(twist_cmd);
          timer+=sample_dt;
          loop_timer.sleep();
          }
    twist_cmd.linear.x=0.0; //stop moving forward
    twist_cmd.angular.z=yaw_rate; //and start spinning in place
    timer=0.0; //reset the timer
    while(timer<time_3_sec) {
          topic_twist.publish(twist_cmd);
          timer+=sample_dt;
          loop_timer.sleep();
          }

    twist_cmd.angular.z=0.0; //and stop spinning in place
    twist_cmd.linear.x=speed; //and move forward again
    timer=0.0; //reset the timer
    while(timer<time_var_sec) {
          topic_twist.publish(twist_cmd);
          timer+=sample_dt;
          loop_timer.sleep();
          }
    //halt the motion
    twist_cmd.angular.z=0.0;
    twist_cmd.linear.x=0.0;
    for (int i=0;i<10;i++) {
      topic_twist.publish(twist_cmd);
      loop_timer.sleep();
    }
    ROS_INFO("END test move");
}
