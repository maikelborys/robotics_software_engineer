#include "rclcpp/rclcpp.hpp"
#include "ekf_lib.hpp"      // Ensure ExtendedKalmanFilter is defined (inline or in a separate source file)
#include <Eigen/Dense>
#include <vector>
#include <nav_msgs/msg/odometry.hpp>
#include <visualization_msgs/msg/marker.hpp>

using namespace std::chrono_literals;

// For simplicity, we assume:
// - State dimension: 5
// - Measurement dimension: 4
typedef Eigen::Matrix<double, 5, 1> Vector5d;
typedef Eigen::Matrix<double, 5, 5> Matrix5d;
typedef Eigen::Matrix<double, 4, 4> Matrix4d;

class EKFExperimentNode : public rclcpp::Node
{
public:
  EKFExperimentNode() : Node("ekf_experiment_node")
  {
    // Declare parameters for Q and R
    this->declare_parameter<double>("process_noise", 0.1);
    this->declare_parameter<double>("measurement_noise", 0.1);

    // Get the parameters
    double process_noise = this->get_parameter("process_noise").as_double();
    double measurement_noise = this->get_parameter("measurement_noise").as_double();

    // Set the Q and R matrices based on the parameters
    Q_ = process_noise * Matrix5d::Identity();
    R_ = measurement_noise * Matrix4d::Identity();

    // A timer calls the experiment routine periodically.
    timer_ = this->create_wall_timer(5s, std::bind(&EKFExperimentNode::run_experiments, this));
  }

private:
  // Helper method to set initial matrices for the EKF.
  void setMatrices()
  {
    x_in_ = Vector5d::Zero();
    P_in_ = Matrix5d::Identity();
    F_in_ = Matrix5d::Identity();

    H_in_.setZero();
    H_in_.block<2,2>(0,0) = Eigen::Matrix2d::Identity();
    H_in_.block<2,1>(2,4) = Eigen::Matrix<double,2,1>::Ones();
  }
  
  // The main running method experiments with different Q and R settings.
  void run_experiments()
  {
    // Dummy measurement vector (for demonstration only)
    Eigen::Matrix<double, 4, 1> z;
    z << 1.0, 1.0, 0.5, 0.2;
    
    // Initialize the matrices
    setMatrices();
    
    ExtendedKalmanFilter ekf;
    ekf.dt = 0.1;
    
    // Initialize the EKF
    ekf.initialize(x_in_, P_in_, F_in_, H_in_, R_, Q_);
    
    // Execute prediction and update using the dummy measurement.
    ekf.predict();
    ekf.update(z);
    
    // Log the estimated state vector.
    RCLCPP_INFO(this->get_logger(), "Estimated state:");
    for (int j = 0; j < 5; j++)
    {
        RCLCPP_INFO(this->get_logger(), "  x[%d] = %f", j, ekf.x_[j]);
    }
  }

  // Member variables to hold EKF matrices.
  Vector5d x_in_;
  Matrix5d P_in_;
  Matrix5d F_in_;
  Eigen::Matrix<double, 4, 5> H_in_;
  Matrix5d Q_;
  Matrix4d R_;

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<EKFExperimentNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
