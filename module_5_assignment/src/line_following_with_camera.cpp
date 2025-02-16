// Propósito:
// - Recibe imágenes de la cámara, detecta bordes con Canny, encuentra la línea en el suelo,
//   calcula el error con respecto al centro y publica velocidades para corregir la trayectoria del robot.
// Autor: Robotisim

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

class LineFollower : public rclcpp::Node {
public:
    LineFollower()
        : Node("line_follower") {

        // Definir parámetros ajustables
        this->declare_parameter<int>("threshold_value", 50);  // valor para threshold
        this->declare_parameter<double>("kp", 0.005);

        // Crear publicador y suscriptor
        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        image_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
            "/camera/image_raw", 10,
            std::bind(&LineFollower::processImage, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Nodo LineFollower iniciado con nuevo algoritmo universal.");
    }

private:
    void processImage(const sensor_msgs::msg::Image::SharedPtr msg) {
        try {
            // Convertir la imagen a OpenCV y convertir a escala de grises
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
            cv::Mat gray;
            cv::cvtColor(cv_ptr->image, gray, cv::COLOR_BGR2GRAY);
            
            // Aplicar GaussianBlur para reducir el ruido
            cv::GaussianBlur(gray, gray, cv::Size(5,5), 0);

            // Umbral fijo para invertir la imagen y resaltar la línea negra
            int thresh_val = this->get_parameter("threshold_value").as_int();
            cv::Mat binary;
            cv::threshold(gray, binary, thresh_val, 255, cv::THRESH_BINARY_INV);

            // Definir la región de interés (ROI) en la parte inferior
            int roi_start_row = 150;
            cv::Mat roi = binary(cv::Range(roi_start_row, roi_start_row + 240), cv::Range(0, binary.cols));

            // Aplicar operaciones morfológicas para eliminar ruido
            cv::erode(roi, roi, cv::Mat(), cv::Point(-1,-1), 2);
            cv::dilate(roi, roi, cv::Mat(), cv::Point(-1,-1), 2);

            // Calcular momentos para obtener el centroide de la línea detectada
            cv::Moments m = cv::moments(roi, true);
            geometry_msgs::msg::Twist velocity_msg;
            if (m.m00 > 0) {
                int cx = static_cast<int>(m.m10 / m.m00);
                int center_roi = roi.cols / 2;
                double error = center_roi - cx;

                // Control simple proporcional para la velocidad angular
                double kp = this->get_parameter("kp").as_double();
                double control = kp * error;

                velocity_msg.linear.x = 0.2;  // aumentar velocidad para mejor seguimiento
                velocity_msg.angular.z = control;

                // Visualización: marcar el centroide y el centro del ROI
                cv::circle(roi, cv::Point(cx, roi.rows/2), 5, cv::Scalar(255), -1);
                cv::circle(roi, cv::Point(center_roi, roi.rows/2), 5, cv::Scalar(128), -1);
                cv::imshow("Algoritmo Universal - ROI", roi);
                cv::waitKey(1);
            } else {
                // Fallback: si la línea se perdió, hacer vuelta y avanzar el doble
                velocity_msg.linear.x = 0.4;  // velocidad doble
                velocity_msg.angular.z = 0.5; // giro para buscar la línea
                RCLCPP_WARN(this->get_logger(), "Línea perdida, ejecutando maniobra de giro.");
            }

            velocity_publisher_->publish(velocity_msg);

        } catch (cv_bridge::Exception &e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        }
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_subscription_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LineFollower>());
    rclcpp::shutdown();
    return 0;
}
