cmake_minimum_required(VERSION 3.8)
project(multithreaded_robot_status_monitor)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

# Each .cpp has its own main — build them as separate executables
add_executable(status_monitor_node src/status_monitor_node.cpp)
add_executable(status_monitor_node_single_thread src/status_monitor_node_single_thread.cpp)
add_executable(status_monitor_threads src/status_monitor_threads.cpp)

ament_target_dependencies(status_monitor_node rclcpp std_msgs)
ament_target_dependencies(status_monitor_node_single_thread rclcpp std_msgs)
ament_target_dependencies(status_monitor_threads rclcpp std_msgs)

install(TARGETS
  status_monitor_node
  status_monitor_node_single_thread
  status_monitor_threads
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
