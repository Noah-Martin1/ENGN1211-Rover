

#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/DistanceSensor.hpp>

#include <iostream>

using namespace webots;

#define TIME_STEP 32
#define MAX_SPEED 6.28

void waitSteps(Robot *robot, int ms) {
  int steps = ms / TIME_STEP;
  for (int i = 0; i < steps; i++) {
    robot->step(TIME_STEP);
  }
}

int main() {
  Robot *robot = new Robot();

  int timestep = (int)robot->getBasicTimeStep();

  //init motors
  Motor *leftMotor = robot->getMotor("left wheel motor");
  Motor *rightMotor = robot->getMotor("right wheel motor");

  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);

  leftMotor->setVelocity(0.0);
  rightMotor->setVelocity(0.0);
  
  //init sensors
  DistanceSensor *ps5 = robot->getDistanceSensor("ps5"); // left
  DistanceSensor *ps6 = robot->getDistanceSensor("ps6"); // front-left
  DistanceSensor *ps7 = robot->getDistanceSensor("ps7"); // front-left bias

  ps5->enable(TIME_STEP);
  ps6->enable(TIME_STEP);
  ps7->enable(TIME_STEP);

  while (robot->step(timestep) != -1) {

  //detect wall
    bool front_wall  = (ps7->getValue() > 80.0 || ps6->getValue() > 80.0);
    bool left_wall   = (ps5->getValue() > 80.0);
    bool left_corner = (ps6->getValue() > 100.0 && ps5->getValue() > 100.0);

    double left_speed = MAX_SPEED;
    double right_speed = MAX_SPEED;
    
    
    waitSteps(robot, 100); // wait to simulate sonar/servo scanning

    if (front_wall) {
      std::cout << "Turn right in place\n";
      left_speed = MAX_SPEED;
      right_speed = -MAX_SPEED;
    }

    else if (left_wall) {
      std::cout << "Drive forward\n";
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED;
    }

    else if (left_corner) {
      std::cout << "Came too close, drive right\n";
      left_speed = MAX_SPEED;
      right_speed = MAX_SPEED * 0.3;
    }

    else {
      std::cout << "Turn left\n";
      left_speed = MAX_SPEED * 0.3;
      right_speed = MAX_SPEED;
    }

    // move
    leftMotor->setVelocity(left_speed);
    rightMotor->setVelocity(right_speed);
  }

  delete robot;
  return 0;
}