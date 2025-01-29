cmake_minimum_required(VERSION 3.8)
project(module_3_assignment)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

add_executable(robotic_arm_tf_broadcaster src/robotic_arm_tf_broadcaster.cpp)
ament_target_dependencies(robotic_arm_tf_broadcaster rclcpp tf2_ros geometry_msgs)

add_executable(ackermann_tf_broadcaster src/ackermann_tf_broadcaster.cpp)
ament_target_dependencies(ackermann_tf_broadcaster rclcpp tf2_ros geometry_msgs)

# Ejecutable del broadcaster TF2
add_executable(wheeled_robot_tf_broadcaster src/wheeled_robot_tf_broadcaster.cpp)
ament_target_dependencies(wheeled_robot_tf_broadcaster rclcpp tf2_ros geometry_msgs)

install(TARGETS
  robotic_arm_tf_broadcaster ackermann_tf_broadcaster wheeled_robot_tf_broadcaster
  DESTINATION lib/${PROJECT_NAME}
)


install(DIRECTORY launch urdf
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
