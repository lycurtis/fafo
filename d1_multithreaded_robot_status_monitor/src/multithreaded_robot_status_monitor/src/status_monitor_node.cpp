#include <thread>
#include <string>
#include <iostream>
#include <random>
#include <chrono>

int globalThreadCnt = 0;

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

// std::string getStatus(){

// }

int main()
{
  RobotState harrison;

  std::thread t1_sensor(simTemp, harrison);
  std::thread t2_motor(simMotor, harrison);

  t1_sensor.join(); // Safely blocks until function finishes
  t2_motor.join();

  return 0;
}