cmake_minimum_required(VERSION 3.0.2)
project(tmr_test)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
#add_message_files(FILES Num.msg)
#add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(pointGenerator src/pointGenerator.cpp)
target_link_libraries(pointGenerator ${catkin_LIBRARIES})
add_dependencies(pointGenerator tmr_test_generate_messages_cpp)


add_executable(pointAnalyzer src/pointAnalyzer.cpp)
target_link_libraries(pointAnalyzer ${catkin_LIBRARIES})
add_dependencies(pointAnalyzer tmr_test_generate_messages_cpp)

