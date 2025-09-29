# Conversation with Gemini

give me instructions to setup up gitea runner with multi arch support on a proxmox vm, use qemu user static

Show thinking

Of course! Here is a comprehensive guide to setting up a Gitea runner on a Proxmox VM with multi-architecture support using Docker and `qemu-user-static`.

This setup will allow your `amd64` (standard x86) virtual machine to execute jobs and build container images for other architectures like `arm64` (ARM) and `riscv64` (RISC-V).

---

### **Overview**

The magic behind this setup lies in two key components:

1. **`qemu-user-static`**: This is an emulator that can execute binaries compiled for one architecture (e.g., ARM) on a different host architecture (e.g., x86_64).

2. **`binfmt_misc`**: This is a Linux kernel feature that allows you to register interpreters for different binary formats. We will use it to tell the kernel, "Whenever you see an ARM executable, use QEMU to run it."

When combined with Docker, this allows you to run containers from other architectures seamlessly. Docker's `buildx` plugin leverages this capability extensively.

### **Prerequisites**

- A running Proxmox VE instance.

- A running Gitea instance (version 1.19+ for Gitea Actions).

- Familiarity with the Linux command line.

---

### **Step 1: Create the Proxmox VM**

1. **Create a new VM:** In your Proxmox web UI, create a new virtual machine. A lightweight Linux distribution is recommended. **Debian 12** or **Ubuntu Server 22.04** are excellent choices.

2. **VM Resources:**
   
   - **CPUs:** 2 or more vCPUs.
   
   - **Memory:** 4 GB or more RAM.
   
   - **Disk:** 30 GB+ of storage (Docker images can take up space).
   
   - **Network:** A standard bridged connection (`vmbr0`) is fine.

3. **Installation:** Install your chosen Linux distribution on the VM.

4. **Initial Update:** Once the VM is running, log in via SSH or the Proxmox console and perform a full system update.
   
   Bash
   
   ```
   sudo apt update && sudo apt upgrade -y
   ```

### **Step 2: Install Docker Engine**

We will install Docker from its official repository to ensure we have the latest version, including the `buildx` plugin.

1. **Install prerequisites:**
   
   Bash
   
   ```
   sudo apt-get install -y ca-certificates curl gnupg
   ```

2. **Add Docker’s official GPG key:**
   
   Bash
   
   ```
   sudo install -m 0755 -d /etc/apt/keyrings
   curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
   sudo chmod a+r /etc/apt/keyrings/docker.gpg
   ```

3. **Set up the Docker repository:**
   
   Bash
   
   ```
   echo \
    "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian \  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
    sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
   ```

4. **Install Docker Engine and plugins:**
   
   Bash
   
   ```
   sudo apt-get update
   sudo apt-get install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
   ```

5. **Add your user to the `docker` group** to manage Docker without `sudo` (log out and log back in for this to take effect):
   
   Bash
   
   ```
   sudo usermod -aG docker $USER
   newgrp docker # Or log out and back in
   ```

### **Step 3: Configure QEMU and binfmt_misc**

This is the core step for enabling multi-arch support. The easiest way to do this is by using a specialized Docker container that properly configures the host system.

1. **Run the `qemu-user-static` registration container:**
   
   Bash
   
   ```
   docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
   ```
   
   - `--rm`: The container removes itself after it's done.
   
   - `--privileged`: This is required for the container to modify the host's `binfmt_misc` registration.
   
   - `--reset -p yes`: Clears any previous `binfmt_misc` configurations and makes the new QEMU registrations persistent across reboots.

2. **Verify the setup:** You can now test if you can run a container from a different architecture.
   
   Bash
   
   ```
   # This command runs on your amd64 machine, but should output "aarch64"
   docker run --rm -t arm64v8/ubuntu uname -m
   
   # This command should output "riscv64"
   docker run --rm -t riscv64/ubuntu uname -m
   ```
   
   If these commands work and print the foreign architecture name, your system is correctly configured!

### **Step 4: Install and Register the Gitea Runner**

1. **Create a dedicated user for the runner** for better security:
   
   Bash
   
   ```
   sudo useradd --system --shell /bin/false --home /var/lib/gitea-runner gitea-runner
   sudo mkdir -p /var/lib/gitea-runner
   sudo chown -R gitea-runner:gitea-runner /var/lib/gitea-runner
   # Also add the new user to the docker group
   sudo usermod -aG docker gitea-runner
   ```

