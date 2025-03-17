# Deer
Deer is a game engine currently in development by Chewico, focused on facilitating voxel-based scenarios.


# Usage
## DeerStudio
DeerStudio is the program used to create games with Deer. When executed, the first thing DeerStudio will do is prompt for the folder of the Deer project. For example, there is an example project named roe/. Simply select the folder containing roe, and you will be able to run DeerStudio.

# How to Compile
Deer can compile for both Linux and Windows. Mac support is not considered due to philosophical and technical reasons.
Deer is built using Premake 5, which generates project files for various build systems such as Make and MSVC.

You can check out Premake at:

- [Premake Official Website](https://premake.github.io)  
- [Premake Core Source Code](https://github.com/premake/premake-core)  

## Linux
#### Clone the repo
```console
git clone https://github.com/ChewicoGIT/Deer
cd Deer
```

## PreRequisites
You will need clang to build the project, as well as the following libraries:

#### Debian
```console
sudo apt update
sudo apt install clang libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev libgtk-3-dev
```

To build the make files you can compile Premake5 (Preferred) or Download the bin from the official website [Premake Bin](https://premake.github.io/download).

### Compile Premake5(Optional)
Premake5 is the build system for Deer. It is needed to generate the Makefiles necessary to build the project.

Premake5 requires the uuid-dev package:

#### Debian
```console
sudo apt-get install uuid-dev
```

```console
cd vendor
git clone https://github.com/premake/premake-core.git
cd premake-core/
git submodule init
git submodule update
make -f Bootstrap.mak linux
cd ..
```

Run
```console
./bin/linux-x86_64/Debug/DeerStudio/DeerStudio
```

### Compile Project
If you have downloaded the Premake5 binary, replace ./vendor/premake-core/bin/release/premake5 with your Premake5 binary location. Make sure to be in the project root when executing commands.

### Compile in relase mode
```console
./vendor/premake-core/bin/release/premake5 gmake
make
```

#### Compile in debug mode
```console
./vendor/premake-core/bin/release/premake5 gmake
make config=debug
```


### Setup VsCode(Optional)
I recomend install the C/C++ extension From Microsoft in VsCode if not present

```console
cd vendor
./vendor/premake-core/bin/release/premake5 vscode
code Deer.code-workspace
```