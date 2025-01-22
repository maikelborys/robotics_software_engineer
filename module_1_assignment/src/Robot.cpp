#include "../include/Robot.h"
#include <iostream>

// Constructor
Robot::Robot(const std::string& name, double speed)
    : name(name), speed(speed) {}

// Activates an actuator
void Robot::activateActuator() {
    std::cout << "Activating all actuators for robot: " << name << "." << std::endl;
}

// Deactivates a specific actuator
void Robot::deactivateActuator(const Actuator& actuator) {
    std::cout << "Deactivating actuator: " << actuator.getName() << " for robot: " << name << "." << std::endl;
}
