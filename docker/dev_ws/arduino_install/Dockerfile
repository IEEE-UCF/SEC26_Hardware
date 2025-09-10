# Start with the ROS 2 Jazzy base image
FROM ros:jazzy-ros-base

# Add build arguments for Gitea host and repository URL with default values
ARG USER_NAME=rosdev
ARG USER_UID=1001
ARG USER_GID=$USER_UID

# Install all necessary dependencies as root
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    python3-pip \
    sudo \
    openssh-client \
    curl \
    unzip \
    ca-certificates \
    wget \
    # Download and install cloudflared
    && wget -q https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-linux-amd64.deb -O cloudflared.deb \
    && apt-get install -y ./cloudflared.deb \
    && rm cloudflared.deb \
    && rm -rf /var/lib/apt/lists/*

# Install and configure arduino-cli as root
# This ensures it can be moved to /usr/local/bin and the data is created under /root
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh \
    && mv bin/arduino-cli /usr/local/bin/arduino-cli \
    && arduino-cli config init \
    && arduino-cli config set board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json \
    && arduino-cli core update-index \
    && arduino-cli core install teensy:avr \
    && arduino-cli core install arduino:avr \
    && arduino-cli core install esp32:esp32

# Create a non-root user
RUN groupadd --gid $USER_GID $USER_NAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USER_NAME \
    && usermod -aG sudo $USER_NAME \
    && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# Install VS Code Server for the new user and set correct ownership
RUN mkdir -p /home/$USER_NAME/.vscode-server/bin \
    && wget -qO- 'https://update.code.visualstudio.com/latest/server-linux-x64/stable' | tar -xz --strip-components 1 -C /home/$USER_NAME/.vscode-server/bin \
    && chown -R $USER_NAME:$USER_NAME /home/$USER_NAME/.vscode-server

# Copy Arduino configuration files from root's home directory to the new user's home directory
# This command is run as root, so it has the necessary permissions.
RUN mv /root/.arduino15 /home/$USER_NAME/ \
    && chown -R $USER_NAME:$USER_NAME /home/$USER_NAME/.arduino15

# Download Linux udev rules for teensy and copy
RUN mkdir -p /etc/udev/rules.d/ \
    && wget https://www.pjrc.com/teensy/00-teensy.rules -O /etc/udev/rules.d/00-teensy.rules

# Switch to the new user and perform remaining setup
USER $USER_NAME
WORKDIR /home/$USER_NAME

# Add ros2 setup to .bashrc for the new user
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> /home/$USER_NAME/.bashrc

# Set ARDUINO_DIRECTORIES_USER environment variable
ENV ARDUINO_DIRECTORIES_USER=/home/$USER_NAME/.arduino15

# micro-ROS for Arduino Installation 
RUN mkdir -p /home/$USER_NAME/Arduino/libraries && \
    cd /home/$USER_NAME/Arduino/libraries && \
    git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_arduino.git && \
    cd micro_ros_arduino && \
    git submodule update --init --recursive

# micro-ROS Teensy patch
RUN TEENSY_PLATFORM_PATH=$(find $ARDUINO_DIRECTORIES_USER/packages/teensy/hardware/avr -name "platform.txt") && \
    if [ -n "$TEENSY_PLATFORM_PATH" ]; then \
        echo "Patching Teensy platform file at $TEENSY_PLATFORM_PATH" && \
        wget https://raw.githubusercontent.com/micro-ROS/micro_ros_arduino/jazzy/extras/patching_boards/platform_teensy.txt -O "$TEENSY_PLATFORM_PATH"; \
    else \
        echo "Warning: Teensy platform.txt not found. Patch skipped."; \
    fi

# Build micro-ROS setup tools using bash explicitly
RUN /bin/bash -c ". /opt/ros/$ROS_DISTRO/setup.bash && \
    mkdir -p /home/$USER_NAME/microros_ws/src && \
    cd /home/$USER_NAME/microros_ws && \
    git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup && \
    sudo apt-get update && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -y && \
    colcon build"

# Source the tools workspace in .bashrc
RUN echo "source /home/$USER_NAME/microros_ws/install/setup.bash" >> /home/$USER_NAME/.bashrc

# Build micro-ROS agent workspace using bash explicitly
RUN /bin/bash -c ". /opt/ros/$ROS_DISTRO/setup.bash && \
    . /home/$USER_NAME/microros_ws/install/setup.bash && \
    cd /home/$USER_NAME && \
    ros2 run micro_ros_setup create_agent_ws.sh && \
    ros2 run micro_ros_setup build_agent.sh"

#  Source the new agent workspace in .bashrc so it's available in new terminals
RUN echo "source /home/$USER_NAME/microros_agent_ws/install/setup.bash" >> /home/$USER_NAME/.bashrc

# Add arduino-cli tools to the PATH for the new user
ENV PATH="/home/rosdev/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin:${PATH}"

# Create default VS Code C/C++ configuration for IntelliSense
RUN mkdir -p /home/rosdev/.vscode \
    && echo '{' > /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '    "configurations": [' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '        {' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "name": "ROS 2 + Arduino",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "includePath": [' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "${workspaceFolder}/**",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "/opt/ros/jazzy/include/**",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "/home/rosdev/.arduino15/packages/**/cores/**",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "/home/rosdev/.arduino15/packages/**/variants/**",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "/home/rosdev/.arduino15/packages/**/libraries/**/src/**",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '                "/home/rosdev/.arduino15/packages/**/tools/**/include/**"' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            ],' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "defines": [],' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "compilerPath": "/usr/bin/gcc",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "cStandard": "c17",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "cppStandard": "c++17",' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '            "intelliSenseMode": "linux-gcc-x64"' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '        }' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '    ],' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '    "version": 4' >> /home/rosdev/.vscode/c_cpp_properties.json \
    && echo '}' >> /home/rosdev/.vscode/c_cpp_properties.json

# Verify installation as the new user
RUN arduino-cli version
RUN arduino-cli core list

# Source ROS 2 entrypoint
CMD ["/ros_entrypoint.sh", "bash"]
