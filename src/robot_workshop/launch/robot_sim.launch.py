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

import os
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition
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

    nav_sim_cofig = os.path.join(
        get_package_share_directory('robot_workshop'),
        'config',
        'nav_sim.yaml'
    )

    arena_cofig = os.path.join(
        get_package_share_directory('robot_workshop'),
        'maps',
        'arena_layout.yaml'
    )

    map = DeclareLaunchArgument(
            'map',
            default_value=arena_cofig,
            description='path to the map yaml file')

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


    # turtlesim = Node(
    #     package = 'turtlesim',
    #     executable='turtlesim_node',
    #     name='turtlesim',
    #     output='screen',
    # )

    map_server = Node(
        package='nav2_map_server',
        executable='map_server',
        name='map_server_node',
        output='screen',
        parameters=[nav_sim_cofig],
    )

    # pathing_node = Node(
    #     package='robot_workshop',
    #     executable='pathing_node',
    #     name='pathing_node',
    #     output='screen',
    #     parameters=[{
    #         'odom_topic': '/odom',
    #         'cmd_vel_topic': '/diff_cont/cmd_vel_unstamped',
    #         'goal_xy': [4.0, 3.0],           # set your goal here or via params
    #         'control_rate_hz': 20.0
    #     }],
    #     # condition=IfCondition(turtlesim)
    # )

    return LaunchDescription([
        gz_sim,
        bridge,
        map,
        map_server,
        # turtlesim,
        # pathing_node,
        # arena_cofig,
        # nav_sim_cofig,
    ])