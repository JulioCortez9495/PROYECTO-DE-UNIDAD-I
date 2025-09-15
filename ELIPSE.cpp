



#include <GL/glut.h>
#include <iostream>
#include <cmath>

using namespace std;

int centerX, centerY;
int radiusX = 0, radiusY = 0;
float rotation = 0.0f;

bool waitingForRadius = false;

enum Mode { NONE, ELLIPSE };
Mode currentMode = NONE;

void putPixel(int x, int y)
{
    glVertex2i(x, y);
}

void rotatePoint(int& x, int& y, float angle)
{
    float rad = angle * M_PI / 180.0f;
    float cosA = cos(rad);
    float sinA = sin(rad);
    float xr = x * cosA - y * sinA;
    float yr = x * sinA + y * cosA;
    x = static_cast<int>(round(xr));
    y = static_cast<int>(round(yr));
}

void drawEllipseMidpoint(int cx, int cy, int rx, int ry, float angle = 0.0f)
{
    int x = 0;
    int y = ry;

    int rxSq = rx * rx;
    int rySq = ry * ry;

    int twoRxSq = 2 * rxSq;
    int twoRySq = 2 * rySq;

    int px = 0;
    int py = twoRxSq * y;

    glBegin(GL_POINTS);

    // Región 1
    int p1 = rySq - (rxSq * ry) + (0.25 * rxSq);
    while (px < py)
    {
        int drawX = x;
        int drawY = y;
        rotatePoint(drawX, drawY, angle);

        putPixel(cx + drawX, cy + drawY);
        putPixel(cx - drawX, cy + drawY);
        putPixel(cx + drawX, cy - drawY);
        putPixel(cx - drawX, cy - drawY);

        x++;
        px += twoRySq;

        if (p1 < 0)
            p1 += rySq + px;
        else
        {
            y--;
            py -= twoRxSq;
            p1 += rySq + px - py;
        }
    }

    // Región 2
    int p2 = rySq * (x + 0.5) * (x + 0.5) + rxSq * (y - 1) * (y - 1) - rxSq * rySq;
    while (y >= 0)
    {
        int drawX = x;
        int drawY = y;
        rotatePoint(drawX, drawY, angle);

        putPixel(cx + drawX, cy + drawY);
        putPixel(cx - drawX, cy + drawY);
        putPixel(cx + drawX, cy - drawY);
        putPixel(cx - drawX, cy - drawY);

        y--;
        py -= twoRxSq;

        if (p2 > 0)
            p2 += rxSq - py;
        else
        {
            x++;
            px += twoRySq;
            p2 += rxSq - py + px;
        }
    }

    glEnd();
}

void drawAxes()
{
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINES);
    glVertex2i(-400, 0); glVertex2i(400, 0);
    glVertex2i(0, -300); glVertex2i(0, 300);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    glColor3f(1, 1, 1);

    if (currentMode == ELLIPSE && !waitingForRadius && radiusX > 0 && radiusY > 0)
    {
        drawEllipseMidpoint(centerX, centerY, radiusX, radiusY, rotation);
    }

    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: exit(0); break;
    case '+':
        radiusX += 5;
        radiusY += 5;
        glutPostRedisplay();
        break;
    case '-':
        if (radiusX > 5) radiusX -= 5;
        if (radiusY > 5) radiusY -= 5;
        glutPostRedisplay();
        break;
    case 'r':
    case 'R':
        radiusX = 0;
        radiusY = 0;
        rotation = 0;
        glutPostRedisplay();
        break;
    case 'q':
    case 'Q':
        rotation += 10;
        if (rotation >= 360) rotation -= 360;
        glutPostRedisplay();
        break;
    case 'e':
    case 'E':
        rotation -= 10;
        if (rotation < 0) rotation += 360;
        glutPostRedisplay();
        break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && currentMode == ELLIPSE)
    {
        int winWidth = glutGet(GLUT_WINDOW_WIDTH);
        int winHeight = glutGet(GLUT_WINDOW_HEIGHT);

        int xGL = x - winWidth / 2;
        int yGL = (winHeight / 2) - y;

        if (!waitingForRadius)
        {
            centerX = xGL;
            centerY = yGL;
            waitingForRadius = true;
        }
        else
        {
            radiusX = abs(xGL - centerX);
            radiusY = abs(yGL - centerY);
            waitingForRadius = false;
            glutPostRedisplay();
        }
    }
}

void menu(int option)
{
    switch (option)
    {
    case 1: currentMode = ELLIPSE; waitingForRadius = false; break;
    case 2: currentMode = NONE; radiusX = 0; radiusY = 0; glutPostRedisplay(); break;
    case 9: exit(0); break;
    }
}

void createMenu()
{
    glutCreateMenu(menu);
    glutAddMenuEntry("Dibujar Elipse (Punto Medio)", 1);
    glutAddMenuEntry("Limpiar", 2);
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
    glutCreateWindow(" ' LA ELIPSE ' ");

    init();
    createMenu();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

