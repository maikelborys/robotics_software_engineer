from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    
    turtlesim=Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim'
    )

    circle_driver=Node(
        package='module_2_assignment',
        executable='circle_drive',
        name='circle_driver'
    )

    spiral_driver=Node(
        package='module_2_assignment',
        executable='spiral_drive',
        name='spiral_driver'
    )

    return LaunchDescription([
        turtlesim, 
        circle_driver,
        spiral_driver,
        
    ])