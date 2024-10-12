/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-07-10
* Time:   19:37
* Life with Passion, Code with Creativity.
* </pre>
*/

#include "index.h"

void color() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f, 1.0f, 1.0f);
    glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
    glFlush();
}

void colorPlate() {
    int i;
    for (i = 0; i < 8; ++i) {
        // auxSetOneColor(i, (float) (i & 0x04), (float) (i & 0x02), (float) (i & 0x01));
    }
    glShadeModel(GL_FLAT);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (i = 0; i <= 8; ++i) {
        glIndexi(i);
        glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();
    glFlush();
}

void colorShadeModel() {
    // glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLE_FAN);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(0.0f, 0.0f);
        for (auto i = 0; i <= 8; ++i) {
            glColor3f(i & 0x04, i & 0x02, i & 0x01);
            glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
        }
        glEnd();
    }
    glFlush();
}

