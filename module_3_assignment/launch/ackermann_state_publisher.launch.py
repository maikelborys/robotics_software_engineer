from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Get package path
    pkg_path = get_package_share_directory('module_3_assignment')

    # Define URDF file path
    urdf_file_path = os.path.join(pkg_path, 'urdf', 'ackermann_drive.urdf')

    # Load URDF content
    with open(urdf_file_path, 'r') as urdf_file:
        urdf_content = urdf_file.read()

    return LaunchDescription([
        # ✅ Publicar `robot_description`
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': urdf_content}]
        ),

        # Node to publish the joint states (for manual movement in RViz)
        Node(
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
            name='joint_state_publisher_gui'
        ),

        # ✅ Publish the TF transforms for Ackermann car
        Node(
            package='module_3_assignment',
            executable='ackermann_tf_broadcaster',
            name='ackermann_tf_broadcaster',
            output='screen'
        )
    ])
