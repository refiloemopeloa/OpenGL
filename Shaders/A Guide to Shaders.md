# A Guide to Shaders

## What is a Shader?

A shader is just a program that runs on your GPU instead of your CPU. Shaders allow us to customize the visual output of a graphics application. 

There are many languages to write shaders in, but for this guide, we will be using the OpenGL Shader Language for Embedded Systems, version 1.0, or GLSL ES 1.0.

Before we get into programming, let us lay a bit of theory on the table.

There are many kinds of shaders, but for now, we'll focus on two. The **vertex** shader and the **fragment** shader.
### Vertex shader

The vertex shader is the first programmable stage in the graphics pipeline. This is because it processes each vertex of the geometry, and it does so individually. Its main responsibilities include:
* Transforming vertex coordinates from model space to screen space.
* Passing per-vertex data like colour, texture coordinates, and normals to the next stage in the pipeline.

### Fragment shader

The fragment shader operates on each fragment that will potentially form part of the final pixel colour in the rendered image. It is also called the pixel shader. It is responsible for:
* Setting the colour of pixels based on various inputs, including data passed from the vertex shader
* Performing complex computations to create effects like texturing, shading, and lighting.

Here is a picture to encapsulate this pipeline:

<img src = "assets/webgl-dataflow.png" style = "background-color: white">

### Variables

While shaders change how our renders look, they don't have the ability to generate new renders. For this reason, we have to pass all the information about our renders to the shaders. We use variables to do this. There are two types, attribute variables and uniform variables.

#### Attribute variables

Attribute variables are used in the vertex shader. They are used to receive per-vertex data from the application on the CPU. This could include vertex coordinates, normals, colours, and texture coordinates. Each vertex processed by the vertex shader gets its set of attribute values.

Examples include:

```c
attribute vec4 a_position;
attribute vec3 a_color;
```

#### Uniform variables

Uniform variables provide a way to pass information to either the vertex or fragment shader. Uniform variables are kind of like constants in the sense that they keep the same value across the execution of all vertices or fragments in a single draw call. They are commonly used to pass transformation matrices, light information, or global settings to the shaders.

Examples include:

```C
uniform mat4 u_modelViewMatrix;
uniform mat4 u_projectionMatrix;
```

#### Varying variable

Varying variables allow us to pass interpolated data from the vertex shaders to the fragment shaders. This typically includes texture coordinates, per-vertex colours, and transformed normals. The rasterizer, for example, interpolates the values output by the vertex shader for each vertex of a primitive to generate the corresponding input for the fragment shader.

To use a varying variable, it should be declared in bother shaders, with the same name and type.

Examples include:

```C
varying vec3 v_color;
```

## Creating a shader

Now that we've gotten some theory out of the way, we can start getting into the fun part. Creating a shader involves declaring your shaders, compiling them, linking them to your program, and attaching them to the OpenGL context for rendering. 

Because of how widely used OpenGL is, it is available on different languages. This means that while the process of loading shaders is the same, the syntax and procedures , may be different. For this guide, we'll focus on doing it with C. This method will work on C++ as well. For other languages, such as JavaScript using WebGL, refer to this guide: [[Shaders in WebGL]].
### Declaring a shader

Before we begin, ensure you've imported `GL/glut.h`. 

First, we need a variable to point to the code of our shader:

```C
const char *vertex_shader_source;
const char *fragment_shader_source;
```

From here there are two methods you can follow:

#### Internal shaders

Internal shaders is when you type your shaders directly in your program, similar to internal CSS. This reduces the complexity of loading your shaders, but also makes it harder to read. You also lose the support of any support from your IDE if you're using one, since it views it as a string, not actual code. 

Let's look at an example:

```C
const char *vertex_shader_source = 
//Declare local variables
"    attribute vec4 a_position;\n"
"    attribute vec3 a_color;\n"
"    uniform mat4 u_modelViewMatrix;\n"
"    uniform mat4 u_projectionMatrix;\n"
"    varying vec3 v_color;\n"
//Define a main method
"void main() {\n"
"    v_color = a_color;\n"
"    gl_Position = u_projectionMatrix * u_modelViewMatrix * a_position;\n"
"}\0";
```

#### External shaders

External shaders is when you write your shaders in a different file to your program. Shader files don't really have extensions, so you can use whatever you'd like. However, as good practice, you can give your shader files the extension `.glsl`. If you'd like to be more specific, you can add the type of shader to the extension, like `.vertex.glsl`. Here's an example:

