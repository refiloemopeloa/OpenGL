# A guide to using OpenGL

## Setup

### Requirements

1. **C Compiler**
    1. **Windows**: MingGW
    2. **Linux**: GCC
    3. **macOS**: Clang
2. **OpenGL Libraries**
3. **Window Management Library**
    1. **GLFW**
    2. **GLUT**
4. **Development Environment**
    1. **IDE**
    2. **Text editor**

### Installation

1. **C Compiler**
    1. **Windows**
        1. **MinGW**: https://github.com/skeeto/w64devkit/releases
    2. **Linux**
        1. **GCC**:
            * Usually comes preinstalled. You can check this by typing:
              ```shell
              gcc --version
              ```
            * If the output is version information, then you're good to go.
            * If you get an error or a message that says this package is not installed, you can install gcc by typing:
              ```shell
              sudo apt-get install gcc 
              ```      
    3. **macOS**
        * Comes preinstalled.
2. **OpenGL Libraries**
    1. **Linux**
        * Install using the following command:
            ```shell
            sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
            ```
    2. **Windows**
        * **OpenGL**: Comes with graphics drivers.
        * **GLUT**: https://freeglut.sourceforge.net/index.php#download
        * **GLFW**: https://www.glfw.org/download.html
    3. **macOS**
        * **OpenGL**: Pre-installed.
        * **GLFW**: Install using brew:
            ```shell
            brew install glfw
            ```
3. **Development Enviromnet**
    1. **VSCode**: https://code.visualstudio.com/Download
    2. **Xcode**: https://developer.apple.com/xcode/
    3. **CLion**: https://www.jetbrains.com/clion/download/

### Compiling Code

1. **Linux**

    * Type the following in the terminal in the directory where your code is:

         ```shell
         gcc -o opengl_example opengl_example.c -lGL -lglut -lGLU -lm
 
         ./opengl_example
         ```
2. **Windows**
    * Type the following in the terminal in the directory where your code is:
        ```shell
        gcc -o opengl_example.exe opengl_example.c -lglfw3 -lopengl32 -lglew32 -lgdi32

        opengl_example.exe
        ```

3. **macOS**
    * Type the following in the terminal in the directory where your code is:
        ```shell
        gcc -o opengl_example opengl_example.c -framework GLUT -framework OpenGL -framework Cocoa
 
        ./opengl_example
        ```

## Code

### Demo

Let's begin with a little demo of OpenGL.

1. Create a C file and call it `hello_OpenGL.c`.
2. Open the file in your development environment.
3. 