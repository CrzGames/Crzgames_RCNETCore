# 📋 Prerequisites

Welcome to the RCNET framework configuration guide. This guide will walk you through the steps of setting up your development environment.

## 🚀 Setup Project

### Create a new GitHub Repository

1. Go to GitHub to create a new repository: [Create New Repository](https://github.com/new)
2. Under 'Repository Template', select: `CrzGames/Crzgames_RCNETFramework`
3. Check : `Include all branches`
4. Choose a name for your GitHub repository.
5. Click the button to create the repository.

::: details Click me to view
![An image](/images/prerequisites1.png)
:::

<br />

### Clone the Project

To start working with the game template, clone the project repository and initialize all submodules using the following commands:

```bash
git clone --recurse-submodules git@github.com:CrzGames/your_project_name.git
```

Ensure to replace `your_project_name` with the actual name of your project repository.

::: warning IMPORTANT
Make sure you do a recursive clone! The submodules contain the dependencies, if you have already cloned the project without the submodules, use this command
```bash
git submodule update --init --recursive
```
:::

<br /><br />

### 🐧 Linux Setup

::: details Click me to view
#### Requirements
- System version : glibc 3.25 or higher (Ubuntu 22.04+, Debian 12.0+)
- System architectures : x86_64
- A C++17 compiler (GCC 8.0 or newer)

<br />

#### Installation Steps
1. Add your user to the `sudo` group if necessary :
```bash
sudo usermod -aG sudo <username>
```
2. **Homebrew:** Facilitate package management by installing Homebrew.
```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
3. **Build Essentials and GCC:** Install necessary build tools and GCC.
```bash
sudo apt-get update
sudo apt-get install -y build-essential && brew install gcc
```
4. **CMake:** Make sure CMake 3.25 or newer is installed via Homebrew.
```bash
brew install cmake
```
:::