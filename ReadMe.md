# OpenGL Project

Project for Basics of Computer Graphics course at [FCSE](https://finki.ukim.mk/). It is a striped down version of [Polythonic/Glitter](https://github.com/Polytonic/Glitter) project that includes:
  - [GLAD](https://github.com/Dav1dde/glad) as OpenGL Function Loader
  - [GLFW](https://github.com/glfw/glfw) for Windowing and Input
  - [GLM](https://github.com/g-truc/glm) for OpenGL Mathematics
  - [stb](https://github.com/nothings/stb) for images and texture loading

#

#### This repository is forked and improved upon from: [https://github.com/joksim/OpenGLPrj](https://github.com/joksim/OpenGLPrj)

The master branch is where all the work is done from, it serves as a baseplate for any other code from this project/lab excercises.

## Setup
  Recursively clone the repository:

    git clone --recursive https://github.com/filipgaming008/OpenGL.git
    
  If you are using an IDE that supports CMake builds (QtCreator, Jetbrains CLion, Visual Studio), open the cloned directory as project (the directory `OpenGL` with the `CMakeLists.txt` file).

  We recomend using [out of source builds](https://cgold.readthedocs.io/en/latest/tutorials/out-of-source.html]).

  Project files can be generated using the following steps:

### Using CMake from cmd/git bash/powershell:
  
  - Create a folder for the project build (e.g. build). It can be in the same level/directory as the OpenGLPrj. The hierarchy should be as depicted

        |
        |-- build/
        |-- OpenGL/

  - Using CMake generate the specified project files into the build directory:
  
        cd build
        cmake ../OpenGL/
   
  Open the project files in the spedified IDE (Visual Studio, QtCreator, ...)

###  Using cmake-gui:
     
  Open the `cmake-gui` app. For the source folder select the `OpenGL` directory. For build directory choose an empty directory (for example, directory named `build` at the same level as `OpenGL`. With both folders choosen, click **Configure** and if successfull procede to **Generate** the build files. A tutorial is given at: [https://cgold.readthedocs.io/en/latest/tutorials/cmake-stages.html#](https://cgold.readthedocs.io/en/latest/tutorials/cmake-stages.html#).
  
  
