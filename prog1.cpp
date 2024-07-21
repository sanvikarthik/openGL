#include <GL/glut.h>
#include <cmath>

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void lineBres(int x0, int y0, int xEnd, int yEnd) {
    int dx = fabs(xEnd - x0), dy = fabs(yEnd - y0);
    int p = 2 * dy - dx;
    int twoDy = 2 * dy, twoDyMinusDx = 2 * (dy - dx);
    int x, y;

    if (x0 > xEnd) {
        x = xEnd;
        y = yEnd;
        xEnd = x0;
    }
    else {
        x = x0;
        y = y0;
    }

    setPixel(x, y);
    while (x < xEnd) {
        x++;
        if (p < 0)
            p += twoDy;
        else {
            y++;
            p += twoDyMinusDx;
        }
        setPixel(x, y);
    }
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Black background
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // White color

    // Define line coordinates
    int x0 = 50, y0 = 100;
    int xEnd = 200, yEnd = 300;

    lineBres(x0, y0, xEnd, yEnd);
    glFlush();
}

void init() {

    //glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0); // Set the window coordinates
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500); // Set the window size
    glutInitWindowPosition(100, 100); // Set the window position
    glutCreateWindow("Bresenham Line Drawing"); // Create the window
    init();
    glutDisplayFunc(display); // Register the display function
    glutMainLoop();
    return 0;
}
