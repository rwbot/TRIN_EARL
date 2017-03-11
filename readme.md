Current developers: Jin Pyo Jeon (jinpyojeon@trincoll.edu), Basileal Imana (basileal.imana@trincoll.edu), Barok Imana (Barok.imana@trincoll.edu)
Previous developers: Mike Castellana

Note that this file is formatted as Github flavored markdown.

Please read the style guide for ROS to allow for some coherence in our project

http://wiki.ros.org/CppStyleGuide

How to work on the IGVC remotely and on windows
--------------------------
1. Google 'bash on windows'
2. SSH into the machine using the command 'ssh -x {username}@{ip}' with the username and ip 
3. Use of tmux (way to have multiple terminal windows in one) is recommended for ssh.
4. Another recommended thing is to enable 24-bit color on the bash on windows. Do this by opting for Insiders Version for now.

Pre-reqs for compiling
==============================
**Honestly, getting it to compile on a different machine is a pain, so I would recommend one to just ssh into the machine.**

* Install Eigen library 
1. Install tar.bz2 from website and follow INSTALL instruction)
2. If Cmake error, find FindEigen3.cmake file and put in it appropriate place, then rename it back to FindEigen.cmake
* (Look inside the cmake folder somewhere within the compiled library folder and paste the cmake files into the catkin folder of the package)
* Install orocos-bfl (sudo apt-get install ros-jade-navigation) 
* Install dependencies for openslam-gmapping (sudo apt-get install ros-jade-openslam-gmapping)
* Install popt.h (sudo apt-get install libpopt-dev)
* Install xbox drivers
 1.sudo apt-add-repository -y ppa:rael-gc/ubuntu-xboxdrv
 2. sudo apt-get update
 3. sudo apt-get install ubuntu-xboxdrv
 4. sudo apt-get install --install-recommends jstest* joystick xboxdrv


Guidelines for compiling using catkin
---------------------
1. When removing package for catkin_make, either put CATKIN_IGNORE file in the package folder or 
2. (Preferred) put it into the donotbuild folder to make it more clear
3. Usually, cmake errors are accompanied by compile error message above the terminal ... reference that to solve the issue
4. You can build just one package using catkin_make --pkg {pkg_name} 
5. Another option is --from-pkg <package> - it builds package first then others
6. When having issues with commands like roscd and rosrun, make sure to have run source ~/catkin_ws/devel/setup.bash

USB/Port access
---------------
1. Modify the /etc/local file. 
2. Currently, the user have been given default rw access to the several ports.
3. Use lsusb to list all USB devices (to test the connections)
4. ? Run python -m serial.tools.list_ports to see available ports (may be incomplete)
5. ? Run python -m serial.tools.miniterm

Two-webcam setup
-------------------
(I dont know how exactly I got it to work, but here are some things I tried...)

1. sudo apt-get install v4l-utils
2. sudo rmmod uvcvideo
3. sudo modprobe uvcvideo quirks=128
4. rosdep install(?) usb_cam
5. sudo apt-get install ros-jade-usb-cam
6. Add serial numbers to the rules, etc
 

Connecting ROS to Arduino
------------------------
sudo apt-get install ros-indigo-rosserial-arduino
sudo apt-get install ros-indigo-rosserial


fovis_ros 
-------------------------
Fovis_ros consists of fovis_ros and libfovis ROS package

* Install libuvc here : https://int80k.com/libuvc/doc/
* Install libgps = sudo apt-get install libgps-dev

Warnings
----------
* Dont install ubuntu-xboxdrv (install just xboxdrv)


Optional
------------
* sudo apt-get install ros-jade-imu-tools
* source devel/setup.bash
1. git clone (git directory)
2. rosdep imu_tools 

* in .bashrc, put:
	* source /opt/ros/jade/setup.bash
	* source /catkin_ws/install/setup.bash
* Run /catkin_ws/devel/setup.bash for good measure

General guidelines
------------------
* Choose python than C++ unless the task is VERY computationally expensive. 


Shortcuts 
-----------------------------------

Ctrl + K  - (Subl) Indents files automatically (if setup so)

