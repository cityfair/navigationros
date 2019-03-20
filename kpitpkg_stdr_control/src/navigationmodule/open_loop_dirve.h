// open-loop movement (meant without sensor data processing) for simulation of robot STDR

#ifndef OPEN_LOOP_H_
#define OPEN_LOOP_H_

#include <stdlib.h>
#include <string>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

class OpenLoopDrive {
private:
    ros::NodeHandle* rosNodeHandle;  // global reference for handle (not specific for the class).
    ros::Publisher topic_twist;  // for the class this Publisher topic is encapsulated (specific).

    double sample_dt;
    double speed; // 1m/s speed command
    double yaw_rate; //0.5 rad/sec yaw rate command
    double time_3_sec; // should move 3 meters or 1.5 rad in 3 seconds
    double time_var_sec;

    geometry_msgs::Twist twist_cmd; //this is the message type required to send twist commands to STDR

public:
    OpenLoopDrive(ros::NodeHandle& rosNodeHandleArg);
    void TestMove();
};
#endif

