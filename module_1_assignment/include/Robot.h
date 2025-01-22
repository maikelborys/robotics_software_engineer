#ifndef ROBOT_H
#define ROBOT_H

#include <string>
#include "../include/Actuator.h"

class Robot {
public:
    Robot(const std::string& name, double speed);
    void activateActuator();
    void deactivateActuator(const Actuator& actuator); // Paso por referencia para evitar copias innecesarias.

private:
    std::string name;
    double speed;
};

#endif // ROBOT_H
