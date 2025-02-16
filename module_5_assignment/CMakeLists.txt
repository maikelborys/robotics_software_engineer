cmake_minimum_required(VERSION 3.8)
project(module_5_assignment)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(EIGEN_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../../extern/eigen)
find_package(Eigen3 REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)  # added to find visualization_msgs package

add_executable(lineFollowing src/line_following_with_camera.cpp)
ament_target_dependencies(lineFollowing rclcpp sensor_msgs cv_bridge geometry_msgs)
target_link_libraries(lineFollowing ${OpenCV_LIBS})

add_executable(goal_planer src/goal_planer.cpp)
ament_target_dependencies(goal_planer rclcpp tf2_ros geometry_msgs nav_msgs)

add_executable(robotic_arm_tf_broadcaster src/robotic_arm_tf_broadcaster.cpp)
ament_target_dependencies(robotic_arm_tf_broadcaster rclcpp tf2_ros geometry_msgs sensor_msgs)


add_library(lqr_lib STATIC src/lqr_lib.cpp)
target_include_directories(lqr_lib PRIVATE ${EIGEN_INCLUDE_DIR} include)
target_link_libraries(lqr_lib Eigen3::Eigen)

add_executable(lqr_node src/lqr_node.cpp)
target_link_libraries(lqr_node lqr_lib Eigen3::Eigen)
ament_target_dependencies(lqr_node rclcpp nav_msgs geometry_msgs tf2 visualization_msgs)  # added visualization_msgs
target_include_directories(lqr_node PRIVATE ${EIGEN_INCLUDE_DIR} include)


install(TARGETS lineFollowing goal_planer robotic_arm_tf_broadcaster 
  lqr_lib
  lqr_node
  DESTINATION lib/${PROJECT_NAME})



install(DIRECTORY launch urdf config
  DESTINATION share/${PROJECT_NAME}/
)



ament_package()