```C
//file: vertex_shader.glsl

//Declare local variables
    attribute vec4 a_position;
    attribute vec3 a_color;
    uniform mat4 u_modelViewMatrix;
    uniform mat4 u_projectionMatrix;
    varying vec3 v_color;
//Define a main method
void main() {
    v_color = a_color;
    gl_Position = u_projectionMatrix * u_modelViewMatrix * a_position;
}
```

To pass this to C, we need to read from the file. You can use this function to read from the file:

```C
char* read_shader_source(const char* file_path) {

    FILE* file = fopen(file_path, "r");

    if (file == NULL) {

        printf("Failed to open file: %s\n", file_path);

        return NULL;

    }

  

    // Seek to the end of file to determine its size

    fseek(file, 0, SEEK_END);

    long fileSize = ftell(file);

    fseek(file, 0, SEEK_SET);

  

    // Allocate memory for the file content

    char* content = (char*)malloc(fileSize + 1);

    if (content == NULL) {

        printf("Failed to allocate memory for file content\n");

        fclose(file);

        return NULL;

    }

  

    // Read the file content

    size_t bytesRead = fread(content, 1, fileSize, file);

    if (bytesRead < fileSize) {

        printf("Failed to read file content\n");

        free(content);

        fclose(file);

        return NULL;

    }

  

    content[fileSize] = '\0';  // Null-terminate the string

    fclose(file);

    return content;

}
```

Then set your vertex shader pointer to the following:

```C
char *vertex_shader_source = read_shader_source("vertex_shader.glsl");
```

### Compile a shader

To compile a shader, we can call the following function:

```C
GLuint compile_shader(GLenum type, const char *source)

{

    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);

    glCompileShader(shader);

  

    int success;

    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)

    {

        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);

        return 0;

    }

    return shader;

}
```

Then, store the compiled shader in a variable:

```C
GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
```

Once you've compiled your shader, you can free the memory buffer holding the code to your shader:

```C
free(vertex_shader_source);
```

### Linking a shader

Once we've compiled, we can link the shader. First we need to create a shader program:

```C
GLuint shader_program = glCreateProgram();
```

Then, we attach our shaders to the program:

```C
glAttachShader(shader_program, vertex_shader);
```

Finally, we link the program to the shader:

```C
glLinkProgram(shader_program);
```

Lastly, we can free our shaders:

```C
glDeleteShader(vertex_shader);
```

It is good practice to check if there are linking errors with your shaders. You can do this as follows:

```C
int success;

char infoLog[512];

glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

if (!success) {

    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);

    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);

    exit(1);

}
```

### Using our shaders

The last thing we need to do to actually see the effects of our shaders is to call them in our code. This is usually done in the `display` function:

```C
glUseProgram(shader_program);
```

### Cube demo

Now that we've done everything above, we can make an actual program. Create a directory called `shader-cube`, and create three files in it, one called `cube.c`, one called `vertex_shader.glsl` and one called `fragment_shader.glsl`.

Once that is done, you can add the following code to each of the files:

In `main.c`:

