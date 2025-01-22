#include "../include/task_3.h"

int main() {
    // Create a temperature sensor with double data
    TemperatureSensor tempSensor(25.5);
    tempSensor.readData();

    // Create a distance sensor with double data
    DistanceSensor distSensor(555.0);
    distSensor.readData();

    // Using the template class for different data types
    Sensor<string> stringSensor("SensorStringData");
    stringSensor.readData();

    Sensor<char> charSensor('A');
    charSensor.readData();

    return 0;
}
