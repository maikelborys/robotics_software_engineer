#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <vector>

class SensorDisplayNode : public rclcpp::Node
{
public:
  SensorDisplayNode() : Node("sensor_display_node")
  {
    odom_subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
      "/odom", 10, std::bind(&SensorDisplayNode::odom_callback, this, std::placeholders::_1));
    imu_subscriber_ = this->create_subscription<sensor_msgs::msg::Imu>(
      "/imu", 10, std::bind(&SensorDisplayNode::imu_callback, this, std::placeholders::_1));
    // New subscription for fused odometry data published by the EKF
    filtered_odom_subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
      "/odometry/filtered", 10, std::bind(&SensorDisplayNode::filtered_odom_callback, this, std::placeholders::_1));

    marker_publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("visualization_marker", 10);
  }

private:
  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
  {
    geometry_msgs::msg::Point pt;
    pt.x = msg->pose.pose.position.x;
    pt.y = msg->pose.pose.position.y;
    pt.z = msg->pose.pose.position.z;
    odom_points_.push_back(pt);
    latest_odom_ = *msg;

    // Publish raw odom data as a red line marker (LINE_STRIP)
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "odom";
    marker.header.stamp = this->get_clock()->now();
    marker.ns = "odom_raw";
    marker.id = 1;
    marker.type = visualization_msgs::msg::Marker::LINE_STRIP;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.scale.x = 0.05;
    marker.color.a = 1.0;
    marker.color.r = 1.0;
    marker.color.g = 0.0;
    marker.color.b = 0.0;
    marker.points = odom_points_;

    marker_publisher_->publish(marker);
  }

  void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg)
  {
    // Use latest odom position as base for the arrow marker
    geometry_msgs::msg::Point pt;
    if (!odom_points_.empty()) {
      pt = odom_points_.back();
    } else {
      pt.x = pt.y = pt.z = 0.0;
    }

    // Publish raw imu data as an arrow marker (blue) to show orientation
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "odom";
    marker.header.stamp = this->get_clock()->now();
    marker.ns = "imu_raw";
    marker.id = 2;
    marker.type = visualization_msgs::msg::Marker::ARROW;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose.position = pt;
    marker.pose.orientation = msg->orientation;
    marker.scale.x = 0.5; // shaft length
    marker.scale.y = 0.1; // shaft diameter
    marker.scale.z = 0.1; // head diameter
    marker.color.a = 1.0;
    marker.color.r = 0.0;
    marker.color.g = 0.0;
    marker.color.b = 1.0;

    marker_publisher_->publish(marker);
  }

  // New callback to display fused output
  void filtered_odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
  {
    visualization_msgs::msg::Marker marker;
    marker.header.frame_id = "odom";
    marker.header.stamp = this->get_clock()->now();
    marker.ns = "ekf_fused";
    marker.id = 3;
    marker.type = visualization_msgs::msg::Marker::ARROW;
    marker.action = visualization_msgs::msg::Marker::ADD;
    marker.pose = msg->pose.pose;
    marker.scale.x = 0.7; // arrow length
    marker.scale.y = 0.15; // shaft diameter
    marker.scale.z = 0.15; // head diameter
    marker.color.a = 1.0;
    marker.color.r = 0.0;
    marker.color.g = 1.0;
    marker.color.b = 0.0;

    marker_publisher_->publish(marker);
  }

  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_subscriber_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscriber_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr filtered_odom_subscriber_;
  rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
  std::vector<geometry_msgs::msg::Point> odom_points_;
  nav_msgs::msg::Odometry latest_odom_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SensorDisplayNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
