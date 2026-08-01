#include <thread>
#include <string>
#include <iostream>
#include <random>
#include <chrono>
#include <mutex>

int globalThreadCnt = 0;
std::mutex mtx;

struct RobotState
{
    double temperature{20.0};
    double motor_speed{0.0};
    int sensor_updates{0};
    int motor_updates{0};
};

// Double type random generator implementation
// double genRandDouble(){
//   const long max_rand = 1000000L;
//   double lower_bound = 50;
//   double upper_bound = 100;

//   srandom(time(NULL));

//   return lower_bound + (upper_bound - lower_bound) * (random() % max_rand) / max_rand;
// }
// Better solution
double genRandDouble(double lowerBound, double upperBound){
  static std::mt19937 generator{std::random_device{}()};
  std::uniform_real_distribution<double> distribution{lowerBound, upperBound};

  return distribution(generator);
}

void simTemp(RobotState *robot){
  // int randomNum = rand() % 101; random int between 0-100
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // period

    mtx.lock();
    globalThreadCnt++;

    robot->temperature = genRandDouble(50.0, 100.0);
    std::cout << "[sensor] " << "thread=" << globalThreadCnt << " new temperature=" << robot->temperature << std::endl;
    robot->sensor_updates += 1;

    mtx.unlock();
    }
  
}

void simMotor(RobotState *robot){
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // period

    mtx.lock();
    globalThreadCnt++;

    std::cout << "[motor] " << "thread=" << globalThreadCnt << " starting slow motor calculation" << std::endl;

    mtx.unlock(); // unlock so that the sleep does not block other threads by holding the mutex access key
    std::this_thread::sleep_for(std::chrono::milliseconds(700)); // blocking 
    mtx.lock(); // ensure to lock since we access robot object and proceed to print

    robot->motor_speed = genRandDouble(0.1, 9.9);
    std::cout << "[motor] " << "thread=" << globalThreadCnt << " finished, speed=" << robot->motor_speed << std::endl;
    robot->motor_updates += 1;

    mtx.unlock();
  }
}

// std::string getStatus(){

// }

void simStatus(RobotState *robot){
  while(1){
    std::this_thread::sleep_for(std::chrono::milliseconds(250)); // period

    mtx.lock();
    globalThreadCnt++;

    std::cout << "[status] " << "thread=" << globalThreadCnt << " temp=" << robot->temperature;
    std::cout << " temp_updates=" << robot->sensor_updates << " speed=" << robot->motor_speed << " motor_updates=" << robot->motor_updates << std::endl;

    mtx.unlock();
  }
}

int main()
{
  RobotState harrison;

  std::thread t1_sensor(simTemp, &harrison);
  std::thread t2_motor(simMotor, &harrison);
  std::thread t3_status(simStatus, &harrison);

  t1_sensor.join(); // Safely blocks until function finishes
  t2_motor.join();
  t3_status.join();

  return 0;
}