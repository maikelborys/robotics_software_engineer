Hello Muhhammad, This module was harder than others to understand. Was watching and listening in earphones lessons 50 times. Especially hard to understand LQR, so task 4 done by Copilot totally. 

Hope to see new podcasts at youtube! 

*****************************************************************************************
*****************************************************************************************
*****************************************************************************************


# Module 5 Assignment: Enhancing Robot Control for Smooth Motion
*****************************************************************************************
#### Task 1: Improve Camera-Based Line Following

1. Make a copy of project files of line following lesson to module_5 folder 
2. Changed algorytm of line detection from Canny edge detection to thresholding approach  

Lessons option algorytm coinsists to localize line uses Canny edge detection
   1. Converted image at grayscale and than applying Canny to detect edges
   2. Than scans fixed row ROI for white pixels to determine the leftmost and rightmost points. 
   3. At final computes midpoint betwene these extreme points. 

Updated algorytm uses thresholding approach. 
   1. Converts a grayscale
   2. Applies gaussian blur to reduce noice 
   3. uses a fixed threshold with THRESH_BINARY_INV so the dark line becomes white on a black BG 
   4. Erosion/dilatation to remove noice 
   5. Computes image momets over the our row ROI to accurately determine line centroid 

Parameters used 
   "threshold_value", 50  - Pixels with intensity below this value become white isolating the dark line. Lower value may capture more details but can introduce noice. 
   "kp", 0.005 - Proportional Gain used in the controller. Scales error between desired and actual line position. Higher kp results in more aggressive corrections, whereas a lower kp leads to gentler adjustments. 

2. Integrated KP coeficient, Proportional Gain, Scales the error, diference between the desired and measured pos, so that larger error produces a proportionally larger corrective action.
 Declared at class constructor         this->declare_parameter<double>("kp", 0.005);

3. Build and launch 
         cd /home/maikelborys/assignment_ws/src/robotics_software_engineer
         source install/setup.bash
         colcon build --packages-select module_5_assignment --symlink-install
         ros2 launch module_5_assignment camera_line_following.launch.py

         Robot start moving following a line, you can move robot by hand and see how it will look for a line. When line finished. Robot looking a line with double speed and starts following. 

*****************************************************************************************
### Task 2: Optimize Goal Selection and Path Planning

1. Implemented goal_planner node from lesson
2. In constructor class of GOalPlanner initialized candidate goals vector of 3 goals. Hardcoded 
3. In the odom_callback function if goal has not been selected yet compute each distance from goals to robot. Select minimal distance.
4. Using selected goal for planning
5. Build and launch 
         cd /home/maikelborys/assignment_ws/src/robotics_software_engineer
         source install/setup.bash
         colcon build --packages-select module_5_assignment --symlink-install
         ros2 launch module_5_assignment goal_planner.launch.py

         Robot start moving to the goal. When the goal reached robot stops and comunicate Goal Reached.

*****************************************************************************************
### Task 3: Add a Position Controller to Robotic Arm URDF

1. Modified robotic_arm.urdf added Custom Position controller to all joints

2. Created arm_controller.yaml file with command interfaces of effort and state interf position and effort

3. Compile colcon build --packages-select module_5_assignment --symlink-install
   Run ros2 launch module_5_assignment robot_state_publisher.launch.py 
   Start controller ros2 run controller_manager spawner arm_controller
   Start broadcaster ros2 run controller_manager spawner joint_state_broadcaster
   Send a manual command to moove ros2 topic pub /arm_controller/commands std_msgs/msg/Float64MultiArray "data: 1.5 1.5"

*****************************************************************************************

### Task 4: Implement and Visualize LQR for Multi-Goal Following


1. Implemented visualisation in RViz by created marker publisher    marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>(
      "visualization_marker", rclcpp::QoS(100).reliable());
2. Created vector of matrix, compared how this values afected behaviour. Analisis above. 

3. Included screenshots from rviz. 

4. RUN ros2 launch module_5_assignment lqr_multi_goals.launch.py matrix_set:=1 (2,3)


# Performance Analysis of LQR Controller with Different Q & R Settings

## Experiment Setup
We tested three different Q and R sets by launching the node with the parameter "matrix_set" (e.g., -p matrix_set:=1, 2, or 3). For each scenario, we recorded:
- Responsiveness (how quickly the robot corrected its state).
- Smoothness (lack of oscillations during transitions).
- Overall stability and speed during multi-goal navigation.
- RViz visualizations were used to capture the robot’s path (green line) and goal locations (red spheres).

## Q & R Matrices Tested

| Matrix Set | Q Matrix                      | R Matrix                      | Observations                                          |
|------------|-------------------------------|-------------------------------|-------------------------------------------------------|
| Set 1      | diag(0.8, 0.8, 0.8)           | diag(0.8, 0.8)                | Moderate responsiveness; smooth transitions with minimal overshoot. |
| Set 2      | diag(1.0, 1.0, 1.0)           | diag(1.0, 1.0)                | Faster response but slight oscillations when approaching goals.        |
| Set 3      | diag(0.5, 0.5, 0.5)           | diag(0.5, 0.5)                | Slower, smoother corrections; less aggressive control adjustments.      |

## RViz Visualizations
- **Set 1:** RViz showed smooth and steady motion with accurate goal tracking. The path marker closely followed the expected trajectory.
- **Set 2:** The robot exhibited quicker turns. RViz captured some oscillations around the goals, indicating a more aggressive response.
- **Set 3:** The robot followed a smooth path, albeit with reduced responsiveness. It took longer to correct errors but maintained overall stability.

## Analysis Summary
- **Best Balance:** Matrix Set 1 provided the best balance between responsiveness and smoothness. With moderate gains, the robot adjusted its trajectory promptly while keeping oscillations minimal.
- **Trade-offs:** Set 2 offered faster responses at the risk of inducing minor oscillations, while Set 3 yielded very smooth but slower corrections.
- **Conclusion:** For our multi-goal navigation task, Set 1 is recommended, achieving both stable dynamics and efficient goal transitions.

*Note: Actual performance may vary with environmental conditions. Further fine-tuning of the matrices may be beneficial based on additional testing.*

*****************************************************************************************