Hello teacher! Thank you so much for lessons. 

### Task 1: Create a Custom Transform Tree
- **Design a robotic arm with 3 DOF** 
  - **Define the transform tree** 
  - **Do not include any visual elements** 

  1. At first lets create URDF file of our robot, coinsists of 4 links: BASE, UPPER_ARM, FOREARM, WRIST. And 3 joints: SHOULDER JOINT, ELBOW JOINT, WRIST JOINT

  2.  Lets create executive file robotic_arm_tf_broadcaster.cpp where we will broadcast transformations of our URDF file. 

  3. Lets change CMAKELISTS.txt to make all work (install directory and execute file)

  4. Create launch file robot_lift_debug.launch.py to publish our URDF model, and ARM NODE 

  5. Lets build package ~/assignment_ws/src/robotics_software_engineer$ colcon build --packages-select module_3_assignment

  6. Run ros2 launch module_3_assignment robot_state_publisher.launch.py

  7. Run rviz2 

  8. Add TF with show names 


### Task 2: Add Joints and Visual Elements

  1. Added types of joints sucsessfuly  
      Base Joint (base_joint) → Continuous (360° rotation).
      Shoulder Joint (shoulder_joint) → Revolute.
      Elbow Joint (elbow_joint) → Revolute.
      Wrist Joint (wrist_joint) → Revolute.
      Finger Joints (finger1_joint, finger2_joint) → Prismatic.

### Task 3: Build a Mobile Manipulator

  1. Done! Fusioned two UDRF from tutorial and from assignment
  2. ros2 launch module_3_assignment ackermann_drive.launch.py

- **Create an Ackerman Drive System:**
  - **Design a car-like robot structure** 
  5 hours later... done! was en error at URDF file that was crashed my robot_state_publisher and i was able to see just TF visualistion at RVIZ


### Task 4: Debugging Task - Fixing an Incorrect URDF for a Wheeled Robot with a Lifting Mechanism
`Task Description:`

now we can see robot properly at RVIZ 
