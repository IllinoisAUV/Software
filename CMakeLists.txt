cmake_minimum_required(VERSION 2.8.3)
project(robosub)

## By default no optimization or debug flags are set use the following
## two lines to pass in these flags. Only use one at a time!
#set(CMAKE_BUILD_TYPE Debug) #Uncomment this line to enable debug symbols
#set(CMAKE_BUILD_TYPE Release) #Uncomment this for max optimization

#enable C++ 11 support
set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++11 ${CMAKE_CXX_FLAGS}")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  mavros
  mavros_msgs
  cv_bridge
  image_transport
  sensor_msgs
  roslaunch
  tf
  actionlib
  actionlib_msgs
)

catkin_python_setup()

# Check all files in the launch folder for common mistakes
roslaunch_add_file_check(launch)

find_package(OpenCV REQUIRED)

add_message_files(
  FILES
  VisualTarget.msg
)

add_action_files(
  DIRECTORY action
  FILES
  BaseVision.action
  Dive.action
  Gate.action
  HitDie.action
  Pinger.action
  Roulette.action
)


generate_messages(
  DEPENDENCIES
  std_msgs
  mavros_msgs
  sensor_msgs
  actionlib_msgs
)

catkin_package(
  INCLUDE_DIRS src
  CATKIN_DEPENDS roscpp rospy std_msgs mavros mavros_msgs cv_bridge image_transport sensor_msgs tf actionlib actionlib_msgs
)

include_directories(
  src
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

## Mark executable scripts (Python etc.) for installation
install(PROGRAMS
  bin/heartbeat.sh
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)


# Macros for declaring ros nodes and libraries 
include(cmake/ros-node.cmake)

# Constructors declares libraries that are convenient for building ros messages
add_subdirectory(src/constructors)

# motion declares some libraries, and must come before any directory that uses it
add_subdirectory(src/motion)

add_subdirectory(src/preprocess)
add_subdirectory(src/utilities)
add_subdirectory(src/experimental)

# actions defines a set of action servers that can be used executed by the sub
add_subdirectory(src/actions)

# transform defines a set of nodes that convert one message type to another 
add_subdirectory(src/transform)
add_subdirectory(src/task_manager)
add_subdirectory(src/simulation)

include(cmake/cppcheck.cmake)
include(cmake/clang-format.cmake)
include(cmake/pylint.cmake)
