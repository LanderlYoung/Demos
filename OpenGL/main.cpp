#include "index.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

constexpr auto WINDOW_SIZE = 400;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1.0, -1, 1.0);
}




int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
    glutCreateWindow("An Example OpenGL Program");

    init();
    glutDisplayFunc(polygonStipple);
    glutMainLoop();
    return 0;
}

#pragma clang diagnostic pop