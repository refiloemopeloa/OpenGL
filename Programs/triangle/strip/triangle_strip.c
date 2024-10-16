//
// Created by matth on 2024/09/24.
//

#include <GL/freeglut.h>
#include <GL/gl.h>

void trapezoid() {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(255, 255 / 2, 0);
    // @formatter:off
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
        glVertex2f(-0.2f, -0.5f); // Vertex 4
        glVertex2f(0.0f, 0.5f); // Vertex 5
    // @formatter:on
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 255, 255);
    // @formatter:off
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
        glVertex2f(-0.2f, -0.5f); // Vertex 4
        glVertex2f(0.0f, 0.5f); // Vertex 5
    // @formatter:on
    glEnd();
}

void triangle() {
    // @formatter:off
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(255, 255 / 2, 0);
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 255, 255);
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
    glEnd();
    // @formatter:on
}

void parallelogram() {
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(255, 255 / 2, 0);
    // @formatter:off
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
        glVertex2f(-0.2f, -0.5f); // Vertex 4
    // @formatter:on
    glEnd();
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    glColor3ub(255, 255, 255);
    // @formatter:off
        glVertex2f(-0.8f, 0.5f); // Vertex 1
        glVertex2f(-0.6f, -0.5f); // Vertex 2
        glVertex2f(-0.4f, 0.5f); // Vertex 3
        glVertex2f(-0.2f, -0.5f); // Vertex 4
    // @formatter:on
    glEnd();
}


void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        // @formatter:off
        glScalef(0.8, 0.8, 0.8);
        glTranslatef(-0.3,0,0);
        trapezoid();
        glPushMatrix();
        glTranslatef(1, 0, 0);
            triangle();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(1.6, 0, 0);
            parallelogram();
        glPopMatrix();
        // @formatter:off
    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(1400, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Triangle");
    glutDisplayFunc(draw);
    glutMainLoop();

    return 0;
}
