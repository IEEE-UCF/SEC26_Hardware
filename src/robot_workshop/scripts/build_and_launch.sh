# creating the file with content
cat > scripts/build_and_launch.sh << 'EOF'
#!/bin/bash
set -e

echo "Building robot_workshop..."
cd ~/ros2_workspaces
colcon build --packages-select robot_workshop --symlink-install

echo "Sourcing workspace..."
source install/setup.bash

echo "Launching simulation..."
ros2 launch robot_workshop robot_sim.launch.py
EOF

# making the script executable
chmod +x scripts/build_and_launch.sh