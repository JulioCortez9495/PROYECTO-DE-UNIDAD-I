



#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

int x0_line, y0_line, x1_line, y1_line;
bool firstClick = true;

enum Algorithm { NONE, DIRECT, DDA };
Algorithm currentAlgorithm = NONE;


void drawLineDirect(int x0, int y0, int x1, int y1)
{
    glBegin(GL_POINTS);
    if (x0 == x1)
    {
        int yStart = min(y0, y1);
        int yEnd = max(y0, y1);
        for (int y = yStart; y <= yEnd; y++)
            glVertex2i(x0, y);
    }
    else
    {
        float m = float(y1 - y0) / (x1 - x0);
        float b = y0 - m * x0;

        int xStart = min(x0, x1);
        int xEnd = max(x0, x1);

        for (int x = xStart; x <= xEnd; x++)
        {
            float y = m * x + b;
            glVertex2i(x, round(y));
        }
    }
    glEnd();
}

void drawLineDDA(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x0;
    float y = y0;

    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2i(round(x), round(y));
        x += xInc;
        y += yInc;
    }
    glEnd();
}

void drawAxes()
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex2i(-400, 0);
    glVertex2i(400, 0);
    glVertex2i(0, -300);
    glVertex2i(0, 300);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);

    drawAxes();

    if (currentAlgorithm == DIRECT)
        drawLineDirect(x0_line, y0_line, x1_line, y1_line);
    else if (currentAlgorithm == DDA)
        drawLineDDA(x0_line, y0_line, x1_line, y1_line);

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        int xGL = x - winWidth / 2;
        int yGL = (winHeight / 2) - y;

        if (firstClick)
        {
            x0_line = xGL;
            y0_line = yGL;
            firstClick = false;
        }
        else
        {
            x1_line = xGL;
            y1_line = yGL;
            firstClick = true;
            glutPostRedisplay();
        }
    }
}

void menu(int option)
{
    switch (option)
    {
    case 1:
        currentAlgorithm = DIRECT;
        break;
    case 2:
        currentAlgorithm = DDA;
        break;
    case 3:
        currentAlgorithm = NONE;
        glutPostRedisplay();
        break;
    case 9:
        exit(0);
    }
    glutPostRedisplay();
}

void createMenu()
{
    glutCreateMenu(menu);
    glutAddMenuEntry("Recta - Método Directo", 1);
    glutAddMenuEntry("Recta - Método DDA", 2);
    glutAddMenuEntry("Limpiar", 3);
    glutAddMenuEntry("Salir", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init()
{
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Lineas: Metodo Directo y DDA - OpenGL");

    init();
    createMenu();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}