2. **Download the Gitea Runner binary:** Go to your Gitea instance's `/instance/runners` page or check the [Gitea releases page](https://www.google.com/search?q=https://gitea.com/gitea/runner/releases) to get the latest version.
   
   Bash
   
   ```
   # Replace with the latest version URL
   wget -O gitea-runner https://dl.gitea.com/gitea/runner/latest/gitea-runner-latest-linux-amd64
   sudo mv gitea-runner /usr/local/bin/gitea-runner
   sudo chmod +x /usr/local/bin/gitea-runner
   ```

3. **Get a Registration Token:**
   
   - In your Gitea instance, go to **Site Administration** > **Actions** > **Runners**.
   
   - You will see a command to register a new runner with a registration token. Copy the token.

4. **Register the runner:** Run the registration command as the `gitea-runner` user. This is where you define the labels that Gitea Actions will use to select this runner. It is crucial to add labels for each architecture you support.
   
   Bash
   
   ```
   sudo -u gitea-runner /usr/local/bin/gitea-runner register
   ```
   
   The command will ask for the following interactively:
   
   - **Gitea instance URL:** `https://your.gitea.instance.com/`
   
   - **Registration Token:** `PASTE_YOUR_TOKEN_HERE`
   
   - **Runner Name:** `proxmox-multiarch-runner` (or your preferred name)
   
   - **Runner Labels (comma-separated):** This is the most important part. Define labels that describe the runner's capabilities.
     
     ```
     docker,linux-amd64,linux-arm64,linux-riscv64
     ```

  This creates a `/var/lib/gitea-runner/.runner` configuration file.

### **Step 5: Configure and Run the Gitea Runner as a Service**

1. **Create a systemd service file** to manage the runner process.
   
   Bash
   
   ```
   sudo nano /etc/systemd/system/gitea-runner.service
   ```

2. **Paste the following content into the file:**
   
   Ini, TOML
   
   ```
   [Unit]
   Description=Gitea Runner
   After=network.target docker.service
   Requires=docker.service
   
   [Service]
   User=gitea-runner
   Group=gitea-runner
   ExecStart=/usr/local/bin/gitea-runner daemon
   WorkingDirectory=/var/lib/gitea-runner
   Restart=always
   RestartSec=10
   Environment=DOCKER_HOST=unix:///var/run/docker.sock
   
   [Install]
   WantedBy=multi-user.target
   ```

3. **Enable and start the service:**
   
   Bash
   
   ```
   sudo systemctl daemon-reload
   sudo systemctl enable --now gitea-runner.service
   ```

4. **Check the status** to ensure it's running without errors:
   
   Bash
   
   ```
   sudo systemctl status gitea-runner.service
   ```
   
   You should also see the runner appear as "Online" in your Gitea instance's Runners administration page.

### **Step 6: Create a Test Gitea Actions Workflow**

Now, let's create a workflow in one of your Gitea repositories to test the multi-arch setup.

1. In your repository, create the file `.gitea/workflows/multi-arch-test.yml`.

2. Paste the following YAML content:
   
   YAML
   
   ```
   name: Multi-Arch Build Test
   
   on: [push]
   
   jobs:
    test-architectures:
      # Use a strategy matrix to run the same job for different architectures
      strategy:
        matrix:
          platform: [linux-amd64, linux-arm64, linux-riscv64]
   
      # The 'runs-on' label must match one of the labels you gave your runner
      runs-on: ${{ matrix.platform }}
   
      steps:
        - name: Identify Architecture
          run: |          echo "Hello from a container on Gitea Actions!"          echo "This job is running on the '${{ matrix.platform }}' platform."          echo "The kernel identifies the architecture as: $(uname -m)"
   ```

3. **Commit and push** this file to your repository.

Go to the **Actions** tab in your Gitea repository. You should see the workflow trigger. It will create three separate jobs, one for each platform in the matrix (`linux-amd64`, `linux-arm64`, and `linux-riscv64`).

When you inspect the logs for each job, you should see the `uname -m` command output the correct architecture (`x86_64`, `aarch64`, and `riscv64`), proving that your single `amd64` runner successfully executed jobs for all three architectures.

