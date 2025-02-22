from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Get file paths for TurtleBot3 empty world, EKF config, and RViz config
    tb3_share = get_package_share_directory('turtlebot3_gazebo')
    tb3_empty_launch = os.path.join(tb3_share, 'launch', 'empty_world.launch.py')
    
    sf_share = get_package_share_directory('module_6_assignment')
    parameters_file = os.path.join(sf_share, 'config', 'ekf_tb3_imu_odom.yaml')
    
    return LaunchDescription([
        # Launch Gazebo with empty world
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(tb3_empty_launch)
        ),
        # Launch the EKF experiment node with low noise parameters
        Node(
            package='module_6_assignment',
            executable='ekf_experiment_node',
            name='ekf_experiment_node',
            output='screen',
            parameters=[{'use_sim_time': True, 'process_noise': 0.1, 'measurement_noise': 0.1}]
        ),
        # Launch the sensor display node
        Node(
            package='module_6_assignment',
            executable='sensor_display_node',
            name='sensor_display_node',
            output='screen',
            parameters=[{'use_sim_time': True}]
        )
    ])
