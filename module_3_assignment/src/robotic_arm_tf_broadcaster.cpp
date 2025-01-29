#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

class StaticTransformPublisher : public rclcpp::Node
{
public:
    StaticTransformPublisher()
    : Node("static_transform_publisher")
    {
        broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

        publish_transforms();
    }

private:
    void publish_transforms()
    {
        // Create and send the transforms
        send_transform("base_link", "shoulder_link", 0.0, 0.0, 0.2);
        send_transform("shoulder_link", "upper_arm_link", 0.0, 0.0, 0.2);
        send_transform("upper_arm_link", "elbow_link", 0.2, 0.0, 0.0);
        send_transform("elbow_link", "forearm_link", 0.2, 0.0, 0.0);
        send_transform("forearm_link", "wrist_link", 0.2, 0.0, 0.0);
    }

    void send_transform(const std::string& parent, const std::string& child,
                        double x, double y, double z)
    {
        geometry_msgs::msg::TransformStamped transform;
        transform.header.stamp = this->get_clock()->now();
        transform.header.frame_id = parent;
        transform.child_frame_id = child;

        transform.transform.translation.x = x;
        transform.transform.translation.y = y;
        transform.transform.translation.z = z;

        tf2::Quaternion q;
        q.setRPY(0, 0, 0);  // No rotation
        transform.transform.rotation.x = q.x();
        transform.transform.rotation.y = q.y();
        transform.transform.rotation.z = q.z();
        transform.transform.rotation.w = q.w();

        broadcaster_->sendTransform(transform);
        RCLCPP_INFO(this->get_logger(), "Published transform from %s to %s", parent.c_str(), child.c_str());
    }

    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<StaticTransformPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}