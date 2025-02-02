// Purpose:
// - Navigate a robot through a maze using LIDAR data
// - Make decisions based on obstacle proximity to maneuver through the maze
// Author: Robotisim

#include <geometry_msgs/msg/twist.hpp>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/imu.hpp>

enum class RobotState {
  MOVING_STRAIGHT,
  TURNING_LEFT,
  TURNING_RIGHT,
  OUT_OF_MAZE
};

class MazeSolving : public rclcpp::Node {
public:
  MazeSolving() : Node("maze_solving"), _state(RobotState::MOVING_STRAIGHT) {
    _publisher =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    _subscription = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 10,
        std::bind(&MazeSolving::lidarCallback, this, std::placeholders::_1));
    _imu_subscription = this->create_subscription<sensor_msgs::msg::Imu>(
        "/imu", 10,
        std::bind(&MazeSolving::imuCallback, this, std::placeholders::_1));
  }

private:
  void lidarCallback(const sensor_msgs::msg::LaserScan::SharedPtr lidarMsg) {
    if (lidarMsg->ranges.empty()) {
      RCLCPP_WARN(this->get_logger(), "LIDAR data is empty");
      return;
    }
    // Analyze LIDAR data to navigate through the maze
    float rightObstacle = *std::min_element(lidarMsg->ranges.begin() + 260,
                                            lidarMsg->ranges.begin() + 280);
    float frontObstacle = *std::min_element(lidarMsg->ranges.begin() + 340,
                                            lidarMsg->ranges.begin() + 360);
    float leftObstacle = *std::min_element(lidarMsg->ranges.begin() + 80,
                                           lidarMsg->ranges.begin() + 100);

    // Log obstacle distances
    RCLCPP_INFO(this->get_logger(), "Front: %f, Right: %f, Left: %f,",
                frontObstacle, rightObstacle, leftObstacle);

    // Determine robot's state based on obstacle proximity
    geometry_msgs::msg::Twist command;  // Declare command variable here
    switch (_state) {
    case RobotState::MOVING_STRAIGHT:
      if (frontObstacle < _frontThreshold) {
        if (leftObstacle < rightObstacle) {
          _state = RobotState::TURNING_RIGHT;
        } else {
          _state = RobotState::TURNING_LEFT;
        }
      } else if (rightObstacle < _sideThreshold) {
        // Adjust to follow the right wall
        command.linear.x = _linearVel;
        command.angular.z = -_angularVel / 2;
      } else {
        command.linear.x = _linearVel;
        command.angular.z = 0.0;
      }
      break;
    case RobotState::TURNING_LEFT:
    case RobotState::TURNING_RIGHT:
      if (frontObstacle > _frontThreshold) {
        _state = RobotState::MOVING_STRAIGHT;
      }
      break;
    case RobotState::OUT_OF_MAZE:
      RCLCPP_INFO(this->get_logger(), "Robot is out of the maze");
      break;
    }

    // Adjust robot's motion based on the state
    switch (_state) {
    case RobotState::MOVING_STRAIGHT:
      command.linear.x = _linearVel;
      command.angular.z = 0.0;
      RCLCPP_INFO(this->get_logger(), "Moving straight");
      break;
    case RobotState::TURNING_LEFT:
      command.linear.x = 0.0;
      command.angular.z = _angularVel;
      RCLCPP_INFO(this->get_logger(), "Turning left");
      break;
    case RobotState::TURNING_RIGHT:
      command.linear.x = 0.0;
      command.angular.z = -_angularVel;
      RCLCPP_INFO(this->get_logger(), "Turning right");
      break;
    case RobotState::OUT_OF_MAZE:
      command.linear.x = 0.0;
      command.angular.z = 0.0;
      RCLCPP_INFO(this->get_logger(), "Stopped");
      break;
    }

    // Publish the command
    _publisher->publish(command);
  }

  void imuCallback(const sensor_msgs::msg::Imu::SharedPtr imuMsg) {
    // Calculate velocity and acceleration from IMU data
    auto linear_acceleration = imuMsg->linear_acceleration;
    auto angular_velocity = imuMsg->angular_velocity;

    RCLCPP_INFO(this->get_logger(), "Linear Acceleration: x=%f, y=%f, z=%f",
                linear_acceleration.x, linear_acceleration.y, linear_acceleration.z);
    RCLCPP_INFO(this->get_logger(), "Angular Velocity: x=%f, y=%f, z=%f",
                angular_velocity.x, angular_velocity.y, angular_velocity.z);
  }

  float _frontThreshold = 1.5f;
  float _sideThreshold = 0.5f;  // New threshold for side obstacles
  float _angularVel = 0.8f;
  float _linearVel = 0.6f;
  RobotState _state;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr _publisher;
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr _subscription;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr _imu_subscription;  // IMU subscription
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MazeSolving>());
  rclcpp::shutdown();
  return 0;
}
