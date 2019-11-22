#!/bin/bash

MAP_FILE=${HOME}/hoshina_ws/map/3_kai_small

source ${HOME}/catkin_ws/devel/setup.bash
source ${HOME}/hoshina_ws/devel/setup.bash
gnome-terminal -- bash -c "roslaunch ypspur_practice orne_r.launch" 
sleep 4
gnome-terminal -- bash -c "roslaunch ypspur_practice localization.launch map_file:=${MAP_FILE}"
sleep 3
gnome-terminal -- bash -c "rosrun rviz rviz"
