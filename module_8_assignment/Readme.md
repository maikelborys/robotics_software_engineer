# Module 8 Assignment: Path Planning with A* and RRT


### Task 1: Compare A* and RRT Path Planning

To make possible path planning we need special format message type of our map (nav_msgs/msg/OccupancyGrid) so we transformed our pgm yaml maze to special msg type.

From anterior module 7 we know how to do it. I make launch file where transformed maze_map.pgm to occupancy_grid msg type. 

map_loading_2d.launch.py

Also i repeated path_planning lessons code and ensured that its worinkg perfectly with occupancy_grid.cpp generated map instead of real.

Than i have integrated path_planning in map_loading_2d.launch.py launch file 

ros2 launch module_8_assignment map_loading_2d.launch.py

With rqt_graph ensured that we have active path planning node subscribed to our topic with map - /map 

![Screenshot](./_Gifs/Screenshot%20from%202025-03-04%2012-46-03.png)

ros2 topic info /map - we see correct mssg type nav_msgs/msg/OccupancyGrid
ros2 topic echo /map - correctly messages like from occupancy_grid generated map
rviz2 - correctly visualizing everything 


at path_planning.cpp we need to change topic which we are subscribed 

   "map_repub", 10,
   //"occupancy_grid", 10,

at algo_astar.cpp lets put goal coordinates 

  goal_point.x = 0;  // Set your goal x-coordinate here
  goal_point.y = 5;  // Set your goal y-coordinate here

   1. A* is a grid-based, deterministic algorithm that finds the shortest path using a cost function, ensuring optimality but requiring more memory. 
   
   2. RRT is a sampling-based, non-deterministic algorithm that explores the space randomly, making it better for high-dimensional spaces but less optimal and sometimes producing jagged paths requiring smoothing.

   3. A* follows a structured approach, finding the shortest path with predictable steps but can be slow in large areas. 
   
   4. RRT grows randomly, quickly reaching the goal but creating rougher paths. A* is better for known maps, while RRT works well in complex spaces. RRT may need extra steps to smooth paths.

   5. A* explores paths logically, ensuring efficiency and accuracy but struggles with large, complex spaces. RRT expands randomly, quickly covering unknown areas but generates uneven paths. In a structured maze, A* is better due to predictability. For unknown or dynamic environments, RRT is more adaptable but requires post-processing for smoothness.

![Screenshot](./_Gifs/Screenshot%20from%202025-03-04%2011-59-31.png)
   

When trying to run RRT I have an error  

`[map_republisher_node-4] [INFO] [1741020195.877981808] [map_republisher]: Republishing map.
[path_planning_node-6] [INFO] [1741020195.878199120] [path_planning_node]: Received occupancy grid with dimensions: 114 x 124
[ERROR] [path_planning_node-6]: process has died [pid 183523, exit code -11, cmd '/home/maikelborys/assignment_ws/src/robotics_software_engineer/install/module_8_assignment/lib/module_8_assignment/path_planning_node --ros-args'].
[INFO] [path_planning_node-6]: process started with pid [183535]
[path_planning_node-6] [INFO] [1741020199.316219092] [path_planning_node]: Performing Path Planning Search
[map_republisher_node-4] [INFO] [1741020200.878082869] [map_republisher]: Republishing map.`


### Task 2: Improve RRT to RRT* for Enhanced Performance

Very hard task for my level 

### Task 3: Explain and Write Unit Tests for RRT*

Very hard task for my level 


*********************************
