//
// Created by matth on 2024/09/24.
//

#include <GL/glut.h>

void triangle() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3ub(255, 255 / 2, 0);
    glVertex2f(-0.7, -0.5);
    glVertex2f(0.7, -0.5);
    glVertex2f(0, 0.7);
    glEnd();
    glutSwapBuffers();
}

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
