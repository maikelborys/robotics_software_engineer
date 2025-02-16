#!/usr/bin/env python3
import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    # Declare launch argument for matrix_set (default value: 1)
    matrix_set_arg = DeclareLaunchArgument(
        'matrix_set',
        default_value='1',
        description='Matrix set parameter for LQR node'
    )

    # Path to the TurtleBot3 Gazebo launch file
    tb3_gazebo_launch_dir = os.path.join(get_package_share_directory('turtlebot3_gazebo'), 'launch')
    tb3_gazebo_launch_file = os.path.join(tb3_gazebo_launch_dir, 'empty_world.launch.py')

    # Node for the LQR controller with parameter forwarding
    goal_planner_node = Node(
        package='module_5_assignment',
        executable='lqr_node',
        name='multi_goal_follow',
        parameters=[{'matrix_set': LaunchConfiguration('matrix_set')}]
    )

    return LaunchDescription([
        matrix_set_arg,
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(tb3_gazebo_launch_file)
        ),
        goal_planner_node
    ])
