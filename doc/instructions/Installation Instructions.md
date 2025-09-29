# Installation Instructions

All necessary applications to download and work with the repo will be listed here.

## Software Overview

1. Install VSCode
2. Install Git Bash on Windows, or Git on any other OS
3. Add Git Bash to environment variables
4. Install Git-LFS
5. Install Cloudflared
6. Setup SSH keys for both Github and Gitea
7. Add proxy commands to your SSH config file
8. Clone repo
9. Install Docker Desktop
10. Install VSCode Extensions (Bolded are must-haves)
    - **Dev Containers**
    - **Remote SSH**

### A foreword...

Please note that these instructions are **only for windows**. Things may be different for Mac, or Ubuntu. Toss this whole page into your favorite LLM for OS-specific instructions.

If you are using windows, make sure that you are using **Windows Terminal** and NOT PowerShell.

You will need to know the following commands: `cd`, `dir`, `ls` to navigate this tutorial comfortably.

If you need help, use an LLM first. Then ask.

### 1. Install VSCode

VSCode can be downloaded from https://code.visualstudio.com/download.

### 2. Git Bash

Git bash can be downloaded from https://git-scm.com/downloads. Defaults are fine. 

* If asked, use VSCode as the default editor. This can be configured later.

### 3. Add Git Bash to Environment Variables

Essentially, Git Bash operates similar to Linux which feels a LOT better than the native Windows Terminal commands. To use Git Bash commands in Windows Terminal, we include the path to the Git Bash in our `$PATH` system variable.

In the Windows Search Bar, search for "Git Bash"" and start it.

Type the command: `where ls`

Verify that the output is `C:\Program Files\Git\usr\bin\**`.

In the Windows Search Bar, Search for "edit the system environment variables" and start it.

In the "advanced" tab, press "environment variables".

There are two sections in the window. In the top section, select "PATH" and press Edit.

Add a new entry for `C:\Program Files\Git\usr\bin\`.

Hit "ok" on **ALL 3 SCREENS***

Restart your Windows Terminal.

### 4. Install Git LFS

Git LFS instructions are found at [Git-LFS](https://git-lfs.com/).

### 5. Cloudflared

We require a reverse proxy to interface with our Gitea server, as we are using a reverse proxy to push our server to the internet. Therefore, Cloudflared must be used.

Cloudflared installation instructions are found at https://developers.cloudflare.com/cloudflare-one/connections/connect-networks/downloads/.

For Windows, run

```
winget install --id Cloudflare.cloudflared
```

You can double check if it's installed correctly with `where cloudflared`.

### 6. SSH Key setup

Gitea needs a SSH key to authenticate Git on your machine.

1. Generate an SSH key. If you have an existing one, skip this step. If prompted for anything, just hit enter to use the defaults.
   
   - `ssh-keygen -t ed25519 -C "your_email@example.com"`

2. Display your public key.
   
   - `cat ~/.ssh/id_ed25519.pub`

3. Add the public key to your profile. Click your **User Icon > Settings > User Settings > SSH/GPG Keys > Add Key.** Copy the public key and name it whatever you want. To copy in a terminal, you can 
   use `Ctrl+Shift+C`

For Github, you can use the following resource: https://www.geeksforgeeks.org/git/how-to-add-ssh-key-to-your-github-account/

### 7. Reverse Proxy Setup

1. Create the file: `.ssh/config` (if it exists, don't overwrite it)
   
   **This must be an extensionless file. If this is a .txt file, this will not work.**
   
   Add the following contents:
   
   ```
   Host sshgitea.syndric.org
   ProxyCommand "C:\Program Files (x86)\cloudflared\cloudflared.exe" access ssh --hostname %h### Instructions:
   
   ```

### 8. Clone Repo

Navigate to the folder you want to clone the repo in.

`git clone --recurse-submodules gitea@sshgitea.syndric.org:ieeeucf/SEC26_Hardware.git`

You may have to request proper permissions to clone the repo.

### 9. Docker Desktop

You can find Docker [here](https://www.docker.com/products/docker-desktop/).

### 10. Install VSCode Extensions

* Dev containers is used to build and run docker development images.

* Remote SSH is used if you ever need to run vscode on a device remotely.


