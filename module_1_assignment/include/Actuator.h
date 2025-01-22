#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <string>
#include <iostream>

class Actuator {
public:
    Actuator(const std::string& name);
    std::string getName() const; // Agregué `const` para garantizar que no se modifique el estado.
protected:
    std::string name;
};

class ArmActuator : public Actuator {
public:
    ArmActuator(const std::string& name);
    void move(int angle);
};

class WheelActuator : public Actuator {
public:
    WheelActuator(const std::string& name);
    void rotate(int degrees);
};

#endif // ACTUATOR_H
