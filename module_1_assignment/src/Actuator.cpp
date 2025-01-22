#include "../include/Actuator.h"

// Base class Actuator
Actuator::Actuator(const std::string& name) : name(name) {}

std::string Actuator::getName() const {
    return name;
}

// Derived class ArmActuator
ArmActuator::ArmActuator(const std::string& name) : Actuator(name) {}

void ArmActuator::move(int angle) {
    std::cout << "Moving " << name << " to angle: " << angle << " degrees." << std::endl;
}

// Derived class WheelActuator
WheelActuator::WheelActuator(const std::string& name) : Actuator(name) {}

void WheelActuator::rotate(int degrees) {
    std::cout << "Rotating " << name << " by " << degrees << " degrees." << std::endl;
}
