# First game

## Building Project
    To build the project you must install Vulkan SDK: https://www.lunarg.com/vulkan-sdk/

### For Windows:
```bash
    git clone --recursive -j4 git@github.com:Danikadza/FirstGame.git 
    cd FirstGame
    mkdir build && mkdir bin
    cd build
    cmake -DGS_WIN32 ..
```

### For Linux:
```bash
    git clone --recursive -j4 git@github.com:Danikadza/FirstGame.git 
    cd FirstGame
    mkdir build && mkdir bin
    cd build
    cmake -DGS_LINUX ..
```