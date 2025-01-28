# Module 2 Assignment: Developing Custom ROS 2 Nodes and Launch Files


### Task 1: Create a Custom ROS 2 Node

 - **Circle Movement:** The turtle should move in a circle with a radius that is provided as a user input.

         1. First at all we need to create .cpp file of our node that will control logic of movement. 
         Ive create it at SRC directory circle_drive.cpp
         2. Than we need to change CMAKELIST 
         added find_package(rclcpp REQUIRED)
         find_package(std_msgs REQUIRED)
         find_package(geometry_msgs REQUIRED)

         add_executable(circle_drive src/circle_drive.cpp)
         ament_target_dependencies(circle_drive rclcpp geometry_msgs)

         and 

         install(TARGETS
         circle_drive
         DESTINATION lib/${PROJECT_NAME})

         3. Creation of code. To simplify the process ive copied code of drive_turtle.cpp to circle_drive without any changes. 

         4. To run a package we need to go at our workspace directory 
         `cd ~/assignment_ws/src/robotics_software_engineer`
         than make cleaning `rm -rf build/ install/ log/`
         after that just build our package `rm -rf build/ install/ log/`
         `source install/setup.bash`
            `ros2 pkg list `
            `ros2 pkg executables module_2_assignment`
         `ros2 run turtlesim turtlesim_node `
         `ros2 run module_2_assignment circle_drive`

  - **Logarithmic Spiral Movement:** The turtle should move in a logarithmic spiral pattern.

         1. Created spiral_drive.cpp, than with GPT ive asked to make code that turtle makes logarithmic moves, gived GPT code circle_drive.cpp
         2. Changed cmake   add_executable(spiral_drive src/spiral_drive.cpp)
      ament_target_dependencies(spiral_drive rclcpp geometry_msgs)
      and INSTALL + spiral_drive
         3. Run 
         ros2 run turtlesim turtlesim_node 
         ros2 run module_2_assignment spiral_drive

### Task 2: Develop a Launch File

- **Create a launch file** that starts the Turtlesim simulation and the custom ROS 2 node simultaneously.

         1. Created multi_nodes.launch.py, rewrited from the lessons code
         2. add     circle_driver=Node(
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
         3. BUILD colcon build --packages-select module_2_assignment
         4. RUN ros2 launch module_2_assignment multi_nodes.launch.py 
         5. As result turtle will have forward and spiral movements together 


### Task 3: Modify the Turtlesim Simulation Environment

- **Use existing Turtlesim services** such as `spawn` and `clear` to modify the simulation environment:
  - **Spawn 5 Turtlebots** with a single launch file, placing them diagonally from the top left to the bottom right.
  - **Drive the middle 3 turtles** back and forth continuously using ROS 2 services.

         1. For multi turtlesim created multi_nodes.launch.py at launch directory 
         2. added code for spawning and code that invoke driving bakc_forth.cpp for 3 turtles
         3. created back_forth.cpp for control back forward movement of turtles
         4. compile just our package $ colcon build --packages-select module_2_assignment
         5. run $ ros2 launch module_2_assignment spawn_turtlesim.launch.py 

         TURTLEBOTS was trying to spawn turtlebots, but without exit yet. I think in the future wi will! 


### Task 4: Modify Turtle Behavior with Parameters

- **Utilize ROS 2 parameters** to alter the behavior of the turtles:
  - **Change the speed** of the turtles dynamically during the simulation.
         1. changed back_forth.cpp , adding logic of change speed
    rcl_interfaces::msg::SetParametersResult param_callback(const std::vector<rclcpp::Parameter> & parameters)
    {
      // Manejar cambios en parámetros dinámicos
      for (const auto & param : parameters) {
        if (param.get_name() == "forward_speed") {
          forward_speed_ = param.as_double();
          RCLCPP_INFO(this->get_logger(), "Updated forward_speed to %.2f", forward_speed_);
        } else if (param.get_name() == "backward_speed") {
          backward_speed_ = param.as_double();
          RCLCPP_INFO(this->get_logger(), "Updated backward_speed to %.2f", backward_speed_);
        }
      }
      rcl_interfaces::msg::SetParametersResult result;
      result.successful = true;
      return result;
    }

### Task 5: Debugging a ROS 2 Node Using a Message Type

      1. Error add executables at cmakelists of led publisher and subscriber
      2. change launch file package name to module_2_assignment
      3. INCLUDE change brackets type from " to <>



ps used GPT for help issues. For spawn turtlebot need a little bit more time. hope in the future will correct