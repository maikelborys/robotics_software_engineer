#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

using namespace std::chrono_literals;

class BackForthPublisher : public rclcpp::Node
{
  public:
    BackForthPublisher()
    : Node("back_forth_publisher"), forward_(true)
    {
      // Declarar parámetros iniciales
      this->declare_parameter<std::string>("cmd_vel_topic", "/turtle1/cmd_vel");
      this->declare_parameter<double>("forward_speed", 3.0);
      this->declare_parameter<double>("backward_speed", -3.0);

      // Obtener valores de parámetros
      std::string cmd_vel_topic = this->get_parameter("cmd_vel_topic").as_string();
      forward_speed_ = this->get_parameter("forward_speed").as_double();
      backward_speed_ = this->get_parameter("backward_speed").as_double();

      // Configurar publicación de mensajes
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(cmd_vel_topic, 10);

      // Configurar temporizador para alternar movimiento
      timer_ = this->create_wall_timer(
        1000ms, // Intervalo de 1 segundo
        std::bind(&BackForthPublisher::timer_callback, this));

      // Configurar parámetros dinámicos
      param_callback_handle_ = this->add_on_set_parameters_callback(
        std::bind(&BackForthPublisher::param_callback, this, std::placeholders::_1));
    }

  private:
    void timer_callback()
    {
      auto message = geometry_msgs::msg::Twist();

      // Cambiar velocidad según la dirección
      message.linear.x = forward_ ? forward_speed_ : backward_speed_;
      message.angular.z = 0.0; // Sin giro

      RCLCPP_INFO(this->get_logger(), "Moving %s at speed %.2f",
                  forward_ ? "forward" : "backward",
                  message.linear.x);

      publisher_->publish(message);

      // Alternar dirección
      forward_ = !forward_;
    }

    rcl_interfaces::msg::SetParametersResult param_callback(const std::vector<rclcpp::Parameter> & parameters)
    {
      // Manejar cambios en parámetros dinámicos
      for (const auto & param : parameters) {
        if (param.get_name() == "forward_speed") {
          forward_speed_ = param.as_double();
          RCLCPP_INFO(this->get_logger(), "Updated forward_speed to %.2f", forward_speed_);
        } else if (param.get_name() == "backward_speed") {
          backward_speed_ = param.as_double();
          RCLCPP_INFO(this->get_logger(), "Updated backward_speed to %.2f", backward_speed_);
        }
      }
      rcl_interfaces::msg::SetParametersResult result;
      result.successful = true;
      return result;
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

    bool forward_; // Estado para alternar dirección
    double forward_speed_; // Velocidad hacia adelante
    double backward_speed_; // Velocidad hacia atrás
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BackForthPublisher>());
  rclcpp::shutdown();
  return 0;
}
