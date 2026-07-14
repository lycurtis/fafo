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


## Building
```bash
source /opt/ros/humble/setup.bash
cd ~/fafo/d1_multithreaded_robot_status_monitor
colcon build --packages-select multithreaded_robot_status_monitor
source install/setup.bash
```
## Execute the Program
ros2 run <package_name> <node_name>
```bash
ros2 run multithreaded_robot_status_monitor status_monitor_node
```


## Gotchas

1. Realized that I cannot have print statements inside of the thread function
```cpp
void simTemp(RobotState robot){
  // int randomNum = rand() % 101; random int between 0-100
  globalThreadCnt++;
  robot.temperature = genRandDouble(50.0, 100.0);
  std::cout << "[sensor] " << "thread=" << globalThreadCnt << " new temperature=" << robot.temperature << std::endl;
}

void simMotor(RobotState robot){
  globalThreadCnt++;
  std::cout << "[motor] " << "thread=" << globalThreadCnt << "starting slow motor calculation" << std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(700));
  robot.motor_speed = genRandDouble(0.1, 9.9);
  std::cout << "[motor] " << "thread=" << globalThreadCnt << " finished, speed=" << robot.motor_speed << std::endl;
}
```

Result
```bash
[sensor] thread=2 new temperature=[motor] thread=2starting slow motor calculation
64.7864
[motor] thread=2 finished, speed=4.3959
```