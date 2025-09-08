cmake_minimum_required(VERSION 3.10)
project(test_package)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(test_node src/test_node.cpp) # Adjust the source file as needed
ament_target_dependencies(test_node rclcpp)

# Install binaries
install(TARGETS
  test_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install headers (if any)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY launch/ DESTINATION share/${PROJECT_NAME}/launch)
install(DIRECTORY config/ DESTINATION share/${PROJECT_NAME}/config)

ament_package()