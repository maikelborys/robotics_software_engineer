#include <geometry_msgs/msg/twist.hpp>
#include <iostream>
#include <memory>
#include <nav_msgs/msg/odometry.hpp>
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include <rclcpp/rclcpp.hpp>
#include <vector>
#include <utility>

class GoalPlaner : public rclcpp::Node {
public:
  GoalPlaner() : Node("Goal_Planer"), goal_selected_(false) {
    // Removed individual goal parameters.
    // Initialize candidate goals: (x, y) pairs.
    candidate_goals_ = {
      {8.0, 3.0},
      {2.0, 2.0},
      {5.0, 5.0}
    };
    this->declare_parameter<double>("kp_angle", 0.5);
    this->declare_parameter<double>("kp_distance", 0.5);
    publisher_ =
        this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    subscription_ = this->create_subscription<nav_msgs::msg::Odometry>(
        "odom", 10,
        std::bind(&GoalPlaner::odom_callback, this, std::placeholders::_1));
  }

private:
  double get_yaw_from_quaternion(const geometry_msgs::msg::Quaternion &quat) {
    tf2::Quaternion q(quat.x, quat.y, quat.z, quat.w);
    tf2::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    return yaw;
  }

  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr tb3_odom_msg) {
    auto cmd_msg = geometry_msgs::msg::Twist();
    double Kp_angle = this->get_parameter("kp_angle").as_double();
    double Kp_distance = this->get_parameter("kp_distance").as_double();

    double robot_x = tb3_odom_msg->pose.pose.position.x;
    double robot_y = tb3_odom_msg->pose.pose.position.y;
    double yaw = get_yaw_from_quaternion(tb3_odom_msg->pose.pose.orientation);

    // Select goal based on minimal energy (i.e. shortest distance)
    if (!goal_selected_) {
      double min_distance = std::numeric_limits<double>::max();
      for (const auto &goal : candidate_goals_) {
        double dx = goal.first - robot_x;
        double dy = goal.second - robot_y;
        double distance = sqrt(pow(dx, 2) + pow(dy, 2));
        if (distance < min_distance) {
          min_distance = distance;
          selected_goal_x_ = goal.first;
          selected_goal_y_ = goal.second;
        }
      }
      goal_selected_ = true;
      RCLCPP_INFO(this->get_logger(), "Selected Goal: (%f, %f)", selected_goal_x_, selected_goal_y_);
    }

    // Use the selected goal for planning
    float error_x = selected_goal_x_ - robot_x;
    float error_y = selected_goal_y_ - robot_y;
    double error_in_distance = sqrt(pow(error_x, 2) + pow(error_y, 2));
    double error_in_angle = atan2(error_y, error_x) - yaw;
    error_in_angle = atan2(sin(error_in_angle), cos(error_in_angle));
    
    RCLCPP_INFO(this->get_logger(), "E_D : %f E_A : %f", error_in_distance, error_in_angle);

    error_in_distance = std::max(0.0, std::min(error_in_distance, 1.0));
    error_in_angle    = std::max(-1.0, std::min(error_in_angle, 1.0));

    if(error_in_distance > 0.1) {
        cmd_msg.linear.x = Kp_distance * error_in_distance;
        cmd_msg.angular.z = Kp_angle * error_in_angle;
    } else {
        RCLCPP_INFO(this->get_logger(), "Goal Reached");
        cmd_msg.linear.x = 0.0;
        cmd_msg.angular.z = 0.0;
    }

    publisher_->publish(cmd_msg);
  }

  std::vector<std::pair<double, double>> candidate_goals_;
  double selected_goal_x_;
  double selected_goal_y_;
  bool goal_selected_;

  double error = 0;
  const double THRESHOLD = 0.01;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<GoalPlaner>());
  rclcpp::shutdown();
  return 0;
}