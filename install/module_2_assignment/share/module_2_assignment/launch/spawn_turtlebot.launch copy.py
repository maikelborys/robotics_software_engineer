from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch_ros.actions import Node
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare
from launch.substitutions import PathJoinSubstitution

def generate_launch_description():
    # Launch TurtleBot3 Gazebo world
    gazebo_world = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            FindPackageShare('turtlebot3_gazebo'),
            '/launch/turtlebot3_world.launch.py'
        ])
    )

    # Spawn TurtleBot3 robots in diagonal positions
    spawn_turtle1 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot1', xml: '$(cat $(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf)', robot_namespace: 'turtlebot1', initial_pose: {position: {x: 0.0, y: 0.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    spawn_turtle2 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot2', xml: '$(cat $(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf)', robot_namespace: 'turtlebot2', initial_pose: {position: {x: 2.0, y: 2.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    spawn_turtle3 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot3', xml: '$(cat $(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf)', robot_namespace: 'turtlebot3', initial_pose: {position: {x: 4.0, y: 4.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    spawn_turtle4 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot4', xml: '$(cat $(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf)', robot_namespace: 'turtlebot4', initial_pose: {position: {x: 6.0, y: 6.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    spawn_turtle5 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call', '/spawn_entity',
            'gazebo_msgs/srv/SpawnEntity',
            "\"{name: 'turtlebot5', xml: '$(cat $(ros2 pkg prefix turtlebot3_description)/urdf/turtlebot3_burger.urdf)', robot_namespace: 'turtlebot5', initial_pose: {position: {x: 8.0, y: 8.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}, reference_frame: 'world'}\""
        ],
        shell=True
    )

    # Launch back_forth controllers for the middle robots
    back_forth_turtle2 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtlebot2_driver',
        parameters=[{'cmd_vel_topic': '/turtlebot2/cmd_vel'}]
    )

    back_forth_turtle3 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtlebot3_driver',
        parameters=[{'cmd_vel_topic': '/turtlebot3/cmd_vel'}]
    )

    back_forth_turtle4 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtlebot4_driver',
        parameters=[{'cmd_vel_topic': '/turtlebot4/cmd_vel'}]
    )

    return LaunchDescription([
        gazebo_world,
        spawn_turtle1,
        spawn_turtle2,
        spawn_turtle3,
        spawn_turtle4,
        spawn_turtle5,
        back_forth_turtle2,
        back_forth_turtle3,
        back_forth_turtle4,
    ])
