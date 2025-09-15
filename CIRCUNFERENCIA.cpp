


#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

int centerX, centerY, radius;
bool waitingForRadius = false;

enum Mode { NONE, CIRCLE };
Mode currentMode = NONE;


void drawCircleMidpoint(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int p = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y)
    {

        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);

        if (p < 0)
            p += 2 * x + 3;
        else
        {
            p += 2 * (x - y) + 5;
            y--;
        }
        x++;
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
    drawAxes();

    if (currentMode == CIRCLE && !waitingForRadius)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        drawCircleMidpoint(centerX, centerY, radius);
    }

    glFlush();
}


void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && currentMode == CIRCLE)
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
            cout << "Centro: (" << centerX << ", " << centerY << ")" << endl;
        }
        else
        {
            int dx = xGL - centerX;
            int dy = yGL - centerY;
            radius = (int)sqrt(dx * dx + dy * dy);
            waitingForRadius = false;

            cout << "Radio: " << radius << endl;
            glutPostRedisplay();
        }
    }
}


void menu(int option)
{
    switch (option)
    {
        case 1:
            currentMode = CIRCLE;
            waitingForRadius = false;
            break;
        case 2: 
            currentMode = NONE;
            glutPostRedisplay();
            break;
        case 9:
            exit(0);
    }
}

void createMenu()
{
    glutCreateMenu(menu);
    glutAddMenuEntry("Dibujar Círculo (Punto Medio)", 1);
    glutAddMenuEntry("Limpiar", 2);
    glutAddMenuEntry("Salir", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);   
    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0); 
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
    glutCreateWindow("Círculo - Método Punto Medio (Mouse + Menú)");

    init();
    createMenu();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}

