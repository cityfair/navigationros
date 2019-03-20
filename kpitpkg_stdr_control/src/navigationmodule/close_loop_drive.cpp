// close_loop_drive.cpp

#include "close_loop_drive.h"
#include <math.h>


CloseLoopDrive::CloseLoopDrive(ros::NodeHandle& rosNodeHandleArg) {
    this->rosNodeHandle = &rosNodeHandleArg;
    topic_twist = rosNodeHandle->advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1);

    sample_dt =0.1;
    speed = 0.1; // 1 m/s speed command
    yaw_rate = 0.1; //0.1 rad/sec yaw rate command

    bGPSdestReached = false;

    twist_cmd.linear.x=0.0;
    twist_cmd.angular.z=0.0;
    act_pos_x = 0;
    act_angle_z = 0;

}


void CloseLoopDrive::goGPS_east(double go_dest_east) {
   if (act_pos_x !=0 || act_angle_z !=0) {

       if ( abs( go_dest_east - act_pos_x ) < 0.2 ) {
           stopCar();
           bGPSdestReached = true;
       }

       // check the angle
       if (  !(act_angle_z>0.00 && act_angle_z<0.05) ) {
           twist_cmd.linear.x=0.0; //stop moving forward
           twist_cmd.angular.z=yaw_rate; //and start spinning in place
           topic_twist.publish(twist_cmd);
           ROS_INFO("SPIN IN PLACE UNTIL ORIENTATION_Z IS 0");
       } else {
           // check the whether the destination x is reached.
           if ( abs( go_dest_east - act_pos_x ) > 0.2 ) {
               twist_cmd.angular.z=0.0; //and stop spinning in place
               twist_cmd.linear.x=speed; //and move forward again
               topic_twist.publish(twist_cmd);
               ROS_INFO("MOVING EAST");
           } else {
               stopCar();
               bGPSdestReached = true;
           }
       }

   }
}

void CloseLoopDrive::stopCar() {
    twist_cmd.linear.x=0.0;
    twist_cmd.angular.z=0.0;
    topic_twist.publish(twist_cmd);
    ROS_INFO("CAR stopped");
}
