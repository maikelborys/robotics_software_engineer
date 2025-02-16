from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os
from launch.actions import ExecuteProcess

def generate_launch_description():
    # Get the package directory
    pkgPath = get_package_share_directory('module_5_assignment')

    # Define the path to the URDF file
    urdf_file_path = os.path.join(pkgPath, 'urdf', 'robotic_arm.urdf')

    # Ensure the file is loaded as a string
    with open(urdf_file_path, 'r') as urdf_file:
        urdf_content = urdf_file.read()

    return LaunchDescription([
        # Node to publish the URDF model
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': urdf_content}]
        ),

        ExecuteProcess(
            # Load both gazebo_ros_init and gazebo_ros_factory plugins
            cmd=['gazebo', '--verbose', '-s', 'libgazebo_ros_init.so', '-s', 'libgazebo_ros_factory.so'],
            output='screen'),

        Node( 
            package='gazebo_ros',
            executable='spawn_entity.py',
            name='robot_spawner',
            output='screen',
            arguments=['-topic', '/robot_description', '-entity', 'robot_arm']
        ),

        # Node to publish the joint states (for manual movement in RViz)
        # Node(
        #     package='joint_state_publisher_gui',
        #     executable='joint_state_publisher_gui',
        #     name='joint_state_publisher_gui'
        # ),

        # Node to publish static transforms for the robotic arm
        #Node(
        #    package='module_5_assignment',
        #    executable='robotic_arm_tf_broadcaster',
        #    name='robotic_arm_tf_broadcaster',
        #    output='screen'
        #)
    ])