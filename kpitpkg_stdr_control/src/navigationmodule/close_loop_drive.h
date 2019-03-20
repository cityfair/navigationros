#ifndef CLOSE_LOOP_H_
#define CLOSE_LOOP_H_

#include <stdlib.h>
#include <string>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>


class CloseLoopDrive {
private:
    ros::NodeHandle* rosNodeHandle;  // global reference for handle (not specific for the class).
    ros::Publisher topic_twist;  // for the class this Publisher topic is encapsulated (specific).

    double sample_dt;
    double speed; // 1 m/s speed command
    double yaw_rate; // 0.1 rad/sec yaw rate command

    geometry_msgs::Twist twist_cmd; //this is the message type required to send twist commands to STDR

    void stopCar();

public:

    double act_pos_x;
    double act_angle_z;

    bool bGPSdestReached;

    CloseLoopDrive(ros::NodeHandle& rosNodeHandleArg);
    void goGPS_east(double go_dest_east);
};


#endif // CLOSE_LOOP_H_

