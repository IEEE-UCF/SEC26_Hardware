# Start with the ROS 2 Jazzy base image
FROM ros:jazzy-ros-base

# Add build arguments for user creation
ARG USER_NAME=rosdev
ARG USER_UID=1001
ARG USER_GID=$USER_UID

# Install all necessary dependencies as root in a single layer
RUN apt-get update && apt-get install -y --no-install-recommends \
    # Core tools
    git \
    sudo \
    openssh-client \
    curl \
    unzip \
    ca-certificates \
    wget \
    coreutils \
    python3-pip \
    # --- Recommended Additions ---
    gdb \
    iputils-ping \
    net-tools \
    usbutils \
    build-essential \
    nano \
    yq \
    # Qt/X11 runtime dependencies commonly required by Gazebo/Qt apps (xcb plugin deps)
    libxcb1 \
    libx11-xcb1 \
    libxcb-glx0 \
    libxcb-xinerama0 \
    libxkbcommon-x11-0 \
    libxcb-icccm4 \
    libxcb-image0 \
    libxcb-keysyms1 \
    libxcb-randr0 \
    libxcb-render-util0 \
    # Download and install cloudflared
    && wget -q https://github.com/cloudflare/cloudflared/releases/latest/download/cloudflared-linux-amd64.deb -O cloudflared.deb \
    && apt-get install -y ./cloudflared.deb \
    && rm cloudflared.deb \
    # --- Cleanup ---
    && rm -rf /var/lib/apt/lists/*

# Install arduino-cli binary system-wide as root
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh \
    && mv bin/arduino-cli /usr/local/bin/arduino-cli

# Create a non-root user and grant sudo privileges
RUN groupadd --gid $USER_GID $USER_NAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USER_NAME \
    && usermod -aG sudo $USER_NAME \
    && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

# Download Linux udev rules for teensy
RUN mkdir -p /etc/udev/rules.d/ \
    && wget https://www.pjrc.com/teensy/00-teensy.rules -O /etc/udev/rules.d/00-teensy.rules

# Switch to the new user and perform remaining setup
USER $USER_NAME
WORKDIR /home/$USER_NAME

# Set ARDUINO_DIRECTORIES_USER environment variable
ENV ARDUINO_DIRECTORIES_USER=/home/$USER_NAME/.arduino15

# Initialize and configure arduino-cli, and install board cores in a single layer
RUN arduino-cli config init && \
    arduino-cli config set board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json && \
    arduino-cli core update-index && \
    arduino-cli core install teensy:avr && \
    arduino-cli core install arduino:avr && \
    arduino-cli core install esp32:esp32

# micro-ROS for Arduino Installation (in the standard sketchbook location)
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

RUN mkdir -p /home/$USER_NAME/ros2_workspaces

# Build micro-ROS setup tools inside the dedicated workspace directory
RUN /bin/bash -c ". /opt/ros/$ROS_DISTRO/setup.bash && \
    cd /home/$USER_NAME/ros2_workspaces && \
    git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup && \
    sudo apt-get update && \
    rosdep update && \
    rosdep install --from-paths src --ignore-src -y && \
    colcon build && \
    rm -rf build log && \
    sudo apt-get clean && sudo rm -rf /var/lib/apt/lists/*"

# Build micro-ROS agent inside the dedicated workspace directory
RUN /bin/bash -c ". /opt/ros/$ROS_DISTRO/setup.bash && \
    . /home/$USER_NAME/ros2_workspaces/install/setup.bash && \
    cd /home/$USER_NAME/ros2_workspaces && \
    ros2 run micro_ros_setup create_agent_ws.sh && \
    ros2 run micro_ros_setup build_agent.sh && \
    rm -rf microros_agent_ws/build microros_agent_ws/log"

# Add ROS setup and all workspace sources to .bashrc
RUN echo "" >> /home/$USER_NAME/.bashrc && \
    echo "# Source ROS 2 and micro-ROS workspaces" >> /home/$USER_NAME/.bashrc && \
    echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> /home/$USER_NAME/.bashrc && \
    echo "source /home/$USER_NAME/ros2_workspaces/install/setup.bash" >> /home/$USER_NAME/.bashrc

# Verify Arduino CLI installation (run as the non-root user)
RUN arduino-cli version && arduino-cli core list

# Switch to root to install Gazebo Harmonic and required tools, then switch back
USER root
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl \
    lsb-release \
    gnupg \
    ca-certificates && \
    # Fetch and install OSRF package signing key
    curl -fsSL https://packages.osrfoundation.org/gazebo.gpg -o /usr/share/keyrings/pkgs-osrf-archive-keyring.gpg && \
    # Add Gazebo (OSRF) apt repository for the current distro
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/pkgs-osrf-archive-keyring.gpg] https://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" > /etc/apt/sources.list.d/gazebo-stable.list && \
    apt-get update && \
    # Install Gazebo harmonic and needed Qt/X11/XCB/OpenGL libs to support GUI
    apt-get install -y --no-install-recommends \
    gz-harmonic && \
    # Cleanup apt caches
    apt-get clean && rm -rf /var/lib/apt/lists/*

USER $USER_NAME
    
# Source ROS 2 entrypoint
CMD ["/ros_entrypoint.sh", "bash"]