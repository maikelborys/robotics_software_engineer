#include <iostream>
#include <vector>
using namespace std;


class Robot {
    public:
       
       Robot(int temperature, int distance) {
           this->distance = distance;
           this->temperature = temperature;
           sensorStatus(temperature, distance);
       }



        void sensorStatus(int temperature, int distance) {
            cout << "Temperature: " << temperature << " m/s" << endl;
            cout << "Distance: " << distance << " cm" << endl;
    
        }

    private:
        int temperature;
        int distance;
};

int main() {
    Robot myRobot(50, 10);

    return 0;
}

