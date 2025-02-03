# Module 4 Assignment: Sensor Data Manipulation for Robot Control

### Task 1: Restructure Line Following Project Code

1. Uptdated paths
2. Refactored execute file line_following_with_camera.cpp, added funcions 
  searchMovement(velocityMsg);
  followEdge(velocityMsg, edge);
  setVelocity(velocityMsg, error);
  calculateError(const std::vector<int> &edge)
  visualize(cv::Mat &roi, const std::vector<int> &edge)
  handleState(geometry_msgs::msg::Twist &velocityMsg, const std::vector<int> &edge)

  created State Machine to make possible spawn robot at point that cant see two lines. Robot starts to moove till find two lines, than enter at foolow rutine. 
   - The `handleState` function manages the robot's behavior based on the detected edges:
     - **SEARCHING**: The robot moves in alternating directions until an edge is detected.
     - **EDGE_DETECTED**: The robot follows the detected edge until two edges are detected.
     - **FOLLOWING**: The robot follows the line based on the detected edges.

 3. Added comments    
 4. To run a program intialize your workspace source install/setup.bash, ros2 launch module_4_assignment camera_line_following.launch.py 
5. GIF of the working program can find at _GIFs_workflow




### Task 2: Design Software Flow for Maze Solving

1. Drawed desing at LUCID.app, you can find a file at _GIFs_workflow folder


### Task 3: Implement Maze Solving in Gazebo

1. You can launch code by ros2 launch module_4_assignment lidar_maze_solving.launch.py 
2. How its made? Created a maze, saved like a world, fixed directions.
3. To find RIGHT side of wall, ive modified a code 
     } else if (rightObstacle < _sideThreshold) {
        // Adjust to follow the right wall
        command.linear.x = _linearVel;
        command.angular.z = -_angularVel / 2;
4. For utilize IMU sensor made new subscribtor _imu_subscription that receives IMU data from /imu topic
every time when it called with  RCLCPP_INFO(this->get_logger(), " 
makin a log to show dates from acceleration and angular velocity



### Task 4: Fix Errors in Launch Files
1. From debug folder transfered all files at pkg folder module_4_assignment
2. Changed project name at package.xml and cmake
3. Error at camera_line_following.launch.py, spawn_tb3.launch.py > spawn_turtlebot3.launch.py


