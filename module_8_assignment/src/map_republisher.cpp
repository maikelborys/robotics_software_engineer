#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
using namespace std::chrono_literals;

class MapRepublisher : public rclcpp::Node {
public:
  MapRepublisher() : Node("map_republisher") {
    // Subscribe to the original map topic
    map_subscriber_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
      "/map", 10,
      std::bind(&MapRepublisher::mapCallback, this, std::placeholders::_1));

    // Publisher for the republished map
    map_publisher_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>("map_repub", 10);

    // Timer to republish the map every 5 seconds
    republish_timer_ = this->create_wall_timer(
      5s, std::bind(&MapRepublisher::republishMap, this));
  }

private:
  void mapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg) {
    latest_map_ = msg;
    RCLCPP_INFO(this->get_logger(), "Received map. Will republish continuously.");
  }

  void republishMap() {
    if (latest_map_) {
      // Update the timestamp before publishing
      latest_map_->header.stamp = this->now();
      map_publisher_->publish(*latest_map_);
      RCLCPP_INFO(this->get_logger(), "Republishing map.");
    }
  }

  rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr map_subscriber_;
  rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr map_publisher_;
  rclcpp::TimerBase::SharedPtr republish_timer_;
  nav_msgs::msg::OccupancyGrid::SharedPtr latest_map_;
};

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MapRepublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
