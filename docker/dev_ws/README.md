# ROS 2 Jazzy & Arduino devcontainer

This image is for ROS 2 (jazzy), micro-ROS/Arduino development, and Gazebo with GUI support.

Usage (VS Code devcontainer - recommended)
1. Open the project in VS Code.
2. Command Palette → Remote-Containers: Reopen in Container (or Rebuild Container after Dockerfile changes).

Manual build & run (optional)
```bash
# from docker/dev_ws
docker build -t ros-dev-environment -f Dockerfile ..
docker run -it --rm --privileged -v /dev:/dev \
    -e DISPLAY=host.docker.internal:0 -e QT_X11_NO_MITSHM=1 \
    -v "$(pwd)/..:/home/rosdev/ros2_workspaces/SEC26_Hardware:cached" \
    ros-dev-environment /bin/bash
```

Windows GUI (VcXsrv only)
- Install and run VcXsrv on Windows. Use display number 0 and (for quick testing) disable access control.
- The devcontainer sets `DISPLAY=host.docker.internal:0` and `QT_X11_NO_MITSHM=1` so Qt/Gazebo work over TCP to VcXsrv.
- If GUI fails: ensure VcXsrv is running and Windows firewall allows port 6000.

Notes
- Post-start script `scripts/install-dependencies.sh` runs inside the container; it requires `yq` and `arduino-cli` (the Dockerfile installs these).
- `QT_X11_NO_MITSHM=1` disables MIT-SHM so Qt works with a remote X server (works reliably with VcXsrv).

That's it — use VS Code Remote - Containers for the smoothest experience.