```C
#include <GL/glew.h>  
#include <GL/freeglut.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
  
GLuint shader_program;  
GLuint VAO, VBO;  
  
// Function to read shader source from file  
char* read_shader_source(const char* filePath) {  
    FILE* file = fopen(filePath, "r");  
    if (file == NULL) {  
        printf("Failed to open file: %s\n", filePath);  
        return NULL;  
    }  
    fseek(file, 0, SEEK_END);  
    long fileSize = ftell(file);  
    fseek(file, 0, SEEK_SET);  
  
    char* content = (char*)malloc(fileSize + 1);  
    if (content == NULL) {  
        printf("Failed to allocate memory for file content\n");  
        fclose(file);  
        return NULL;  
    }  
    size_t bytesRead = fread(content, 1, fileSize, file);  
    if (bytesRead < fileSize) {  
        printf("Failed to read file content\n");  
        free(content);  
        fclose(file);  
        return NULL;  
    }  
    content[fileSize] = '\0';  
    fclose(file);  
    return content;  
}  
  
// Function to compile shader  
GLuint compile_shader(GLenum type, const char* source) {  
    GLuint shader = glCreateShader(type);  
    glShaderSource(shader, 1, &source, NULL);  
    glCompileShader(shader);  
  
    int success;  
    char infoLog[512];  
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);  
    if (!success) {  
        glGetShaderInfoLog(shader, 512, NULL, infoLog);  
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);  
        return 0;  
    }    return shader;  
}  
  
void init_shaders() {  
    // Read vertex shader from file  
    char* vertex_shader_source = read_shader_source("shaders/vertex.glsl");  
    if (vertex_shader_source == NULL) {  
        exit(1);  
    }  
    // Compile vertex shader  
    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_source);  
    free(vertex_shader_source);  
    if (!vertex_shader) exit(1);  
  
    // Read and compile fragment shader  
    char* fragment_shader_source = read_shader_source("shaders/fragment.glsl");  
    if (fragment_shader_source == NULL) {  
        exit(1);  
    }    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);  
    free(fragment_shader_source);  
    if (!fragment_shader) exit(1);  
  
    // Link shaders  
    shader_program = glCreateProgram();  
    glAttachShader(shader_program, vertex_shader);  
    glAttachShader(shader_program, fragment_shader);  
    glLinkProgram(shader_program);  
  
    // Check for linking errors  
    int success;  
    char infoLog[512];  
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);  
    if (!success) {  
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);  
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);  
        exit(1);  
    }  
    glDeleteShader(vertex_shader);  
    glDeleteShader(fragment_shader);  
}  
  
void init_buffers() {  
    // Set up vertex data (and buffer(s)) and configure vertex attributes  
    float vertices[] = {  
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
  
        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
  
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  
  
        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f  
    };  
  
    glGenVertexArrays(1, &VAO);  
    glGenBuffers(1, &VBO);  
  
    glBindVertexArray(VAO);  
  
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);  
    glEnableVertexAttribArray(0);  
}  
  
void init() {  
    glewInit();  
    glEnable(GL_DEPTH_TEST);  
    init_shaders();  
    init_buffers();  
}  
  
//-------------------- Key-handling functions ---------------------------  
  
  
int rotateX = 0; // Rotations of the cube about the axes.  
int rotateY = 0; //   (Controlled by arrow, PageUp, PageDown keys;  
int rotateZ = 0; //    Home key sets all rotations to 0.)  
  
void doSpecialKey(int key, int x, int y) {  
    // called when a special key is pressed  
    int redraw = 1;  
    if (key == GLUT_KEY_LEFT)  
        rotateY -= 15;  
    else if (key == GLUT_KEY_RIGHT)  
        rotateY += 15;  
    else if (key == GLUT_KEY_DOWN)  
        rotateX += 15;  
    else if (key == GLUT_KEY_UP)  
        rotateX -= 15;  
    else if (key == GLUT_KEY_PAGE_UP)  
        rotateZ += 15;  
    else if (key == GLUT_KEY_PAGE_DOWN)  
        rotateZ -= 15;  
    else if (key == GLUT_KEY_HOME)  
        rotateX = rotateY = rotateZ = 0;  
    else  
        redraw = 0;  
    if (redraw)  
        glutPostRedisplay(); // will repaint the window  
}  
  
  
void draw() {  
    glUseProgram(shader_program);  
  
    // Create and apply the rotation matrix  
    GLfloat modelMatrix[16];  
    glMatrixMode(GL_MODELVIEW);  
    glLoadIdentity();  
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);  
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);  
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);  
    glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);  
  
    // Pass the model matrix to the shader  
    GLint modelLoc = glGetUniformLocation(shader_program, "model");  
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelMatrix);  
  
    glBindVertexArray(VAO);  
    glScalef(0.5,0.5,0.5);  
  
    glDrawArrays(GL_TRIANGLES, 0, 36);  
}  
  
  
  
void display() {  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  
    glUseProgram(shader_program);  
    glBindVertexArray(VAO);  
  
    draw();  
    glutSwapBuffers();  
}  
  
int main(int argc, char** argv) {  
    glutInit(&argc, argv);  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  
    glutInitWindowSize(800, 600);  
    glutCreateWindow("OpenGL Cube with GLUT");  
  
    init();  
  
    glutDisplayFunc(display);  
    glutSpecialFunc(doSpecialKey); // doSpecialKey() is called to process other keys (such as arrows).  
    glutMainLoop();  
  
    return 0;  
}
```

In `vertex.glsl`:

```C
#version 330 core  
layout (location = 0) in vec3 aPos;  
  
uniform mat4 model;  
  
out vec3 FragPos;  
  
void main() {  
    gl_Position = model * vec4(aPos, 2.0);  
    FragPos = aPos;  
}
```

In `fragment.glsl`:

```C
#version 330 core  
out vec4 FragColor;  
    
void main() {  
    vec3 color;  
    color = vec3(1.0, 0.0, 1.0);
    FragColor = vec4(color, 1.0);  
}
```

You should get something like this:

<img src="assets/shader-cube.gif" style="background-color: white">

You now have your cube with shaders determining the vertex positions and the color of the cube.
# References

1. [Chapter 10 The Programmable Pipeline | Computer Graphics and Visualisation (wits.ac.za)](https://courses.ms.wits.ac.za/~branden/CGV/_book/pipe.html)
2. [glDrawArrays - OpenGL 4 Reference Pages (khronos.org)](https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawArrays.xhtml)