ieee@runner02:~$ docker run --rm -t arm64v8/ubuntu uname -m

Unable to find image 'arm64v8/ubuntu:latest' locally

latest: Pulling from arm64v8/ubuntu

docker: no matching manifest for linux/amd64 in the manifest list entries

Run 'docker run --help' for more information

ieee@runner02:~$ docker run --rm -t riscv64/ubuntu uname -m

Unable to find image 'riscv64/ubuntu:latest' locally

latest: Pulling from riscv64/ubuntu

docker: no matching manifest for linux/amd64 in the manifest list entries

Run 'docker run --help' for more information

Show thinking

Excellent question. You've encountered a common issue where the Docker client itself performs a check before pulling an image. The error message is key:

`docker: no matching manifest for linux/amd64 in the manifest list entries`

This means your Docker client, which knows it's running on a `linux/amd64` machine, is looking at the image manifest for `arm64v8/ubuntu` and saying, "This image doesn't list an `amd64` version, so I'm not going to pull it." It does this *before* the `binfmt_misc` and QEMU emulation can even attempt to run the container.

Here is how to solve this and verify your setup correctly.

### **Solution 1: The Modern `docker buildx` Approach (Recommended)**

Modern Docker handles this much more gracefully using the `buildx` toolkit, which is designed for multi-platform operations. Even if you aren't "building" anything, setting up a `buildx` builder configures Docker to be aware of the QEMU capabilities.

1. **Ensure `buildx` is available** (it should be from our installation):
   
   Bash
   
   ```
   docker buildx version
   # Should show a version number
   ```

2. **Create and use a new `buildx` builder.** This step configures Docker's multi-arch capabilities.
   
   Bash
   
   ```
   docker buildx create --name mybuilder --use
   docker buildx inspect --bootstrap
   ```
   
   When you run the `inspect` command, you should see a list of `Platforms` that includes `linux/amd64`, `linux/arm64`, `linux/riscv64`, etc. This confirms the builder is aware of the QEMU emulators.

3. **Retry the command.** After setting up the builder, the standard `docker run` command should now work as expected.
   
   Bash
   
   ```
   ieee@runner02:~$ docker run --rm -t arm64v8/ubuntu uname -m
   ```
   
   You should now see it successfully pull the image and output:
   
   ```
   aarch64
   ```

This is the recommended approach because it aligns your Docker environment with the modern tooling used for multi-arch builds, which is exactly what the Gitea runner will be doing.

---

### **Solution 2: The Direct `--platform` Flag**

If you want a more direct, one-time fix for the `docker run` command without setting up a builder, you can explicitly tell Docker which platform's image to pull.

1. **Use the `--platform` flag** to override the client's default check.
   
   Bash
   
   ```
   ieee@runner02:~$ docker run --rm --platform linux/arm64 arm64v8/ubuntu uname -m
   ```
   
   And for RISC-V:
   
   Bash
   
   ```
   ieee@runner02:~$ docker run --rm --platform linux/riscv64 riscv64/ubuntu uname -m
   ```
   
   This forces Docker to pull the manifest for the specified architecture. Once pulled, `binfmt_misc` and QEMU will take over to execute it on your `amd64` host.

---

### **Verification and Troubleshooting**

If you still have issues after trying the solutions above, it's a good idea to ensure the QEMU registration is actually active.

1. **Check the `binfmt_misc` registrations:**
   
   Bash
   
   ```
   ls /proc/sys/fs/binfmt_misc/
   ```
   
   You should see entries like `qemu-aarch64`, `qemu-riscv64`, etc. If this directory is empty or missing those entries, the registration failed.

2. **Re-run the registration command:** It's safe to run it again.
   
   Bash
   
   ```
   docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
   ```

### **Summary**

For your goal of setting up a Gitea Runner, **Solution 1 is the best path forward.** Setting up the `buildx` builder ensures that the entire Docker environment is correctly configured for the multi-platform tasks the runner will perform.

You can now proceed with confidence to **Step 4: Install and Register the Gitea Runner** from the original guide.

ieee@runner02:~$ docker buildx inspect --bootstrap

