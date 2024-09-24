//
// Created by matth on 2024/09/24.
//

#include <GL/glut.h>

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Hello world!");
    glutMainLoop();

    return 0;
}
