




#include <GL/glut.h>
#include <iostream>

enum MenuOpciones {
    RECT_LINEA_DIRECTA = 1,
    RECT_LINEA_DDA,
    CIRCULO_PM,
    ELIPSE_PM,
    COLOR_ROJO,
    COLOR_VERDE,
    COLOR_AZUL,
    GROSOR_1,
    GROSOR_3,
    LIMPIAR
};

void menuHandler(int opcion) {
    switch (opcion) {
        case RECT_LINEA_DIRECTA:
            std::cout << "Dibujar recta (Directo)\n";
            break;
        case RECT_LINEA_DDA:
            std::cout << "Dibujar recta (DDA)\n";
            break;
        case CIRCULO_PM:
            std::cout << "Dibujar círculo (Punto Medio)\n";
            break;
        case ELIPSE_PM:
            std::cout << "Dibujar elipse (Punto Medio)\n";
            break;
        case COLOR_ROJO:
            std::cout << "Color cambiado a rojo\n";
            break;
        case COLOR_VERDE:
            std::cout << "Color cambiado a verde\n";
            break;
        case COLOR_AZUL:
            std::cout << "Color cambiado a azul\n";
            break;
        case GROSOR_1:
            std::cout << "Grosor 1 px\n";
            break;
        case GROSOR_3:
            std::cout << "Grosor 3 px\n";
            break;
        case LIMPIAR:
            std::cout << "Lienzo limpiado\n";
            glClear(GL_COLOR_BUFFER_BIT);
            glutPostRedisplay();
            break;
    }
}

void initMenu() {

    int submenuDibujo = glutCreateMenu(menuHandler);
    glutAddMenuEntry("Recta (Directo)", RECT_LINEA_DIRECTA);
    glutAddMenuEntry("Recta (DDA)", RECT_LINEA_DDA);
    glutAddMenuEntry("Círculo (Punto Medio)", CIRCULO_PM);
    glutAddMenuEntry("Elipse (Punto Medio)", ELIPSE_PM);

    int submenuColor = glutCreateMenu(menuHandler);
    glutAddMenuEntry("Rojo", COLOR_ROJO);
    glutAddMenuEntry("Verde", COLOR_VERDE);
    glutAddMenuEntry("Azul", COLOR_AZUL);

    int submenuGrosor = glutCreateMenu(menuHandler);
    glutAddMenuEntry("1 px", GROSOR_1);
    glutAddMenuEntry("3 px", GROSOR_3);

    int menuPrincipal = glutCreateMenu(menuHandler);
    glutAddSubMenu("Dibujo", submenuDibujo);
    glutAddSubMenu("Color", submenuColor);
    glutAddSubMenu("Grosor", submenuGrosor);
    glutAddMenuEntry("Limpiar lienzo", LIMPIAR);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("CAD 2D - Menú FreeGLUT");

    initMenu();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

