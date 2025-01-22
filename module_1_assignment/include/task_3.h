#ifndef TASK_3_H
#define TASK_3_H

#include <iostream>
using namespace std;

// Template class for general sensor
template <typename T>
class Sensor {
private:
    T data;
public:
    Sensor(T data) : data(data) {}
    void readData() {
        cout << "Sensor reading: " << data << endl;
    }
};

// TemperatureSensor class
class TemperatureSensor : public Sensor<double> {
public:
    TemperatureSensor(double temp) : Sensor(temp) {
        cout << "Temperature sensor initialized with value: " << temp << endl;
    }
};

// DistanceSensor class
class DistanceSensor : public Sensor<double> {
public:
    DistanceSensor(double distance) : Sensor(distance) {
        cout << "Distance sensor initialized with value: " << distance << endl;
    }
};

#endif // TASK_3_H
