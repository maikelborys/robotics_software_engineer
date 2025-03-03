#!/usr/bin/env python3
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction
from launch_ros.actions import Node

def generate_launch_description():
    pkg_path = os.path.join(get_package_share_directory('module_8_assignment'))
    map_yaml_file = os.path.join(pkg_path, 'map_gens', 'maze_map.yaml')

    rviz = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz'
    )

    map_server = Node(
        package='nav2_map_server',
        executable='map_server',
        name='map_server',
        output='screen',
        parameters=[{'yaml_filename': map_yaml_file}]
    )


    set_map_server_to_configure = ExecuteProcess(
        cmd=['ros2', 'lifecycle', 'set', '/map_server', 'configure'],
        output='screen'
    )

    # Command to activate the map server, wrapped in a TimerAction for delay
    set_map_server_to_activate = TimerAction(
        period=3.0,  # Delay in seconds
        actions=[
            ExecuteProcess(
                cmd=['ros2', 'lifecycle', 'set', '/map_server', 'activate'],
                output='screen'
            )
        ]
    )

    # Launch our Map Republisher node to continuously republish the map
    map_republisher = Node(
         package='module_8_assignment',
         executable='map_republisher_node',
         name='map_republisher',
         output='screen'
     )
    
    Occupancy_grid = Node(
        package="module_8_assignment",
        executable="occupancy_grid_node",
        output="screen",
    )

    path_planning_node = Node(
        package="module_8_assignment",
        executable="path_planning_node",
        output="screen",
        respawn=True,           # Automatically relaunch if node dies
        respawn_delay=3.0       # Wait 3 seconds before relaunching
    )



    ld = LaunchDescription()
    ld.add_action(map_server)
    ld.add_action(rviz)
    ld.add_action(set_map_server_to_configure)
    ld.add_action(set_map_server_to_activate)
    ld.add_action(map_republisher)
    ld.add_action(Occupancy_grid)
    ld.add_action(path_planning_node)

    return ld