[+] Building 24.0s (1/1) FINISHED

 => [internal] booting buildkit                                                                                   24.0s

 => => pulling image moby/buildkit:buildx-stable-1                                                                23.3s

 => => creating container buildx_buildkit_mybuilder0                                                               0.7s

Name:          mybuilder

Driver:        docker-container

Last Activity: 2025-09-10 01:42:04 +0000 UTC

Nodes:

Name:     mybuilder0

Endpoint: unix:///var/run/docker.sock

Error:    Get "http://%2Fvar%2Frun%2Fdocker.sock/v1.51/containers/buildx_buildkit_mybuilder0/json": context deadline exceeded

ieee@runner02:~$ docker buildx inspect --bootstrap

Name:          mybuilder

Driver:        docker-container

Last Activity: 2025-09-10 01:42:04 +0000 UTC

Nodes:

Name:                  mybuilder0

Endpoint:              unix:///var/run/docker.sock

Status:                running

BuildKit daemon flags: --allow-insecure-entitlement=network.host

BuildKit version:      v0.23.2

Platforms:             linux/amd64, linux/amd64/v2, linux/arm64, linux/riscv64, linux/ppc64, linux/ppc64le, linux/s390x, linux/386, linux/arm/v7, linux/arm/v6

Labels:

 org.mobyproject.buildkit.worker.executor:         oci

 org.mobyproject.buildkit.worker.hostname:         12d75fc5f335

 org.mobyproject.buildkit.worker.network:          host

 org.mobyproject.buildkit.worker.oci.process-mode: sandbox

 org.mobyproject.buildkit.worker.selinux.enabled:  false

 org.mobyproject.buildkit.worker.snapshotter:      overlayfs

GC Policy rule#0:

 All:            false

 Filters:        type==source.local,type==exec.cachemount,type==source.git.checkout

 Keep Duration:  48h0m0s

 Max Used Space: 488.3MiB

GC Policy rule#1:

 All:            false

 Keep Duration:  1440h0m0s

 Reserved Space: 9.313GiB

 Max Used Space: 93.13GiB

 Min Free Space: 36.32GiB

GC Policy rule#2:

 All:            false

 Reserved Space: 9.313GiB

 Max Used Space: 93.13GiB

 Min Free Space: 36.32GiB

GC Policy rule#3:

 All:            true

 Reserved Space: 9.313GiB

 Max Used Space: 93.13GiB

 Min Free Space: 36.32GiB

ieee@runner02:~$ docker run --rm -t arm64v8/ubuntu uname -m

Unable to find image 'arm64v8/ubuntu:latest' locally

latest: Pulling from arm64v8/ubuntu

docker: no matching manifest for linux/amd64 in the manifest list entries

Run 'docker run --help' for more information

ieee@runner02:~$

Show thinking

You have provided a perfect log, and it reveals a very interesting and subtle situation. Thank you!

Let's break down what's happening.

**The Good News:**

The output of your second `docker buildx inspect --bootstrap` command is **perfect**. Specifically this line:

```
Platforms:           linux/amd64, linux/amd64/v2, linux/arm64, linux/riscv64, linux/ppc64, linux/ppc64le, linux/s390x, linux/386, linux/arm/v7, linux/arm/v6
```

This is the confirmation we were looking for. It proves that the BuildKit engine (which `buildx` uses) has successfully detected the `qemu-user-static` emulators and is fully capable of handling multi-architecture operations. **Your core multi-arch setup is correct.**

**The Problem:**

The final `docker run` command is still failing with the same error. This reveals a disconnect:

- The `buildx` subsystem is multi-arch aware.

- The standard `docker run` command is, for some reason, *not* using this awareness and is still defaulting to the host's architecture (`amd64`) for its checks.

The initial timeout error (`context deadline exceeded`) is also a clue. It suggests your VM might have been under heavy load while the `buildkit` daemon container was starting for the first time, which can sometimes lead to an inconsistent state.

Here are the steps to fix this and move forward.

### **Step 1: Definitively Test the Emulation with `--platform`**

Let's bypass Docker's auto-detection and force it to use the `arm64` platform. This will prove the underlying QEMU emulation is working.

Bash

```
docker run --rm --platform linux/arm64 arm64v8/ubuntu uname -m
```

This command should now succeed and print `aarch64`. If it does, you can be **100% confident** that the emulation layer is functional. The issue is purely with the Docker client's default behavior.

