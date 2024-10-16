//
// Created by matth on 2024/09/24.
//

#include <stdio.h>
#include <GL/glut.h>


int rotateX = 0; // Rotations of the cube about the axes.
int rotateY = 0; //   (Controlled by arrow, PageUp, PageDown keys;
int rotateZ = 0; //    Home key sets all rotations to 0.)

int vertex_count = 8;

float vertex_array[][3] = {
    {0.5, 0.5, 0}, // vertex 0
    {0.5, 1.0, 0}, // vertex 1
    {1.0, 1.0, 0}, // vertex 2
    {1.0, 0.5, 0}, // vertex 3
    {0.5, 1, 0.5}, // 4
    {0.5, 0.5, 0.5}, // 5
    {1, 1, 0.5}, // 6
    {1, 0.5, 0.5} // 7
};

int outlined_face_count = 6;

int outlined_face_array[][4] = {
    {0, 1, 2, 3}, // 0
    {0, 5, 4, 1}, // 1
    {0, 5, 7, 3}, // 2
    {6, 2, 3, 7}, // 3
    {6, 4, 5, 7}, // 4
    {6, 2, 1, 4} // 5
};

int face_count = 12;

int face_array[][3] = {
    {1, 0, 3}, //face 0
    {1, 2, 3}, // 1
    {0, 5, 4}, // 2
    {0, 1, 4}, // 3
    {2, 1, 4}, // 4
    {2, 6, 4}, // 5
    {7, 3, 2}, // 6
    {7, 6, 2}, // 7
    {5, 4, 6}, // 8
    {5, 7, 6}, // 9
    {3, 7, 5}, // 10
    {3, 0, 5}, // 11
};

float color_array[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0}
};

void outlined_cube() {
    int v;
    float *vCoords;
    glColor3ub(255, 255, 255);
    for (int face = 0; face < outlined_face_count; face++) {
        glBegin(GL_LINE_LOOP);
        // @formatter:off
            for (int vertex = 0; vertex < 4; vertex++) {
                v = outlined_face_array[face][vertex];
                vCoords = vertex_array[v];
                glVertex3fv(vCoords);
            }
        // @formatter:on
        glEnd();
    }
}

void cube() {
    int v, face_colour, face_counter = 0;
    float *vCoords;
    // glColor3ub(255, 255, 255);
    // @formatter:off
        for (int face=0; face< face_count; face++ ) {
            if (face%2==0) {
                face_colour = face_counter%6;
                printf("Face color: %d\n", face_colour);
                glColor3fv(color_array[face_colour]);
                face_counter++;
            }
            glBegin(GL_TRIANGLE_STRIP);
                for (int vertex = 0; vertex < 3; vertex++) {
                    v = face_array[face][vertex];
                    vCoords = vertex_array[v];
                    glVertex3fv(vCoords);
                }
            glEnd();
        }
    // @formatter:on
}

void doSpecialKey(int key, int x, int y) {
    // called when a special key is pressed
    int redraw = 1;
    int rotation_factor = 5;
    if (key == GLUT_KEY_LEFT)
        rotateY -= rotation_factor;
    else if (key == GLUT_KEY_RIGHT)
        rotateY += rotation_factor;
    else if (key == GLUT_KEY_DOWN)
        rotateX += rotation_factor;
    else if (key == GLUT_KEY_UP)
        rotateX -= rotation_factor;
    else if (key == GLUT_KEY_PAGE_UP)
        rotateZ += rotation_factor;
    else if (key == GLUT_KEY_PAGE_DOWN)
        rotateZ -= rotation_factor;
    else if (key == GLUT_KEY_HOME)
        rotateX = rotateY = rotateZ = 0;
    else
        redraw = 0;
    rotateX %= rotation_factor + 1;
    rotateY %= rotation_factor + 1;
    rotateZ %= rotation_factor + 1;
    if (redraw)
        glutPostRedisplay(); // will repaint the window
}

void draw() {
    glRotatef(rotateZ, 0, 0, 1); // Apply rotations to complete object.
    glRotatef(rotateY, 0, 1, 0);
    glRotatef(rotateX, 1, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    // @formatter:off
        glTranslatef(-0.5,-0.5, 0);
        outlined_cube();
        cube();
    // @formatter:on
    glPopMatrix();
    // glDisable(GL_DEPTH_TEST);
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle");
    glutDisplayFunc(draw);
    glutSpecialFunc(doSpecialKey);
    glutMainLoop();

    return 0;
}
