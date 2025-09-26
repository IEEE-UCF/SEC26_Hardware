## ROS 2 & Arduino Development Environment

This Dockerfile creates a self-contained development environment for ROS 2 and Arduino projects. It's designed to provide a consistent and isolated workspace for building and testing code for both platforms.

### Features

- **ROS 2 Jazzy:** The base image is `ros:jazzy-ros-base`, providing the core ROS 2 environment.

- **Arduino CLI:** The latest version of the Arduino CLI is installed for compiling and uploading sketches.

- **Board Support:** Includes board packages for Teensy (teensy:avr), Arduino (arduino:avr), and ESP32 (esp32:esp32@3.2.1).

- **VS Code Server:** The container is pre-configured with VS Code Server, allowing you to use the powerful "Remote - Containers" extension for a seamless development experience.

- **Pre-configured IntelliSense:** A `c_cpp_properties.json` file is set up to provide code IntelliSense for both ROS 2 and Arduino libraries.

- **Non-root User:** All development work is performed as a non-root user (`rosdev`), which is a best practice for security.

### How to Use

#### 1. Build the Docker Image

Navigate to the directory containing the `Dockerfile` and run the following command to build the image. This may take a few minutes as it downloads all the necessary components.

```
docker build -t ros-dev-environment .
```

#### 2. Run the Container

Once the image is built, you can run a container from it. The following command starts the container and maps the host's current directory to the container's workspace. This is useful for working on projects located on your host machine.

```
docker run -it --rm \
    --name ros-dev-container \
    -v "$(pwd)":/home/rosdev/ros2_ws/src/my_project \
    -w /home/rosdev/ros2_ws/src/my_project \
    ros-dev-environment
```

**Note:**

- `--rm`: Automatically removes the container when it exits.

- `-it`: Keeps the container interactive.

- `--name`: Assigns a name to the container for easy management.

- `-v "$(pwd)":/home/rosdev/ros2_ws/src/my_project`: Mounts your current host directory into the container's workspace.

- `-w /home/rosdev/ros2_ws/src/my_project`: Sets the working directory inside the container.

#### 3. Connect with VS Code

The most powerful way to use this environment is with the VS Code "Remote - Containers" extension.

1. Start a container as described in Step 2.

2. In VS Code, open the **Command Palette** (`Ctrl+Shift+P` or `Cmd+Shift+P`).

3. Type and select `Remote-Containers: Attach to Running Container...`.

4. Choose the container you just started (e.g., `ros-dev-container`).

VS Code will connect to the container, and you will have a fully functional development environment, with access to all the installed tools and your local project files.

### Working with Arduino CLI

Inside the container, you can use the `arduino-cli` command directly.

#### Compile a Sketch

To compile a sketch, you'll need to specify the board type.

```
arduino-cli compile --fqbn esp32:esp32:esp32 my_sketch_directory
```

#### List Installed Boards

To see all installed board packages, run:

```
arduino-cli core list
```
