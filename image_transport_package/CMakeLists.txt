cmake_minimum_required(VERSION 2.8)
project(image_transport_package)

find_package(catkin REQUIRED cv_bridge genmsg image_transport sensor_msgs)

# add the resized image message
add_message_files(DIRECTORY msg
   FILES ResizedImage.msg
)
generate_messages(DEPENDENCIES sensor_msgs)

catkin_package()

find_package(OpenCV)

include_directories(include ${OpenCV_INCLUDE_DIRS})

# add the publisher example
add_executable(my_publisher src/my_publisher.cpp)
target_link_libraries(my_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

# add the subscriber example
add_executable(my_subscriber src/my_subscriber.cpp)
target_link_libraries(my_subscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

#add the subscriber test
add_executable(sub_test src/sub_test.cpp)
target_link_libraries(sub_test ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


#camera calibration
add_executable(cam_calibration src/camera_calibration.cpp)
target_link_libraries(cam_calibration ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

#line test
add_executable(line_test src/line_test.cpp)
target_link_libraries(line_test ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

#scan test
add_executable(scan_test src/scan_test.cpp)
target_link_libraries(scan_test ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})


