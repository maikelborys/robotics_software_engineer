from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Obtener el directorio del paquete
    pkg_path = get_package_share_directory('module_3_assignment')

    # Definir la ruta del archivo URDF
    urdf_file_path = os.path.join(pkg_path, 'urdf', 'wheeled_robot_lift.urdf')

    # Cargar el contenido del URDF
    with open(urdf_file_path, 'r') as urdf_file:
        urdf_content = urdf_file.read()

    return LaunchDescription([
        # ✅ Publicar la descripción del robot
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{'robot_description': urdf_content}]
        ),

        # ✅ Publicar estados de articulaciones
        Node(
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
            name='joint_state_publisher_gui'
        ),

        # ✅ Lanzar el broadcaster de transformaciones TF2
        Node(
            package='module_3_assignment',
            executable='wheeled_robot_tf_broadcaster',
            name='wheeled_robot_tf_broadcaster',
            output='screen'
        ),

        # ✅ Iniciar RViz para visualizar
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen'
        )
    ])