Teleoperation
-------------------
* roslaunch turtlebot_teleop keyboard_teleop.launch --screen
* rosrun turtlesim turtle_teleop_key /turtle1/cmd_vel:=/mybot/cmd_vel
* rosrun turtlebot_teleop turtlebot_teleop_key /turtlebot_teleop/cmd_vel:=/mybot/cmd_vel

GPS
-----------------------------
rosrun nmea_navsat_driver nmea_serial_driver _port=/dev/ttyUSB0 

Running the robot
------------------------
launch the earl.launch file in igvc/src

Run if file change 
------------------
catkin_make --pkg rosserial_server 


Man_drive location: ~/catkin_ws/src/rosserial/rosserial_server/src/man_drive_update.cpp

Run commands 
------------
roslaunch rosserial_server man_drive.launch
sudo xboxdrv --silent 
-== sudo rmmod xpad
password: Contact jinpyojeon@trincoll.edu / Jin

Arduino
-----------
The Arduino code is in the catkin_ws as well.

ROS Commands
----------------

rostopic echo /topic_name
rostopic pub my_topic std_msgs/String "hello there"

Transforms
----------------------------
rosrun tf tf_echo turtle1 turtle2
rosrun tf view_frames
rosrun tf tf_monitor turtle1 turtle2
rosrun rqt_graph rqt_graph

Setting up the Jetsons and Connecting to Camera
----------------------
######For serial - try this if ssh is not setup due to system failure
1. Refer https://demotomohiro.github.io/hardware/jetson_tk1/setup/serial.html
2. sudo screen /dev/ttyUSB0 115200
3. You can also try with the more-featured minicom program if you need more features (replace screen with minicom)
4. The USB-serial cable that we have need a 'gender changer' or adapter

######If connected via network
1. ssh -X ubuntu@Jetson1
2. ssh -X ubuntu@Jetson2

3. Run *source devel/setup.bash* on each (or, better, put those commands in ~/.bashrc if not already)

4. roslaunch image_transport_package steam.launch

Emergency Lights
-----------------
The lights will be attached to the arduino, 
with its cables attached to port 13 (signal), power 3.3v, and ground.
Its functions are handled in the multiplexer.py

Launching Phidgets
----------------------

roslaunch phidgets_imu imu.launch

Launch file 
----------
ns  => stands for namespace to launch the node in

Other command line commands
--------------
lsusb - lists usb connections 
(can be used to check if Kinect is properly powered and connected, etc.)

GPS
-----------
rosrun nmea_navsat_driver nmea_serial_driver _port=/dev/ttyUSB0 

Camera calibration
------------------

Refer: http://wiki.ros.org/camera_calibration

The camera calibration file is stored in /.ros/camera_info <=required for rectification
 
rosrun camera_calibration cameracalibrat.py --size 7x6 --square 0.0121 right:=/stereo/right/image_raw left:=/stereo/left/image_raw right_camera:=/stereo/right left_camera:=/stereo/left --approximate=0.1

Size (8x6) refers to the corners where four corners meet
Square refers to the size of the squares in meters

-Test image calibration routinely using the checkerboard and looking at the disparity image 

Image view
-------------
rosrun image_view stereo_view stereo:=stereo image:=image_rect _queue_size:=1000 _approximate_sync:=True

Things tested (to be) with Gazebo Sim
-----------------------------------
* motor_controller.cpp & odom transform (diff_tf.py)
* integration of sensors (EKF) and its transform of odometry
* path planning variables in launch (nav stack)
* transforms (also in launch)
* speed and acceleration restrictions (i.e. motor controller options)
* line detection (with camera feed) 
  * OpenCV test
* Turn & obstacles (e.g. if line is exited)

#### What is not tested
* Actual, physical sensors
* Motor controller functions

Working in Gazebo 
--------------------

* Go to here for the shortcuts: http://gazebosim.org/hotkeys
* Sending commands (through topic): Use rosrun rqt_gui rqt_gui 
	- See http://gazebosim.org/tutorials?tut=ros_control&cat=connect_ros
* The Gazebo plugin directory is ~/gazebo_plugins


TODOS
-----------------
* https://www.youtube.com/watch?v=LzyMQVSJvzY
	* =Forbot?? point distance?? 
	* =Seam time
* Look into dynamic reconfigure for simulation variable setting

