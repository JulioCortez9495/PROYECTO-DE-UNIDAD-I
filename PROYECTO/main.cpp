


#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

enum Figura { NINGUNO, LINEA_DIRECTA, LINEA_DDA, CIRCULO_PM, ELIPSE_PM };

struct Color { float r, g, b; };

struct Shape
{
    Figura tipo;
    Color color;
    int grosor;
    int p1, p2, p3, p4;

};


Figura herramientaActual = NINGUNO;
Color colorActual = {1.0f, 1.0f, 1.0f};
int grosorActual = 2;

vector<Shape> canvasShapes;

bool firstClick = true;
int tmp_x0 = 0, tmp_y0 = 0;

int centerX = 0, centerY = 0;
bool esperandoRadio = false;

int centerEx = 0, centerEy = 0;
bool esperandoRadioElipse = false;

void putPixel(int x, int y) { glVertex2i(x, y); }

void drawAxes()
{
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
        glVertex2i(-400, 0); glVertex2i(400, 0);
        glVertex2i(0, -300); glVertex2i(0, 300);
    glEnd();
}

void drawLineDirect(int x0, int y0, int x1, int y1)
{
    glBegin(GL_POINTS);
    if (x0 == x1)
    {
        int yStart = min(y0, y1), yEnd = max(y0, y1);
        for (int y = yStart; y <= yEnd; y++) putPixel(x0, y);
    } else
    {
        float m = float(y1 - y0) / (x1 - x0);
        float b = y0 - m * x0;
        int xStart = min(x0, x1), xEnd = max(x0, x1);
        for (int x = xStart; x <= xEnd; x++)
        {
            float y = m * x + b;
            putPixel(x, (int)round(y));
        }
    }
    glEnd();
}

void drawLineDDA(int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xInc = dx / (float)steps, yInc = dy / (float)steps;
    float x = x0, y = y0;
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        putPixel((int)round(x), (int)round(y));
        x += xInc; y += yInc;
    }
    glEnd();
}

void drawCircleMidpoint(int xc, int yc, int r)
{
    int x = 0, y = r, p = 1 - r;
    glBegin(GL_POINTS);
    while (x <= y)
    {
        putPixel(xc + x, yc + y); putPixel(xc - x, yc + y);
        putPixel(xc + x, yc - y); putPixel(xc - x, yc - y);
        putPixel(xc + y, yc + x); putPixel(xc - y, yc + x);
        putPixel(xc + y, yc - x); putPixel(xc - y, yc - x);
        if (p < 0) p += 2 * x + 3;
        else
        {
                p += 2 * (x - y) + 5; y--;
        }
        x++;
    }
    glEnd();
}


void drawEllipseMidpoint(int cx, int cy, int rx, int ry)
{
    int x = 0, y = ry;
    int rxSq = rx * rx, rySq = ry * ry;
    int twoRxSq = 2 * rxSq, twoRySq = 2 * rySq;
    int px = 0, py = twoRxSq * y;

    glBegin(GL_POINTS);

    double p1 = rySq - (rxSq * ry) + (0.25 * rxSq);
    while (px < py)
        {
        putPixel(cx + x, cy + y); putPixel(cx - x, cy + y);
        putPixel(cx + x, cy - y); putPixel(cx - x, cy - y);
        x++; px += twoRySq;
        if (p1 < 0) p1 += rySq + px;
        else
        {
            y--; py -= twoRxSq; p1 += rySq + px - py;
        }
    }

    double p2 = rySq * (x + 0.5) * (x + 0.5) + rxSq * (y - 1) * (y - 1) - rxSq * rySq;
    while (y >= 0)
    {
        putPixel(cx + x, cy + y); putPixel(cx - x, cy + y);
        putPixel(cx + x, cy - y); putPixel(cx - x, cy - y);
        y--; py -= twoRxSq;
        if (p2 > 0) p2 += rxSq - py;
        else
        {
            x++; px += twoRySq; p2 += rxSq - py + px;
        }
    }
    glEnd();
}

