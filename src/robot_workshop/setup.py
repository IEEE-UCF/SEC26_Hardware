from setuptools import setup, find_packages
from glob import glob
import os

package_name = 'robot_workshop'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        # Install launch files
        (os.path.join('share', package_name, 'launch'),
            glob('launch/*.py')),
        # Install world files
        (os.path.join('share', package_name, 'worlds'),
            glob('worlds/*.sdf')),
        # Install config files
        (os.path.join('share', package_name, 'config'),
            glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Your Name',
    maintainer_email='your.email@example.com',
    description='Gazebo Harmonic simulation for my_bot with ROS 2 Jazzy',
    license='MIT',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'teleop_pid = robot_workshop.teleop_pid:main',
            'detector_node = robot_workshop.detector_node:main',
        ],
    },
)