#include <iostream>
#include <GLUT/glut.h>
#include <cmath>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

constexpr auto WINDOW_SIZE = 400;

void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1.0, -1, 1.0);
}

void lineSegment()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);
    GLint p1[] = {30, 100};
    GLint p2[] = {20, 150};
    GLint p3[] = {0, 0};

    glBegin(GL_LINE_LOOP);
    {
        glVertex2iv(p1);
        glVertex2iv(p2);
        glVertex2iv(p3);
    }
    glEnd();

    glFlush();
}

void drawCircle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    constexpr auto N = 100;
    constexpr auto R = 50;
    constexpr auto center = std::make_pair(100, 100);
    auto pi = std::atan(1) * 4;

    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    for (auto i = 0; i < N; i++) {
        auto radius = 2 * pi * i / N;
        auto x = center.first + R * std::cos(radius);
        auto y = center.second + R * std::sin(radius);
        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}

void drawSinWave()
{
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr auto N = 200;
    constexpr auto origin = std::make_pair(1.0 / 2.0, 1.0 / 2.0);
    auto pi = std::atan(1) * 4;
    constexpr auto T = 1.0 / 4.0;
    constexpr auto H = 50.0;

    glColor3i(0, 0, 0);
    glBegin(GL_LINES);
    {
        glVertex2d(0, origin.second);
        glVertex2d(1.0, origin.second);
        glVertex2d(origin.first, 0);
        glVertex2d(origin.first, 1.0);
    }
    glEnd();

    glColor3i(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    {
        for (auto i = 0; i < N; i++) {
            auto x = 1.0 * i / (double) N;
            auto y = origin.second + H * std::sin((x - origin.first) * 2 * pi / T);
            glVertex2d(x, y);
        }
    }
    glEnd();

    glFlush();
}

void polygonFace()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3i(0, 0, 0);

    glPolygonMode(GL_FRONT, GL_FILL); // 设置正面为填充模式
    glPolygonMode(GL_BACK, GL_LINE);   // 设置反面为线形模式

    glFrontFace(GL_CCW);               // 设置逆时针方向为正面

    glBegin(GL_POLYGON);               // 按逆时针绘制一个正方形，在左下方
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();

    glBegin(GL_POLYGON);               // 按顺时针绘制一个正方形，在右上方
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.0f);
    glEnd();

    glFlush();
}

void polygonStipple()
{
    static GLubyte Mask[128] = {
            0x00, 0x00, 0x00, 0x00,    //   这是最下面的一行
            0x00, 0x00, 0x00, 0x00,
            0x03, 0x80, 0x01, 0xC0,    //   麻
            0x06, 0xC0, 0x03, 0x60,    //   烦
            0x04, 0x60, 0x06, 0x20,    //   的
            0x04, 0x30, 0x0C, 0x20,    //   初
            0x04, 0x18, 0x18, 0x20,    //   始
            0x04, 0x0C, 0x30, 0x20,    //   化
            0x04, 0x06, 0x60, 0x20,    //   ，
            0x44, 0x03, 0xC0, 0x22,    //   不
            0x44, 0x01, 0x80, 0x22,    //   建
            0x44, 0x01, 0x80, 0x22,    //   议
            0x44, 0x01, 0x80, 0x22,    //   使
            0x44, 0x01, 0x80, 0x22,    //   用
            0x44, 0x01, 0x80, 0x22,
            0x44, 0x01, 0x80, 0x22,
            0x66, 0x01, 0x80, 0x66,
            0x33, 0x01, 0x80, 0xCC,
            0x19, 0x81, 0x81, 0x98,
            0x0C, 0xC1, 0x83, 0x30,
            0x07, 0xE1, 0x87, 0xE0,
            0x03, 0x3F, 0xFC, 0xC0,
            0x03, 0x31, 0x8C, 0xC0,
            0x03, 0x3F, 0xFC, 0xC0,
            0x06, 0x64, 0x26, 0x60,
            0x0C, 0xCC, 0x33, 0x30,
            0x18, 0xCC, 0x33, 0x18,
            0x10, 0xC4, 0x23, 0x08,
            0x10, 0x63, 0xC6, 0x08,
            0x10, 0x30, 0x0C, 0x08,
            0x10, 0x18, 0x18, 0x08,
            0x10, 0x00, 0x00, 0x08    // 这是最上面的一行
    };

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3i(1, 0, 0);

    glEnable(GL_POLYGON_STIPPLE);

    glPolygonStipple(Mask);
    glRectf(-0.5f, -0.5f, 0.0f, 0.0f);

    glDisable(GL_POLYGON_STIPPLE);
    glRectf(0.0f, 0.0f, 0.5f, 0.5f);

    glFlush();
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