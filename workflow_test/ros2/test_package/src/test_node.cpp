#include <iostream>
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  // Initialize ROS2.
  rclcpp::init(argc, argv);

  // Create a ROS2 node.
  auto node = std::make_shared<rclcpp::Node>("test_node");

  // Print a message to the console.
  RCLCPP_INFO(node->get_logger(), "Hello, ROS2!");

  // Shut down the ROS2 system.
  rclcpp::shutdown();

  return 0;
}