void renderShape(const Shape &s)
{
    glColor3f(s.color.r, s.color.g, s.color.b);
    glPointSize((GLfloat)s.grosor);
    switch (s.tipo)
    {
        case LINEA_DIRECTA: drawLineDirect(s.p1, s.p2, s.p3, s.p4); break;
        case LINEA_DDA:     drawLineDDA(s.p1, s.p2, s.p3, s.p4); break;
        case CIRCULO_PM:    drawCircleMidpoint(s.p1, s.p2, s.p3); break;
        case ELIPSE_PM:     drawEllipseMidpoint(s.p1, s.p2, s.p3, s.p4); break;
        default: break;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    for (const Shape &s : canvasShapes) renderShape(s);

    glColor3f(colorActual.r, colorActual.g, colorActual.b);
    glPointSize((GLfloat)grosorActual);
    if (!firstClick && (herramientaActual == LINEA_DIRECTA || herramientaActual == LINEA_DDA))
    {

    }
    if (esperandoRadio && herramientaActual == CIRCULO_PM)
    {

    }
    if (esperandoRadioElipse && herramientaActual == ELIPSE_PM)
    {

    }

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

        if (herramientaActual == LINEA_DIRECTA || herramientaActual == LINEA_DDA)
            {
            if (firstClick)
            {
                tmp_x0 = xGL; tmp_y0 = yGL; firstClick = false;
            } else
            {

                Shape s;
                s.tipo = (herramientaActual == LINEA_DIRECTA) ? LINEA_DIRECTA : LINEA_DDA;
                s.color = colorActual; s.grosor = grosorActual;
                s.p1 = tmp_x0; s.p2 = tmp_y0; s.p3 = xGL; s.p4 = yGL;
                canvasShapes.push_back(s);
                firstClick = true;
                glutPostRedisplay();
            }
        }
        else if (herramientaActual == CIRCULO_PM)
        {
            if (!esperandoRadio)
            {
                centerX = xGL; centerY = yGL; esperandoRadio = true;
            } else
            {
                int dx = xGL - centerX, dy = yGL - centerY;
                int r = (int)round(sqrt(dx*dx + dy*dy));
                Shape s; s.tipo = CIRCULO_PM; s.color = colorActual; s.grosor = grosorActual;
                s.p1 = centerX; s.p2 = centerY; s.p3 = r; s.p4 = 0;
                canvasShapes.push_back(s);
                esperandoRadio = false;
                glutPostRedisplay();
            }
        }
        else if (herramientaActual == ELIPSE_PM)
        {
            if (!esperandoRadioElipse)
            {
                centerEx = xGL; centerEy = yGL; esperandoRadioElipse = true;
            } else
            {
                int rx = abs(xGL - centerEx);
                int ry = abs(yGL - centerEy);
                Shape s; s.tipo = ELIPSE_PM; s.color = colorActual; s.grosor = grosorActual;
                s.p1 = centerEx; s.p2 = centerEy; s.p3 = rx; s.p4 = ry;
                canvasShapes.push_back(s);
                esperandoRadioElipse = false;
                glutPostRedisplay();
            }
        }
    }
}

enum MenuOpciones
{
    M_RECT_LINEA_DIRECTA = 1, M_RECT_LINEA_DDA, M_CIRCULO_MID, M_ELIPSE_MID,
    M_COLOR_ROJO, M_COLOR_VERDE, M_COLOR_AZUL, M_COLOR_BLANCO,
    M_GROSOR_1, M_GROSOR_2, M_GROSOR_3, M_GROSOR_5, M_LIMPIAR
};

void menuHandler(int opcion)
{
    switch (opcion)
    {
        case M_RECT_LINEA_DIRECTA: herramientaActual = LINEA_DIRECTA; firstClick = true; esperandoRadio = false; esperandoRadioElipse = false; break;
        case M_RECT_LINEA_DDA:     herramientaActual = LINEA_DDA; firstClick = true; esperandoRadio = false; esperandoRadioElipse = false; break;
        case M_CIRCULO_MID:        herramientaActual = CIRCULO_PM; esperandoRadio = false; firstClick = true; esperandoRadioElipse = false; break;
        case M_ELIPSE_MID:         herramientaActual = ELIPSE_PM; esperandoRadioElipse = false; firstClick = true; esperandoRadio = false; break;
        case M_COLOR_ROJO:         colorActual = {1,0,0}; break;
        case M_COLOR_VERDE:        colorActual = {0,1,0}; break;
        case M_COLOR_AZUL:         colorActual = {0,0,1}; break;
        case M_COLOR_BLANCO:       colorActual = {1,1,1}; break;
        case M_GROSOR_1:           grosorActual = 1; break;
        case M_GROSOR_2:           grosorActual = 2; break;
        case M_GROSOR_3:           grosorActual = 3; break;
        case M_GROSOR_5:           grosorActual = 5; break;
        case M_LIMPIAR:
            canvasShapes.clear();
            herramientaActual = NINGUNO;
            firstClick = true; esperandoRadio = false; esperandoRadioElipse = false;
            break;
    }
    glutPostRedisplay();
}

void initMenu()
{
    int submenuDibujo = glutCreateMenu(menuHandler);
    glutAddMenuEntry("RECTA (Directo)", M_RECT_LINEA_DIRECTA);
    glutAddMenuEntry("RECTA (DDA)", M_RECT_LINEA_DDA);
    glutAddMenuEntry("CÍRCULO (Punto Medio)", M_CIRCULO_MID);
    glutAddMenuEntry("ELIPSE (Punto Medio)", M_ELIPSE_MID);

    int submenuColor = glutCreateMenu(menuHandler);
    glutAddMenuEntry("ROJO", M_COLOR_ROJO);
    glutAddMenuEntry("VERDE", M_COLOR_VERDE);
    glutAddMenuEntry("AZUL", M_COLOR_AZUL);
    glutAddMenuEntry("BLANCO", M_COLOR_BLANCO);

    int submenuGrosor = glutCreateMenu(menuHandler);
    glutAddMenuEntry("1 px", M_GROSOR_1);
    glutAddMenuEntry("2 px", M_GROSOR_2);
    glutAddMenuEntry("3 px", M_GROSOR_3);
    glutAddMenuEntry("5 px", M_GROSOR_5);

    int menuPrincipal = glutCreateMenu(menuHandler);
    glutAddSubMenu("DIBUJO", submenuDibujo);
    glutAddSubMenu("COLOR", submenuColor);
    glutAddSubMenu("GROSOR", submenuGrosor);
    glutAddMenuEntry("LIMPIAR", M_LIMPIAR);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init() {
    glClearColor(0,0,0,1);
    glColor3f(1,1,1);
    glPointSize((GLfloat)grosorActual);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow(" ' PROYECTO DE UNIDAD ' ");

    init();
    initMenu();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
