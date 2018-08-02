#!/bin/bash

# Script that starts the submarine. Should be called by hand or on boot by
# systemd

set -x

# Remove old files if they exist
source /opt/ros/kinetic/setup.bash
source ~/catkin_ws/devel/setup.bash
sudo rm /home/ubuntu/l4t_dfs.conf

# Maximize the jetson's performance
set -e
sudo /home/ubuntu/jetson_clocks.sh --store
trap "sudo /home/ubuntu/jetson_clocks.sh --restore" EXIT

# Add all necessary launch files here
roslaunch robosub motion.launch &
sleep 5
roslaunch robosub zed.launch &
# roslaunch robosub bottom-camera.launch &

wait
