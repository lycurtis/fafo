# Multithreaded Robot Status Monitor

## Goal
Build one ROS2 C++ node that simulates three indepdent robot activities
- A **sensor timer** generates fake temperature readings every 500 ms
- A **motor timer** performs a deliberately slow "motor update" every 1 second
- A **status time** publishes or prints the latest combined robot status every 250 ms

ROS 2’s `MultiThreadedExecutor` uses a pool of threads to execute eligible callbacks concurrently. Callback groups control which callbacks are allowed to overlap: callbacks in a mutually exclusive group cannot run simultaneously, while callbacks in different groups can.

Package name:     multithreaded_robot_status_monitor
Executable name:  status_monitor_node
Source file:      status_monitor_node.cpp
Node name:        status_monitor
Class name:       StatusMonitorNode