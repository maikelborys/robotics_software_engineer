from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='module_2_assignment',  # Corrected package name
            executable='led_publisher',
            name='led_publisher',
        ),
        Node(
            package='module_2_assignment',  # Corrected package name
            executable='led_subscriber',
            name='led_subscriber',
        ),
    ])