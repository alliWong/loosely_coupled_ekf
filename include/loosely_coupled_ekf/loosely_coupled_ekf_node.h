/*
  \file		loosely_coupled_ekf_node.h
  \brief	Header file for Loosely Coupled EKF ROS wrapper.
  
  \author	Stephen Geiger <sag0020@tigermail.auburn.edu>
  \date		April 4, 2017
*/


#ifndef LOOSELY_COUPLED_EKF_NODE_H
#define	LOOSELY_COUPLED_EKF_NODE_H

// ROS Includes
#include <ros/ros.h>
#include <ros/time.h>

// C++ Includes
#include <math.h>
#include <cmath>
#include <vector>

// Message Includes
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <ublox_msgs/NavVELNED.h>
#include <ublox_msgs/NavSOL.h>

// Eigen Package include
#include <eigen3/Eigen/Dense>

// GPS Coordinate Conversions Include
#include <gnsscommon/CoordinateConversions.hpp>

// Other Includes
#include "loosely_coupled_ekf.h"

// Define PI and gpsPI
#ifndef PI
#define PI 3.14159265358979
#endif
#ifndef gpsPI
#define gpsPI 3.1415926535898
#endif

int GPS_SENSOR;
int IMU_SENSOR;
int NAV_FRAME;
bool POS_MEASURE;
bool VEL_MEASURE;

class LooselyCoupledNode
{
public:

	LooselyCoupledNode();
	~LooselyCoupledNode(){};

	// Subscribers
	ros::Subscriber odomSub;
	ros::Subscriber courseSub;
	ros::Subscriber imuSub;
	// ublox Callbacks
	void ublox_course_callback(const ublox_msgs::NavVELNED& msg); // Course
	void ublox_odom_callback(const ublox_msgs::NavSOL& msg); // Odometry
    // Novatel Callbacks
	void novatel_odom_callback(const nav_msgs::Odometry& msg); // Odometry
    // Xbow Callback
    void xbow_callback(const sensor_msgs::Imu& msg); // Gyros and Accels

	LooselyCoupledEKF EKF;

private:

	// GPS Fix and Measurement Update Conditions
	bool initLLA;
	bool MEAS_UPDATE_COND;

    int ecef2enuVel(double enu_vel[], double ecef_vel[], double lat, double lon, double alt);
    void ned2enuCourse(double &angle);
    void wrapToPi(double &angle);
    void wrapTo2Pi(double &angle);
    Eigen::Matrix3d rot(double angle, int axis);
	Eigen::Matrix3d rotation_matrix(std::vector <double> euler);

	Eigen::Matrix3d C;
	double ecef_pos[3];
	double ecef_vel[3];
	double enu_pos[3];
	double enu_vel[3];
	double ned_pos[3];
	double ned_vel[3];

	// Lat, Lon, Alt array
	double lla[3];
	double init_lla[3];

	// Raw Measurement Variables
	int ublox_course;
	double course;
	double roll_rate_raw;
	double pitch_rate_raw;
	double yaw_rate_raw;
	double accel_x_raw;
	double accel_y_raw;
	double accel_z_raw;
};


#endif