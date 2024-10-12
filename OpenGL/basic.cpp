/**
* <pre>
* Author: taylorcyang@tencent.com
* Date:   2019-07-10
* Time:   18:00
* Life with Passion, Code with Creativity.
* </pre>
*/

#include "index.h"

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