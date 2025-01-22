#include <iostream>
#include <vector>
using namespace std;

// Namespace for Robot 1. CORRECTED BY GPT, I was hesitating if it correctly to implemate namespace in that way, but all code was written by me. Thanks
namespace Robot1 {
    class Robot {
    public:
        Robot(string name, int speed, int weight, string size, int numberOfSensors) {
            this->name = name;
            this->speed = speed;
            this->weight = weight;
            this->size = size;
            this->numberOfSensors = numberOfSensors;
            robotDescription();
        }

        void moveForward() {
            cout << name << " is moving forward at " << speed << " m/s." << endl;
        }

        void moveBackward() {
            cout << name << " is moving backward at " << speed << " m/s." << endl;
        }

        void stop() {
            cout << name << " is stopping." << endl;
        }

    private:
        string name;
        int speed;
        int weight;
        string size;
        int numberOfSensors;

        void robotDescription() {
            cout << "Robot name: " << name << endl;
            cout << "Robot speed: " << speed << " m/s" << endl;
            cout << "Robot weight: " << weight << " kg" << endl;
            cout << "Robot size: " << size << endl;
            cout << "Number of sensors: " << numberOfSensors << endl;
            cout << "----------------------------------------" << endl;
        }
    };
}

// Namespace for Robot 2 (can be expanded with more robots)
namespace Robot2 {
    class Robot {
    public:
        Robot(string name, int speed, int weight, string size, int numberOfSensors) {
            this->name = name;
            this->speed = speed;
            this->weight = weight;
            this->size = size;
            this->numberOfSensors = numberOfSensors;
            robotDescription();
        }

        void moveForward() {
            cout << name << " is moving forward at " << speed << " m/s." << endl;
        }

        void moveBackward() {
            cout << name << " is moving backward at " << speed << " m/s." << endl;
        }

        void stop() {
            cout << name << " is stopping." << endl;
        }

    private:
        string name;
        int speed;
        int weight;
        string size;
        int numberOfSensors;

        void robotDescription() {
            cout << "Robot name: " << name << endl;
            cout << "Robot speed: " << speed << " m/s" << endl;
            cout << "Robot weight: " << weight << " kg" << endl;
            cout << "Robot size: " << size << endl;
            cout << "Number of sensors: " << numberOfSensors << endl;
            cout << "----------------------------------------" << endl;
        }
    };
}

int main() {
    // Instantiate a Robot from Robot1 namespace
    Robot1::Robot explorer("Explorer", 50, 10, "Large", 5);
    explorer.moveForward();
    explorer.moveBackward();
    explorer.stop();

    // Instantiate a Robot from Robot2 namespace
    Robot2::Robot defender("Defender", 40, 15, "Medium", 8);
    defender.moveForward();
    defender.moveBackward();
    defender.stop();

    return 0;
}
