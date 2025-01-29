#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

class WheeledRobotTFBroadcaster : public rclcpp::Node
{
public:
    WheeledRobotTFBroadcaster()
        : Node("wheeled_robot_tf_broadcaster")
    {
        broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        publish_transforms();
    }

private:
    void publish_transforms()
    {
        // ✅ Publicamos la base
        send_transform("world", "base_link", 0.0, 0.0, 0.1);

        // ✅ Ruedas traseras (fijas)
        send_transform("base_link", "rear_left_wheel", -0.4, 0.25, -0.1);
        send_transform("base_link", "rear_right_wheel", -0.4, -0.25, -0.1);

        // ✅ Bisagras de dirección delantera
        send_transform("base_link", "front_left_steering_hinge", 0.4, 0.25, -0.1);
        send_transform("base_link", "front_right_steering_hinge", 0.4, -0.25, -0.1);

        // ✅ Ruedas delanteras (conectadas a la bisagra de dirección)
        send_transform("front_left_steering_hinge", "front_left_wheel", 0.0, 0.0, 0.0);
        send_transform("front_right_steering_hinge", "front_right_wheel", 0.0, 0.0, 0.0);

        // ✅ Mecanismo de elevación (lift)
        send_transform("base_link", "lift", 0.0, 0.0, 0.1);
    }

    void send_transform(const std::string& parent, const std::string& child, double x, double y, double z)
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
    auto node = std::make_shared<WheeledRobotTFBroadcaster>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