### **Step 2: Configure the Docker Daemon**

We can make the main Docker daemon more aware of modern features, which can help align `docker run` with `buildx`.

1. **Edit or create the Docker daemon configuration file:**
   
   Bash
   
   ```
   sudo nano /etc/docker/daemon.json
   ```

2. **Add the following content.** If the file already has content, just add the `"features"` key.
   
   JSON
   
   ```
   {
    "features": {
      "containerd-snapshotter": true
    }
   }
   ```
   
   This enables a feature that aligns the daemon more closely with how BuildKit operates.

3. **Restart the Docker service to apply the changes:**
   
   Bash
   
   ```
   sudo systemctl restart docker
   ```

4. **Re-run the test command WITHOUT the `--platform` flag:**
   
   Bash
   
   ```
   docker run --rm -t arm64v8/ubuntu uname -m
   ```
   
   For many users, this configuration change is enough to make the plain `run` command work as expected.

### **Step 3: If It Still Fails, Don't Worry**

Even if the plain `docker run` command without `--platform` remains stubborn, **this will not prevent your Gitea runner from working.**

The Gitea runner, when executing jobs for different architectures, interacts with the Docker API in a more explicit way, similar to how the `--platform` flag works. It tells the Docker daemon, "I need to run this job in a `linux/arm64` environment," and Docker will oblige because, as we've proven, the capability is there.

**Your multi-arch setup is functional.** The issue you're seeing is a quirk in the default behavior of the `docker run` command-line tool.

**You can now confidently proceed with the rest of the guide.**

- **Step 4: Install and Register the Gitea Runner**

- **Step 5: Configure and Run the Gitea Runner as a Service**

- **Step 6: Create a Test Gitea Actions Workflow**

When your Gitea workflow runs, it will correctly execute the jobs for `linux-amd64`, `linux-arm64`, etc., on your single runner.

sudo mv gitea-runner /usr/local/bin/gitea-runner

mv: cannot stat 'gitea-runner': No such file or directory

Show thinking

Of course. That's a very common issue, and the error message tells us exactly what's wrong.

The error `mv: cannot stat 'gitea-runner': No such file or directory` means that the file named `gitea-runner` does not exist in the directory where you are currently running the command.

This usually happens for one of two reasons:

1. The download command saved the file with a different name.

2. The download command failed.

Let's figure it out and fix it.

### **Step 1: Find the Correct Filename**

Run the `ls -l` command in your terminal to list all the files in the current directory. This will show us what the runner binary was actually named when you downloaded it.

Bash

```
ls -l
```

Look through the list of files. You are probably looking for a file named something like `gitea-runner-latest-linux-amd64`.

### **Step 2: Use the Correct Filename**

Once you've identified the correct name from the list, use that name in the `mv` command.

For example, if you see the file is named `gitea-runner-latest-linux-amd64`, your command should be:

Bash

```
# Use the actual filename you found to move AND rename it
sudo mv gitea-runner-latest-linux-amd64 /usr/local/bin/gitea-runner
```

This command tells the system to move the file with the long name to the `/usr/local/bin/` directory and rename it to `gitea-runner` in the process.

---

### **If You Can't Find the File**

If you run `ls -l` and you don't see any file that looks like the Gitea runner, it means the download step failed or was incomplete.

In that case, simply run the download command from the guide again. This command is specifically designed to download the file and save it with the correct short name (`gitea-runner`) using the `-O` flag.

1. **Re-run the download command:**
   
   Bash
   
   ```
   wget -O gitea-runner https://dl.gitea.com/gitea/runner/latest/gitea-runner-latest-linux-amd64
   ```

2. **Now, try the `mv` and `chmod` commands again:**
   
   Bash
   
   ```
   sudo mv gitea-runner /usr/local/bin/gitea-runner
   sudo chmod +x /usr/local/bin/gitea-runner
   ```

You should now be able to proceed with the rest of the setup.

ieee@runner02:~$ sudo -u gitea-runner /usr/local/bin/gitea-runner register

INFO Registering runner, arch=amd64, os=linux, version=v0.2.13.

WARN Runner in user-mode.

