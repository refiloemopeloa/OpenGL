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

## Demo

Let's begin with a little demo of OpenGL. We are going to display a black window with white borders.

1. Create a C file and call it `hello_OpenGL.c`.
2. Open the file in your development environment.
3. Import the OpenGL library.
    * If you're on Windows or Linux:
        ```C++
        #include <GL/glut.h>
        ```
    * If you're on macOS
        ```C++
        #include <GLUT/glut.h>
        ```
4. Add the following code to your file:
    ```C++
    int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Hello world!");
    glutMainLoop();

    return 0;
    }
    ```
5. Run your code with the following command in the terminal:
    ```shell
    gcc -o hello_OpenGL hello_OpenGL.c -lglut
    ```
6. You should see a black window with white borders in the top left corner of your screen.

## Initialization

### glutInit(&argc, argv)

* Initializes GLUT library.
* Takes arguments from command line.
* Processes any arguments that control GLUT's behaviour.

### glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)

* Sets initial display mode for the window.
    * `GLUT_DOUBLE`:
        * Enables double buffering:
            * Rendering is done on a back buffer, and then it is swapped with the front buffer to display.
        * Prevents flickering during rendering.
    * `GLUT_RGB`:
        * Specifies that the window will use an RGB color model.
    * `GLUT_DEPTH`:
        * Enables depth buffering:
            * Handles depth calculations.
            * Ensure that objects are rendered in the correct order (near objects obscure far objects).

### glutInitWindowSize(width, height)

* Sets the initial size of the window to `width` pixels wide and `height` pixels tall.

### glutCreateWindow(window_title)

* Creates a window with the title `window_title`.
* Returns an integer identifier for the window.

### glClearColor(R, G, B, A)

* Sets the clear color for the window, which is the color used when the color buffers are cleared.
* The values `R`, `G`, `B`, `A` correspond to (RGBA) values.

### glutDisplayFunc(display)

* Registers the display callback function, `display`, which will be called whenever the window needs to be redrawn.
* `display` function contains the code for rendering the scene.

### glutReshapeFunc(reshape)

* Registers the reshape callback function, `reshape`, which is called whenever the window is resized.
* `reshape` function typically adjusts the viewport and projection matrix to ensure the scene is rendered correctly in
  the new window size.

### glutIdleFunc(idle)

* Registers the idle callback function, `idle`, which is called whenever the application is idle (not handling other
  events).
* `idle` function can be used to update animations or perform background tasks.

### glutMainLoop()

* Enters the GLUT event processing loop.
* Listens for events such as keyboard input, mouse movement, and window resizing, and dispatches them to the appropriate
  callback functions.
* Program will remain in this loop until the window is closed or the program is terminated.

## State

The state includes all settings that affect the rendering.

## Pen

The pen is responsible for drawing shapes. The pen is initialized as follows:

```C++
glBegin(primitive);
...
glEnd();
```

## Refresh buffer

Once something has been rendered, you need to refresh the buffer in some way.

* In single buffer display mode, `GLUT_SINGLE`, use the following command after drawing:
    ```C++
    glFlush();
    ```
* In double buffer mode, `GLUT_DOUBLE`, use the following command:
    ```C++
    glutSwapBuffers();
    ```

## Shapes and Colours in OpenGL

### Primitives

* Basic geometric shapes.
    * **Vertex**: simply a point in 3D, given by its x, y, and z coordinates.
        * **Points**: Single vertices.
        * **Lines**: Connecting two vertices. Also called edges.
        * **Triangles**: A polygon with three vertices.
        * **Obselete**: Primitives which are no longer supported.
* Used as the building blocks for creating complex 3D scenes.

#### Drawing vertices

* Vertices are drawn with the following functions:

    ```C++
    glVertex2f(x, y);   //for 2D
    glVertex3f(x,y,z);  //for 3D
    ```

* The size of point primitives can be changed by calling:
    ```C++
    glPointSize(size);  //size specifies diameter of point, in pixels
    ```
* By default, points are squares. You can get circular points by calling:
    ```C++
    glEnable(GL_POINT_SMOOTH);
    ```

#### Drawing line segments

* `GL_LINES`:
    * Draws disconnected line segments.
    * Specify two vertices for each segment that you want to draw.
* `GL_LINE_STRIP`:
    * Draws connected sequences of line segments.
* `GL_LINE_LOOP`:
    * Adds an extra line segment from the final vertex back to the first vertex.

<img src="assets/point-and-line-primitives.png">

* The width for line primitives can be set by calling:
    ```C++
    glLineWidth(width)
    ```
    * The line width is always specified in pixels.
    * It is not subject to scaling by transformations.

#### Drawing triangle segments

* There are three primitives for drawing triangles segments:
    * `GL_TRIANGLES`:
        * Primitive, with nine vertices.
        * With that primitive, every set of three vertices makes a separate triangle.
    * `GL_TRIANGLE_STRIP`:
        * The first three vertices produce a triangle.
        * After that, every new vertex adds another triangle to the strip, connecting the new vertex to the two previous
          vertices.
    * `GL_TRIANGLE_FAN`:
        * the first three vertices make a triangle, and every vertex after that adds anther triangle, but in this case,
          the new triangle is made by connecting the new vertex to the previous vertex and to the very first vertex that
          was specified (vertex “A” in the picture).

<img src="assets/triangle-primitives.png">

### Triangle demo

Lets draw a triangle on our hello_OpenGL demo.

1. Create a function called `triangle()`:

```c++
void triangle() {
    glClear(GL_COLOR_BUFFER_BIT);   //fill the drawng area with a colour that you set
    glBegin(GL_TRIANGLES);  //begin drawing and what primitive to draw
    glColor3ub(255, 255/2, 0);     //what colour the primitive should be drawn in
    glVertex2f(-0.7, -0.5);     //where the first vertex is
    glVertex2f(0.7, -0.5);
    glVertex2f(0, 0.7);
    glEnd();    //close the drawing area
    glutSwapBuffers();  //swap buffers so what is drawn can be displayed
}
```

2. initialize your glut window in `main()`:

```c++
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle");
    glutDisplayFunc(triangle);
    glutMainLoop();

    return 0;
}
```

