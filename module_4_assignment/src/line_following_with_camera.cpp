// Purpose:
// - Receives Image messages, applies Canny edge detection, calculates the error between the detected line's midpoint and the image center,
//   and publishes Twist messages to control the robot's motion.
// - Demonstrates integration of image processing (using OpenCV) and motion control in a ROS2 environment.
// Author: Robotisim

#include "rclcpp/rclcpp.hpp"                        // ROS 2 C++ client library
#include "sensor_msgs/msg/image.hpp"                // ROS 2 Image message type
#include "geometry_msgs/msg/twist.hpp"              // ROS 2 Twist message type for velocity commands
#include "cv_bridge/cv_bridge.h"                    // cv_bridge to convert ROS image messages to OpenCV images
#include "opencv2/opencv.hpp"                       // OpenCV library

// Define the CameraSubscriber node class
class CameraSubscriber : public rclcpp::Node {
public:
  // Constructor: Initialize the node, parameters, publisher, and subscription
  CameraSubscriber()
  : Node("camera_subscriber_node"), _angularVel(0.3) {
    // Declare parameters for the Canny edge detection thresholds
    this->declare_parameter<int>("lower_threshold", 200);
    this->declare_parameter<int>("upper_threshold", 250);

    // Create a publisher for Twist messages on the "cmd_vel" topic
    _publisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    
    // Create a subscription for Image messages on the "/camera/image_raw" topic.
    // When a new image arrives, the cameraCallback function is called.
    _subscription = this->create_subscription<sensor_msgs::msg::Image>(
      "/camera/image_raw", 10,
      std::bind(&CameraSubscriber::cameraCallback, this, std::placeholders::_1));

    // Log that the node has started
    RCLCPP_INFO(this->get_logger(), "\n------ Node Started -----\n");
  }

private:
  // Callback function that is called every time an image message is received
  void cameraCallback(const sensor_msgs::msg::Image::SharedPtr cameraMsg) {
    // Create a Twist message for velocity command output
    auto velocityMsg = geometry_msgs::msg::Twist();

    try {
      // Convert the ROS image message to an OpenCV image (in BGR8 format)
      cv_bridge::CvImagePtr cvPtr = cv_bridge::toCvCopy(cameraMsg, "bgr8");

      // Convert the BGR image to grayscale for edge detection
      cv::Mat grayImage, cannyImage;
      cv::cvtColor(cvPtr->image, grayImage, cv::COLOR_BGR2GRAY);

      // Retrieve the Canny edge detection threshold parameters from ROS parameters
      int lowerThreshold = this->get_parameter("lower_threshold").as_int();
      int upperThreshold = this->get_parameter("upper_threshold").as_int();

      // Apply the Canny edge detection algorithm to the grayscale image
      cv::Canny(grayImage, cannyImage, lowerThreshold, upperThreshold);

      // Define a Region Of Interest (ROI) in the Canny image for line detection.
      // In this example, we use a region starting at row 150, with a height of 240 pixels and full width (640 pixels).
      int row = 150, column = 0;
      cv::Mat roi = cannyImage(cv::Range(row, row + 240), cv::Range(column, column + 640));

      // Find the x-coordinates of the white pixels (value 255) in a specific row of the ROI.
      // This is used to detect the line.
      std::vector<int> edge;
      for (int i = 0; i < 640; ++i) {
        if (roi.at<uchar>(160, i) == 255) {  // Check row 160 within ROI
          edge.push_back(i);                // Store the x position of each detected edge
        }
      }

      // If edges are found, process them to determine the robot's steering command.
      if (!edge.empty()) {
        // Calculate the midpoint of the detected line from the first to the last white pixel.
        int midArea = edge.back() - edge.front();
        int midPoint = edge.front() + midArea / 2;
        // The center of the image (assumed robot center) is at half of 640 pixels.
        int robotMidPoint = 640 / 2;

        // Calculate the error: difference between the robot center and the detected line midpoint.
        double error = robotMidPoint - midPoint;

        // Set a constant forward velocity.
        velocityMsg.linear.x = 0.1;
        // Adjust the angular velocity based on the error:
        // If the error is negative, turn right; if positive, turn left.
        if (error < 0) {
          velocityMsg.angular.z = -_angularVel;
        } else {
          velocityMsg.angular.z = _angularVel;
        }

        // Publish the velocity command to control the robot
        _publisher->publish(velocityMsg);

        // For visualization: draw circles on the ROI to mark the detected line midpoint and the robot's center.
        cv::circle(roi, cv::Point(midPoint, 160), 2, cv::Scalar(255, 255, 255), -1);      // White circle at the line's midpoint
        cv::circle(roi, cv::Point(robotMidPoint, 160), 5, cv::Scalar(255, 255, 255), -1);   // White circle at the center of the robot

        // Show the processed ROI image in a window
        cv::imshow("Image", roi);
        cv::waitKey(1);  // Wait for 1 millisecond to allow image window update
      }

    } catch (cv_bridge::Exception &e) {
      // Log any exceptions from cv_bridge
      RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
    }
  }

  // Publisher for velocity commands (Twist messages)
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr _publisher;
  // Subscription for image messages
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr _subscription;
  // Angular speed used for steering adjustments
  double _angularVel;
};

int main(int argc, char **argv) {
  // Initialize ROS 2
  rclcpp::init(argc, argv);
  // Create and spin the CameraSubscriber node
  rclcpp::spin(std::make_shared<CameraSubscriber>());
  // Shutdown ROS 2
  rclcpp::shutdown();
  return 0;
}
