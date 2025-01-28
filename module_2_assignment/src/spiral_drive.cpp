#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cmath> // Para exp y M_PI

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      this->declare_parameter<std::string>("cmd_vel_topic", "/turtle1/cmd_vel");

      std::string cmd_vel_topic = this->get_parameter("cmd_vel_topic").as_string();

      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, 10);
      timer_ = this->create_wall_timer(
      100ms, std::bind(&MinimalPublisher::timer_callback, this)); // Intervalo de 100ms para un movimiento fluido
    }

  private:
    void timer_callback()
    {
      auto message = geometry_msgs::msg::Twist();

      // Parámetros ajustados para espirales rápidas
      double a = 0.5;  // Velocidad base (ajusta para mayor o menor velocidad inicial)
      double b = 0.1;  // Tasa de crecimiento de la espiral (aumenta para espirales más rápidas)

      double time = count_ * 0.1; // Tiempo basado en el contador (ajustado para 100ms intervalos)

      // Velocidades para la espiral logarítmica
      message.linear.x = a * exp(b * time); // Incremento exponencial más rápido de la velocidad lineal
      message.angular.z = 5.0 / (0.1 + time); // Giro más pronunciado inicialmente, se reduce más lento

      RCLCPP_INFO(this->get_logger(), "Linear x: %.2f, Angular z: %.2f", message.linear.x, message.angular.z);

      publisher_->publish(message);

      // Incrementar contador para el tiempo
      count_++;
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
