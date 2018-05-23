#include "phidgets_imu/imu_ros_i.h"

namespace phidgets {

ImuRosI::ImuRosI(ros::NodeHandle nh, ros::NodeHandle nh_private):
  Imu(),
  nh_(nh), 
  nh_private_(nh_private),
  initialized_(false)
{
  ROS_INFO ("Starting Phidgets IMU");

  // **** get parameters

  if (!nh_private_.getParam ("period", period_))
    period_ = 8; // 8 ms
  if (!nh_private_.getParam ("frame_id", frame_id_))
    frame_id_ = "imu";
  if (!nh_private_.getParam ("angular_velocity_stdev", angular_velocity_stdev_))
    angular_velocity_stdev_ = 0.02 * (M_PI / 180.0); // 0.02 deg/s resolution, as per manual
  if (!nh_private_.getParam ("linear_acceleration_stdev", linear_acceleration_stdev_))
    linear_acceleration_stdev_ = 300.0 * 1e-6 * G; // 300 ug as per manual

  bool has_compass_params =
      nh_private_.getParam ("cc_mag_field", cc_mag_field_)
      && nh_private_.getParam ("cc_offset0", cc_offset0_)
      && nh_private_.getParam ("cc_offset1", cc_offset1_)
      && nh_private_.getParam ("cc_offset2", cc_offset2_)
      && nh_private_.getParam ("cc_gain0", cc_gain0_)
      && nh_private_.getParam ("cc_gain1", cc_gain1_)
      && nh_private_.getParam ("cc_gain2", cc_gain2_)
      && nh_private_.getParam ("cc_t0", cc_T0_)
      && nh_private_.getParam ("cc_t1", cc_T1_)
      && nh_private_.getParam ("cc_t2", cc_T2_)
      && nh_private_.getParam ("cc_t3", cc_T3_)
      && nh_private_.getParam ("cc_t4", cc_T4_)
      && nh_private_.getParam ("cc_t5", cc_T5_);

  // float add_lin_accel_x = 0;
  // float add_lin_accel_y = 0; 
  // float add_lin_accel_z = 0;
  // float lin_accel_x_range = 0;
  // float lin_accel_y_range = 0;
  // float lin_accel_z_range = 0;
  // float ang_vel_x_range = 0;
  // float ang_vel_y_range = 0;
  // float ang_vel_z_range = 0;

  // nh_private_.getParam("add_lin_accel_x", add_lin_accel_x);
  // nh_private_.getParam("add_lin_accel_y", add_lin_accel_y);
  // nh_private_.getParam("add_lin_accel_z", add_lin_accel_z);
  // nh_private_.getParam("lin_accel_x_range", lin_accel_x_range);
  // nh_private_.getParam("lin_accel_y_range", lin_accel_y_range);
  // nh_private_.getParam("ang_vel_x_range", ang_vel_x_range);
  // nh_private_.getParam("ang_vel_y_range", ang_vel_y_range);
  // nh_private_.getParam("ang_vel_z_range", ang_vel_z_range);

  // **** advertise topics

  imu_publisher_ = nh_.advertise<ImuMsg>(
    "imu/data_raw", 5);
  mag_publisher_ = nh_.advertise<MagMsg>(
    "imu/mag", 5);
  cal_publisher_ = nh_.advertise<std_msgs::Bool>(
    "imu/is_calibrated", 5);

  bearing_publisher_ = nh_.advertise<std_msgs::Float32>( 
   "imu/bearing", 5);

  // **** advertise services

  cal_srv_ = nh_.advertiseService(
    "imu/calibrate", &ImuRosI::calibrateService, this);

  zero_srv_ = nh_.advertiseService(
    "imu/zeroout", &ImuRosI::zeroOutService, this);

  // **** initialize variables and device
  
  imu_msg_.header.frame_id = frame_id_;

  // build covariance matrices

  double ang_vel_var = angular_velocity_stdev_ * angular_velocity_stdev_;
  double lin_acc_var = linear_acceleration_stdev_ * linear_acceleration_stdev_;

  for (int i = 0; i < 3; ++i)
  for (int j = 0; j < 3; ++j)
  {
    int idx = j*3 +i;

    if (i == j)
    {
      imu_msg_.angular_velocity_covariance[idx]    = ang_vel_var;
      imu_msg_.linear_acceleration_covariance[idx] = lin_acc_var;
    }
    else
    {
      imu_msg_.angular_velocity_covariance[idx]    = 0.0;
      imu_msg_.linear_acceleration_covariance[idx] = 0.0;
    }
  }

  // signal that we have no orientation estimate (see Imu.msg)
  imu_msg_.orientation_covariance[0] = -1;

  initDevice();

  if (has_compass_params)
  {
    int result = CPhidgetSpatial_setCompassCorrectionParameters(imu_handle_, cc_mag_field_,
          cc_offset0_, cc_offset1_, cc_offset2_, cc_gain0_, cc_gain1_, cc_gain2_,
          cc_T0_, cc_T1_, cc_T2_, cc_T3_, cc_T4_, cc_T5_);
    if (result)
    {
      const char *err;
      CPhidget_getErrorDescription(result, &err);
      ROS_ERROR("Problem while trying to set compass correction params: '%s'.", err);
    }
  }
  else
  {
    ROS_INFO("No compass correction params found.");
  }
}

void ImuRosI::initDevice()
{
	ROS_INFO("Opening device");
	open(-1);

	ROS_INFO("Waiting for IMU to be attached...");
	int result = waitForAttachment(10000);
	if(result)
	{
	  const char *err;
		CPhidget_getErrorDescription(result, &err);
		ROS_FATAL("Problem waiting for IMU attachment: %s Make sure the USB cable is connected and you have executed the phidgets_api/share/setup-udev.sh script.", err);
	}

	// set the data rate for the spatial events
  setDataRate(period_);

  // calibrate on startup
  calibrate();
}

bool ImuRosI::calibrateService(std_srvs::Empty::Request  &req,
                               std_srvs::Empty::Response &res)
{
  calibrate();
  return true;
}

void ImuRosI::calibrate()
{
  ROS_INFO("Calibrating IMU...");
  zero();
  ROS_INFO("Calibrating IMU done.");

  time_zero_ = ros::Time::now();

  // publish message
  std_msgs::Bool is_calibrated_msg;
  is_calibrated_msg.data = true;
  cal_publisher_.publish(is_calibrated_msg);
}

bool ImuRosI::zeroOutService(std_srvs::Empty::Request  &req,
                            std_srvs::Empty::Response &res)
{
  zeroOut();
  return true;
}

void ImuRosI::zeroOut()
{
  ROS_INFO("Zeroing out accelerations");
  negateAccel();
  ROS_INFO("Zeroed out accelerations");

  time_zero_ = ros::Time::now();

  // publish message
  std_msgs::Bool is_calibrated_msg;
  is_calibrated_msg.data = true;
  cal_publisher_.publish(is_calibrated_msg);
}

void ImuRosI::negateAccel(){

  ros::Time start_time = ros::Time::now();
  ros::Time end_time = ros::Time::now();
  
  double AccumLinX, AccumLinY, AccumLinZ;
  AccumLinX = AccumLinY = AccumLinZ = 0; 

  int counter = 0;

  while ((end_time.toSec() - start_time.toSec()) < 5) {
    AccumLinX += currLinX; 
    AccumLinY += currLinY;
    //AccumLinZ += currLinZ;

    counter++;
    end_time = ros::Time::now();
  }

  linXZero  = AccumLinX / counter;
  linYZero  = AccumLinY / counter;
  linZZero  = AccumLinZ / counter;

  zeroedOut_ = true;
}

void ImuRosI::processImuData(CPhidgetSpatial_SpatialEventDataHandle* data, int i)
{
  // **** calculate time from timestamp
  ros::Duration time_imu(data[i]->timestamp.seconds + 
                         data[i]->timestamp.microseconds * 1e-6);

  ros::Time time_now = time_zero_ + time_imu;

  double timediff = time_now.toSec() - ros::Time::now().toSec();
  if (fabs(timediff) > 0.1) {
    ROS_WARN("IMU time lags behind by %f seconds, resetting IMU time offset!", timediff);
    time_zero_ = ros::Time::now() - time_imu;
    time_now = ros::Time::now();
  }

  // **** initialize if needed

  if (!initialized_)
  { 
    last_imu_time_ = time_now;
    initialized_ = true;
  }

  // **** create and publish imu message

  boost::shared_ptr<ImuMsg> imu_msg = 
    boost::make_shared<ImuMsg>(imu_msg_);

  imu_msg->header.stamp = time_now;

  // set linear acceleration
  
  // Switching x and z axis 
  imu_msg->linear_acceleration.x = - data[i]->acceleration[0] * G;
  imu_msg->linear_acceleration.y = - data[i]->acceleration[1] * G + 0.54;
  imu_msg->linear_acceleration.z = - data[i]->acceleration[2] * G;

  // Manual calibration - setting linear acceleration to 0 if below certain threshold
  if (imu_msg->linear_acceleration.x < 0.02 && imu_msg->linear_acceleration.x > (-0.02)) imu_msg->linear_acceleration.x = 0;
  if (imu_msg->linear_acceleration.y < 0.02 && imu_msg->linear_acceleration.y > (-0.02)) imu_msg->linear_acceleration.y = 0;
  
  // set angular velocities

  // Changing angular velocity for x and z
  imu_msg->angular_velocity.x = data[i]->angularRate[0] * (M_PI / 180.0);
  imu_msg->angular_velocity.y = data[i]->angularRate[1] * (M_PI / 180.0);
  imu_msg->angular_velocity.z = data[i]->angularRate[2] * (M_PI / 180.0);

  if (imu_msg->angular_velocity.x < 0.005 && imu_msg->angular_velocity.x > -0.005) imu_msg->angular_velocity.x = 0;
  if (imu_msg->angular_velocity.y < 0.005 && imu_msg->angular_velocity.y > -0.005) imu_msg->angular_velocity.y = 0;
  if (imu_msg->angular_velocity.z < 0.005 && imu_msg->angular_velocity.z > -0.005) imu_msg->angular_velocity.z = 0;

  // Subtract out the variables for zeroing out

  currLinX = imu_msg->linear_acceleration.x;
  currLinY = imu_msg->linear_acceleration.y;
  currLinZ = imu_msg->linear_acceleration.z;

  if (zeroedOut_) { 
    imu_msg->linear_acceleration.x = imu_msg->linear_acceleration.x - linXZero;
    imu_msg->linear_acceleration.y = imu_msg->linear_acceleration.y - linYZero;
    imu_msg->linear_acceleration.z = imu_msg->linear_acceleration.z - linZZero;
  }

  imu_publisher_.publish(imu_msg);

  // **** create and publish magnetic field message

  boost::shared_ptr<MagMsg> mag_msg = 
    boost::make_shared<MagMsg>();
  
  mag_msg->header.frame_id = frame_id_;
  mag_msg->header.stamp = time_now;

  if (data[i]->magneticField[0] != PUNK_DBL)
  {
    mag_msg->vector.x = data[i]->magneticField[0];
    mag_msg->vector.y = data[i]->magneticField[1];
    mag_msg->vector.z = data[i]->magneticField[2];
  }
  else
  {
    double nan = std::numeric_limits<double>::quiet_NaN();

    mag_msg->vector.x = nan;
    mag_msg->vector.y = nan;
    mag_msg->vector.z = nan;
  }
   
  mag_publisher_.publish(mag_msg);

  std_msgs::Float32 bearing_msg;


  double accel[3] = {imu_msg->linear_acceleration.x, imu_msg->linear_acceleration.y, imu_msg->linear_acceleration.z};
  double magField[3] = {mag_msg->vector.x, mag_msg->vector.y, mag_msg->vector.z};
  double bearing = calculateCompassBearing(accel, magField);


  bearing_msg.data = bearing;

  bearing_publisher_.publish(bearing_msg);

}

void ImuRosI::dataHandler(CPhidgetSpatial_SpatialEventDataHandle *data, int count)
{
  for(int i = 0; i < count; i++)
    processImuData(data, i);
}

//This finds a magnetic north bearing, correcting for board tilt and roll as measured by the accelerometer
//This doesn't account for dynamic acceleration - ie accelerations other than gravity will throw off the calculation
//Calculations based on AN4248
//double[] lastAngles = { 0, 0, 0 };
double ImuRosI::calculateCompassBearing(double accel[3], double magField[3]) {
  double PI = 3.14159;

  // double[] accel = {
  //   e.Acceleration[0],
  //   e.Acceleration[1],
  //   e.Acceleration[2]};

  // double[] magField = {
  //   e.MagneticField[0],
  //   e.MagneticField[1],
  //   e.MagneticField[2]};

  //Roll Angle - about axis 0
  //  tan(roll angle) = gy/gz
  //  Use Atan2 so we have an output os (-180 - 180) degrees
  double rollAngle = atan2(accel[1], accel[2]);

  //Pitch Angle - about axis 1
  //  tan(pitch angle) = -gx / (gy * sin(roll angle) * gz * cos(roll angle))
  //  Pitch angle range is (-90 - 90) degrees
  double pitchAngle = atan(-accel[0] / (accel[1] * sin(rollAngle) + accel[2] * cos(rollAngle)));

  //Yaw Angle - about axis 2
  //  tan(yaw angle) = (mz * sin(roll) – my * cos(roll)) /
  //                   (mx * cos(pitch) + my * sin(pitch) * sin(roll) + mz * sin(pitch) * cos(roll))
  //  Use Atan2 to get our range in (-180 - 180)
  //
  //  Yaw angle == 0 degrees when axis 0 is pointing at magnetic north
  double yawAngle = atan2(magField[2] * sin(rollAngle) - magField[1] * cos(rollAngle),
    magField[0] * cos(pitchAngle) + magField[1] * sin(pitchAngle) * sin(rollAngle) + magField[2] * sin(pitchAngle) * cos(rollAngle));

  //double[] angles = { rollAngle, pitchAngle, yawAngle };

  double compassBearing = ((int) (yawAngle * (180.0 / PI) + 360)) % 360;

  return compassBearing;

  //we low-pass filter the angle data so that it looks nicer on-screen
  // try {
  //   //make sure the filter buffer doesn't have values passing the -180<->180 mark
  //   //Only for Roll and Yaw - Pitch will never have a sudden switch like that
  //   for (int i = 0; i < 3; i += 2) {
  //     if (abs(angles[i] - lastAngles[i]) > 3)
  //       for (double value in compassBearingFilter)
  //         if (angles[i] > lastAngles[i])
  //           value[i] += 360 * Math.PI / 180.0;
  //         else
  //           value[i] -= 360 * Math.PI / 180.0;
  //     }

  //   lastAngles = (double[])angles.Clone();

  //   compassBearingFilter.Add((double[])angles.Clone());
  //   if (compassBearingFilter.Count > compassBearingFilterSize)
  //     compassBearingFilter.RemoveAt(0);

  //   yawAngle = pitchAngle = rollAngle = 0;
  //   foreach (double[] stuff in compassBearingFilter) {
  //     rollAngle += stuff[0];
  //     pitchAngle += stuff[1];
  //     yawAngle += stuff[2];
  //   }
  //   yawAngle /= compassBearingFilter.Count;
  //   pitchAngle /= compassBearingFilter.Count;
  //   rollAngle /= compassBearingFilter.Count;

    //Convert radians to degrees for display
    //compassBearing = yawAngle * (180.0 / Math.PI);

    // bearingTxt.Text = compassBearing.ToString("F1") + "°";
    // pitchAngleTxt.Text = (pitchAngle * (180.0 / Math.PI)).ToString("F1") + "°";
    // rollAngleTxt.Text = (rollAngle * (180.0 / Math.PI)).ToString("F1") + "°";
  // } catch { }
}

} // namespace phidgets

