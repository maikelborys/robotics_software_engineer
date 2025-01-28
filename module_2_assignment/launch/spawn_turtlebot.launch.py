from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():
    # Generar el modelo URDF
    generate_urdf = ExecuteProcess(
        cmd=[
            'ros2', 'run', 'xacro', 'xacro',
            '$(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf.xacro',
            '>', '/tmp/turtlebot3_burger.urdf'
        ],
        shell=True
    )

    # Hacer spawn del TurtleBot3
    spawn_turtle = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot1', xml: '$(cat /tmp/turtlebot3_burger.urdf)', initial_pose: {position: {x: 2.0, y: 2.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    return LaunchDescription([
        generate_urdf,
        spawn_turtle,
    ])
