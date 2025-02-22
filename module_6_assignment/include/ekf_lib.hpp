#ifndef EKF_LIB_HPP
#define EKF_LIB_HPP

#include <Eigen/Dense>

// IMU Readings: [omega, a]
// GPS Readings: [x_gps, y_gps]
// State Vector: [x, y, vx, vy, theta]
// Measurements: [GPS_readings, IMU_readings]

class ExtendedKalmanFilter {
public:
    ExtendedKalmanFilter();

    void initialize(Eigen::VectorXd const& x_in,
                    Eigen::MatrixXd const& P_in,
                    Eigen::MatrixXd const& F_in,
                    Eigen::MatrixXd const& H_in,
                    Eigen::MatrixXd const& R_in,
                    Eigen::MatrixXd const& Q_in);

    // Prediction Step
    void predict();

    // Measurement Prediction
    void measurement_prediction();

    // Update Step
    void update(Eigen::VectorXd const& z);

    // Matrices Updating
    void updateF();
    void updateR(std::vector<double> const& R_in);

    Eigen::VectorXd x_;
    Eigen::VectorXd x_pred_;
    Eigen::VectorXd z_pred_;
    double dt;

private:


    Eigen::MatrixXd P_;
    Eigen::MatrixXd F_;
    Eigen::MatrixXd H_;
    Eigen::MatrixXd R_;
    Eigen::MatrixXd Q_;
    double epsilon = 1e-6;
    void normalizeAngle(double& angle);
    void update_state();
};

// Inline implementations

inline ExtendedKalmanFilter::ExtendedKalmanFilter() : dt(0.1) {
  x_ = Eigen::VectorXd::Zero(5);  // Assuming state dimension 5
}

inline void ExtendedKalmanFilter::initialize(const Eigen::VectorXd& x,
                                               const Eigen::MatrixXd& P,
                                               const Eigen::MatrixXd& F,
                                               const Eigen::MatrixXd& H,
                                               const Eigen::MatrixXd& R,
                                               const Eigen::MatrixXd& Q) {
  x_ = x;
  P_ = P;
  F_ = F;
  H_ = H;
  R_ = R;
  Q_ = Q;
}

inline void ExtendedKalmanFilter::predict() {
  // Simple prediction: x = F * x and update covariance P.
  x_ = F_ * x_;
  P_ = F_ * P_ * F_.transpose() + Q_;
}

inline void ExtendedKalmanFilter::update(const Eigen::VectorXd& z) {
  // Standard EKF update.
  Eigen::VectorXd y = z - H_ * x_;
  Eigen::MatrixXd S = H_ * P_ * H_.transpose() + R_;
  Eigen::MatrixXd K = P_ * H_.transpose() * S.inverse();
  x_ = x_ + K * y;
  P_ = (Eigen::MatrixXd::Identity(x_.size(), x_.size()) - K * H_) * P_;
}

#endif
