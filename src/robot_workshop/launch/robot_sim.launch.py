#!/usr/bin/env python3
"""
Launch file for my_bot in Gazebo Harmonic with ROS 2 bridge
"""

import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, ExecuteProcess
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory


def generate_launch_description():
    # Path to SDF file
    sdf_file = os.path.join(
        get_package_share_directory('robot_workshop'),
        'worlds',
        'my_bot_harmonic.sdf'
    )
    
    # Path to bridge config
    bridge_config = os.path.join(
        get_package_share_directory('robot_workshop'),
        'config',
        'ros_gz_bridge.yaml'
    )

    # Gazebo Sim (Harmonic)
    gz_sim = ExecuteProcess(
        cmd=['gz', 'sim', sdf_file, '-r'],
        output='screen'
    )

    # ROS-Gazebo Bridge
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=[
            '--ros-args',
            '-p',
            f'config_file:={bridge_config}'
        ],
        output='screen'
    )

    return LaunchDescription([
        gz_sim,
        bridge,
    ])