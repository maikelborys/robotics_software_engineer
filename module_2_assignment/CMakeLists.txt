cmake_minimum_required(VERSION 3.8)
project(module_2_assignment)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# Encontrar dependencias
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(circle_drive src/circle_drive.cpp)
ament_target_dependencies(circle_drive rclcpp geometry_msgs)

add_executable(spiral_drive src/spiral_drive.cpp)
ament_target_dependencies(spiral_drive rclcpp geometry_msgs)

add_executable(back_forth src/back_forth.cpp)
ament_target_dependencies(back_forth rclcpp geometry_msgs)

add_executable(led_publisher src/led_publisher.cpp)
ament_target_dependencies(led_publisher rclcpp std_msgs)

add_executable(led_subscriber src/led_subscriber.cpp)
ament_target_dependencies(led_subscriber rclcpp std_msgs)


# ADD TASK 5 Executables here

install(TARGETS
  circle_drive
  spiral_drive
  back_forth
  led_publisher
  led_subscriber
  DESTINATION lib/${PROJECT_NAME})

  install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

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
