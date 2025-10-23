#!/usr/bin/env python3

# MAKE SURE YOU HAVE ALL INFO INSTALLED:
# source /opt/ros/jazzy/setup.bash
# colcon build --symlink-install --packages-select robot_workshop
# source install/local_setup.bash
# ros2 pkg executables robot_workshop


# TO RUN: 
# 1) IMPORT MODEL (Mustard bottle):
# ros2 run ros_gz_sim create   -world default   -name mustard_bottle   -file '/home/rosdev/ros2_workspaces/src/sec26_hardware/src/robot_workshop/worlds/model.sdf'   -x 0.7 -y 0.0 -z 0.15 -Y 0.0

# 2) RUN GAZEBO (SEPERATE TERMINAL):
# ros2 launch robot_workshop robot_sim.launch.py image_topic:=/camera2/image_raw

# 3) SEE WHAT ROBOT SEES (OPTIONAL)
# ros2 run image_tools showimage --ros-args -r image:=/camera2/image_raw

import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile, QoSReliabilityPolicy
from sensor_msgs.msg import Image
from vision_msgs.msg import Detection2DArray
from cv_bridge import CvBridge
from .duck_detector import process_frame

class DetectorNode(Node):
    """DetectorNode class"""
    def __init__(self):
        """initalize all parameters, logging info and inputs and outputs"""
        super().__init__('detector_node')

        # Parameters
        self.declare_parameter('image_topic', '/camera/image_raw')
        self.declare_parameter('class_name', 'yellow_object')
        self.declare_parameter('pub_topic', '/detected_objects')
        self.declare_parameter('pub_debug_image', '/vision/debug_image')
        self.declare_parameter('debug_viz', True)

        # read parameters
        self.class_name = self.get_parameter('class_name').value
        image_topic     = self.get_parameter('image_topic').value
        self.pub_topic  = self.get_parameter('pub_topic').value
        self.debug_viz  = self.get_parameter('debug_viz').value
        self.pub_dbg_name = self.get_parameter('pub_debug_image').value

        # ROS Inputs and outputs
        qos = QoSProfile(depth=5, reliability=QoSReliabilityPolicy.BEST_EFFORT)
        self.bridge  = CvBridge()
        self.sub     = self.create_subscription(Image, image_topic, self.on_image, qos)
        self.pub_det = self.create_publisher(Detection2DArray, self.pub_topic, 10)
        self.pub_dbg = self.create_publisher(Image, self.pub_dbg_name, 10) if self.debug_viz else None

        # Logging info
        self.get_logger().info(f"Using: {image_topic}")
        self.get_logger().info(f"Detections: {self.pub_topic}")
        if self.debug_viz:
            self.get_logger().info(f"Debug image: {self.pub_dbg_name}")

    def on_image(self, msg: Image):
        """Call function and read the metadata/debuging details"""
        # Use OpenCV in ROS2
        frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')

        # detector
        metadata, debug = process_frame(frame)

        # build detection array
        arr = Detection2DArray() 
        # time stames of camera
        arr.header = msg.header
        for meta in metadata:
            x, y = meta["Center Position"]
            w, h = meta["Size"]
            score = meta["Label"]

            self.get_logger().info(
                f"[Detection - {self.class_name}] "
                f"pos=({x:.1f}, {y:.1f}) size=({w:.1f}, {h:.1f}) conf={score:.2f}"
            )

        if self.pub_dbg and debug is not None:
            self.pub_dbg.publish(self.bridge.cv2_to_imgmsg(debug, encoding='bgr8'))
            self.get_logger().info(
                f"[DEBUG IMG] Published annotated frame with {len(metadata)} detections to: {self.pub_dbg_name}"
            )

        

def main():
    rclpy.init()
    node = DetectorNode() 

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Detector interrupted, shutting down')
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()
