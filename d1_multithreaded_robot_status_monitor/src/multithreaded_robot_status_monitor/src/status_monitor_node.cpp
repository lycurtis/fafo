// PART 3: MultiThreaded

/*
MultiThreadedExecutor  = has multiple worker threads
Callback groups        = who is allowed to run at the same time
Mutex                  = still protects shared harrison_ data
*/

#include "rclcpp/rclcpp.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <mutex>

#define simTempPeriod 500
#define simMotorPeriod 1000
#define simStatusPeriod 250

#define simMotorActionBlockingPeriod 700

class StatusMonitorNode : public rclcpp::Node // inherits from rclcpp::Node class
{
public:
  StatusMonitorNode() : Node("status_monitor"){ // constructor Node("node_name")
    // Initialize publishers, subscribers, or timers here

    // Create a Reentrant callback group to allow concurrent execution
    callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

    // Create a wall timer firing every specified Period
    sensor_timer_ = this->create_wall_timer(
        std::chrono::milliseconds(simTempPeriod), 
        std::bind(&StatusMonitorNode::timer_callback_simTemp, this),
        callback_group_
    );
    motor_timer_ = this->create_wall_timer(
        std::chrono::milliseconds(simMotorPeriod), 
        std::bind(&StatusMonitorNode::timer_callback_simMotor, this),
        callback_group_
    );
    status_timer_ = this->create_wall_timer(
        std::chrono::milliseconds(simStatusPeriod), 
        std::bind(&StatusMonitorNode::timer_callback_simStatus, this),
        callback_group_
    );

  }


private:
  // Declare member variables, publishers, subscriptions, or timers here
  int globalThreadCnt_ = 0;
  std::mutex mtx_;

  // callback group member variable (who is allowed to run at the same time)
  rclcpp::CallbackGroup::SharedPtr callback_group_;

  // Timer member variable
  rclcpp::TimerBase::SharedPtr sensor_timer_;
  rclcpp::TimerBase::SharedPtr motor_timer_;
  rclcpp::TimerBase::SharedPtr status_timer_;

  struct RobotState{
    double temperature{20.0};
    double motor_speed{0.0};
    int sensor_updates{0};
    int motor_updates{0};
  };

  RobotState harrison_;

  double genRandDouble(double lowerBound, double upperBound){
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{lowerBound, upperBound};
  
    return distribution(generator);
  }

  // The callback function
  void timer_callback_simTemp(){
    mtx_.lock();
    globalThreadCnt_++;

    harrison_.temperature = genRandDouble(50.0, 100.0);
    RCLCPP_INFO(
        this->get_logger(), 
        "[sensor] thread=%d new temperature=%.1f",
        globalThreadCnt_,
        harrison_.temperature
    );
    harrison_.sensor_updates += 1;

    mtx_.unlock();
  }

  void timer_callback_simMotor(){
    mtx_.lock();
    globalThreadCnt_++;

    RCLCPP_INFO(
        this->get_logger(),
        "[motor] thread=%d starting slow motor calculation",
        globalThreadCnt_
    );

    mtx_.unlock();

    std::this_thread::sleep_for(std::chrono::milliseconds(simMotorActionBlockingPeriod)); // blocking 

    mtx_.lock();

    harrison_.motor_speed = genRandDouble(0.1, 9.9);
    RCLCPP_INFO(
        this->get_logger(),
        "[motor] thread=%d finished, speed=%.1f",
        globalThreadCnt_,
        harrison_.motor_speed
    );
    harrison_.motor_updates += 1;

    mtx_.unlock();
  }

  void timer_callback_simStatus(){
    mtx_.lock();
    globalThreadCnt_++;

    RCLCPP_INFO(
        this->get_logger(),
        "[status] thread=%d temp=%.1f temp_updates=%d speed=%.1f motor_updates=%d",
        globalThreadCnt_,
        harrison_.temperature,
        harrison_.sensor_updates,
        harrison_.motor_speed,
        harrison_.motor_updates
    );

    mtx_.unlock();
  }

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<StatusMonitorNode>();

  // Instantiate the MultiThreadedExecutor
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 0); // passing 0 means pick a default thread count

  executor.add_node(node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