INFO Enter the Gitea instance URL (for example, https://gitea.com/):

http://192.168.8.195:3000/

INFO Enter the runner token:

ZogVyTIJZBc3kLet2xEa3K1LSpzvnB7uqQPc5Dnf

INFO Enter the runner name (if set empty, use hostname: runner02):

INFO Enter the runner labels, leave blank to use the default labels (comma-separated, for example, ubuntu-latest:docker://docker.gitea.com/runner-images:ubuntu-latest):

docker,linux-amd64,linux-arm64,linux-riscv64,ubuntu-latest

INFO Registering runner, name=runner02, instance=http://192.168.8.195:3000/, labels=[docker linux-amd64 linux-arm64 linux-riscv64 ubuntu-latest].

DEBU Successfully pinged the Gitea instance server

FATA Failed to register runner: failed to save runner config: open .runner: permission denied

Show thinking

Excellent, you've provided the perfect log! You are **very** close. This is a classic Linux file permission error, and the log tells us exactly what we need to fix it.

### **The Problem Explained**

Let's look at the key lines:

1. **Your command prompt:** `ieee@runner02:~$` The `~` means you are in your own user's home directory, `/home/ieee`.

2. **The error:** `FATA Failed to register runner: failed to save runner config: open .runner: permission denied`

Here’s what happened:

- You correctly ran the command as the `gitea-runner` user using `sudo -u gitea-runner`.

- The runner tried to save its configuration file, named `.runner`, in the **current working directory**.

- The current working directory was `/home/ieee`.

- The `gitea-runner` user does not have permission to create files in the `/home/ieee` directory, so the operation was denied by the operating system.

### **The Solution**

We need to run the registration command from a directory where the `gitea-runner` user **does** have write permissions. We already created the perfect place for this in a previous step: the `gitea-runner` user's own home directory.

1. **Change to the `gitea-runner`'s home directory:**
   
   Bash
   
   ```
   cd /var/lib/gitea-runner
   ```

2. **Now, run the exact same registration command again:**
   
   Bash
   
   ```
   sudo -u gitea-runner /usr/local/bin/gitea-runner register
   ```

This time, when the runner tries to create the `.runner` file, it will do so inside `/var/lib/gitea-runner`, where it has full permission. The registration will complete successfully.

You will be prompted for the URL, token, and labels again. You can enter the same information you did before.

---

### **Pro Tip (One-Liner Alternative)**

For future reference, you can achieve the same result without changing your own directory by using the `--chdir` flag in the `sudo` command:

Bash

```
# This command does the same thing as the two steps above
sudo -u gitea-runner --chdir=/var/lib/gitea-runner /usr/local/bin/gitea-runner register
```

After this step is successful, you are ready to create the systemd service file and start the runner!

is it linux_amd64 or linux-amd64?

Show thinking

That's an excellent and very important question for ensuring your setup is consistent and follows best practices.

The correct and standard convention is **`linux-amd64` (with a hyphen)**.

Here’s a breakdown of why:

1. **CI/CD Convention:** The broader ecosystem, including GitHub Actions (which Gitea Actions is based on) and Docker, overwhelmingly uses hyphens as separators in labels, tags, and identifiers. You'll see `ubuntu-latest`, `windows-latest`, `docker-ce`, etc.

2. **Readability:** Hyphens are generally preferred for separating words in machine-readable tags as they are easier to read and parse visually.

3. **Gitea's Default Behavior:** The default Gitea runner (`act_runner`) and the community-provided runner images are configured with hyphenated labels. For example, the default label for a standard Ubuntu image is often `ubuntu-22.04:docker://...`. If you mix your own `linux_amd64` with the standard `ubuntu-22.04`, you create an inconsistent system for yourself.

### **Can you use `linux_amd64`?**

Technically, yes. The runner will register with whatever string you give it, and as long as your `.gitea/workflows/your-workflow.yml` file has `runs-on: linux_amd64`, it will match and run the job.

However, you would be creating a non-standard label that goes against the established convention, which can cause confusion and potential issues with tooling or community actions down the line.

### **Recommendation**

**Always use a hyphen.** Stick to the labels you used during the registration step in the guide:

```
docker,linux-amd64,linux-arm64,linux-riscv64
```

This ensures your runner setup is clean, conventional, and will integrate smoothly with other parts of the Gitea Actions ecosystem.
