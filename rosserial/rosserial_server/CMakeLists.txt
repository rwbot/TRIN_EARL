cmake_minimum_required(VERSION 2.8.3)
project(rosserial_server)

find_package(catkin REQUIRED COMPONENTS roscpp rosserial_msgs topic_tools tf nav_msgs)
find_package(Boost REQUIRED COMPONENTS system thread)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp rosserial_msgs topic_tools
)

find_package(OpenCV)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


add_executable(${PROJECT_NAME}_man_drive src/man_drive.cpp)
target_link_libraries(${PROJECT_NAME}_man_drive ${catkin_LIBRARIES})
set_target_properties(${PROJECT_NAME}_man_drive PROPERTIES OUTPUT_NAME man_drive PREFIX "")
add_dependencies(${PROJECT_NAME}_man_drive rosserial_msgs_gencpp)


add_executable(${PROJECT_NAME}_man_drive_update src/man_drive_update.cpp)
target_link_libraries(${PROJECT_NAME}_man_drive_update ${catkin_LIBRARIES})
set_target_properties(${PROJECT_NAME}_man_drive_update PROPERTIES OUTPUT_NAME man_drive_update PREFIX "")
add_dependencies(${PROJECT_NAME}_man_drive_update rosserial_msgs_gencpp)


add_executable(${PROJECT_NAME}_serial_test src/serial_test.cpp)
target_link_libraries(${PROJECT_NAME}_serial_test ${catkin_LIBRARIES})
set_target_properties(${PROJECT_NAME}_serial_test PROPERTIES OUTPUT_NAME serial_test PREFIX "")
add_dependencies(${PROJECT_NAME}_serial_test rosserial_msgs_gencpp)

add_executable(${PROJECT_NAME}_serial_node src/serial_node.cpp)
target_link_libraries(${PROJECT_NAME}_serial_node ${catkin_LIBRARIES})
set_target_properties(${PROJECT_NAME}_serial_node PROPERTIES OUTPUT_NAME serial_node PREFIX "")
add_dependencies(${PROJECT_NAME}_serial_node rosserial_msgs_gencpp)

add_executable(${PROJECT_NAME}_socket_node src/socket_node.cpp)
target_link_libraries(${PROJECT_NAME}_socket_node ${catkin_LIBRARIES})
set_target_properties(${PROJECT_NAME}_socket_node PROPERTIES OUTPUT_NAME socket_node PREFIX "")
add_dependencies(${PROJECT_NAME}_socket_node rosserial_msgs_gencpp)

add_executable(${PROJECT_NAME}_ros_odometry src/ros_odometry.cpp)
target_link_libraries(${PROJECT_NAME}_ros_odometry ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
set_target_properties(${PROJECT_NAME}_ros_odometry PROPERTIES OUTPUT_NAME ros_odometry PREFIX "")
add_dependencies(${PROJECT_NAME}_ros_odometry rosserial_msgs_gencpp)

add_executable(test_drive src/test_drive.cpp)
target_link_libraries(test_drive ${catkin_LIBRARIES})
set_target_properties(test_drive PROPERTIES OUTPUT_NAME test_drive PREFIX "")
add_dependencies(test_drive rosserial_msgs_gencpp)




## Declare a C++ executable
# add_executable(man_drive2 src/man_drive2.cpp)
## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(man_drive2 ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
## Specify libraries to link a library or executable target against
# target_link_libraries(man_drive2 ${catkin_LIBRARIES})


# add_executable(xbox_drive src/xbox_drive.cpp)
## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(xbox_drive ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
## Specify libraries to link a library or executable target against
# target_link_libraries(xbox_drive ${catkin_LIBRARIES})

install(
  TARGETS ${PROJECT_NAME}_socket_node ${PROJECT_NAME}_serial_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
  DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
