from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

def generate_launch_description():

    # Nodo principal de turtlesim
    turtlesim = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim',
    )

    # Llamadas al servicio spawn para agregar tortugas manualmente
    spawn_turtle2 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call',
            '/spawn',
            'turtlesim/srv/Spawn',
            '"{x: 1.0, y: 9.0, theta: 0.0, name: \'turtle2\'}"'
        ],
        shell=True
    )
    spawn_turtle3 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call',
            '/spawn',
            'turtlesim/srv/Spawn',
            '"{x: 3.0, y: 7.0, theta: 0.0, name: \'turtle3\'}"'
        ],
        shell=True
    )
    spawn_turtle4 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call',
            '/spawn',
            'turtlesim/srv/Spawn',
            '"{x: 5.0, y: 5.0, theta: 0.0, name: \'turtle4\'}"'
        ],
        shell=True
    )
    spawn_turtle5 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call',
            '/spawn',
            'turtlesim/srv/Spawn',
            '"{x: 7.0, y: 3.0, theta: 0.0, name: \'turtle5\'}"'
        ],
        shell=True
    )
    spawn_turtle6 = ExecuteProcess(
        cmd=[
            'ros2', 'service', 'call',
            '/spawn',
            'turtlesim/srv/Spawn',
            '"{x: 9.0, y: 1.0, theta: 0.0, name: \'turtle6\'}"'
        ],
        shell=True
    )

    # Nodos para controlar las tortugas (solo turtle3, turtle4 y turtle5)
    turtle_driver3 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtle_driver3',
        parameters=[{'cmd_vel_topic': '/turtle3/cmd_vel'}]
    )
    turtle_driver4 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtle_driver4',
        parameters=[{'cmd_vel_topic': '/turtle4/cmd_vel'}]
    )
    turtle_driver5 = Node(
        package='module_2_assignment',
        executable='back_forth',
        name='turtle_driver5',
        parameters=[{'cmd_vel_topic': '/turtle5/cmd_vel'}]
    )

    return LaunchDescription([
        turtlesim,
        spawn_turtle2,
        spawn_turtle3,
        spawn_turtle4,
        spawn_turtle5,
        spawn_turtle6,
        turtle_driver3,
        turtle_driver4,
        turtle_driver5,
    ])
