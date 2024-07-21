#include<stdlib.h>
#include <GL/glut.h>

float angleX = 0.0, angleY = 0.0, angleZ = 0.0;
float scale = 1.0;
float translateX = 0.0, translateY = 0.0, translateZ = 0.0;

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void drawCube() {
    glBegin(GL_QUADS);

    // Front face
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);

    // Back face
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);

    // Left face
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(-0.5, -0.5, 0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(-0.5, 0.5, -0.5);

    // Right face
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, 0.5, -0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, -0.5, 0.5);

    // Top face
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-0.5, 0.5, -0.5);
    glVertex3f(-0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5);
    glVertex3f(0.5, 0.5, -0.5);

    // Bottom face
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, -0.5);
    glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(-0.5, -0.5, 0.5);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // GL_DEPTH_BUFFER_BIT: Clears the depth buffer. This resets the depth values 
    // for each pixel to a large value (usually 1.0) 
    // because depth values range from 0.0 (closest) to 1.0 (farthest).
    glLoadIdentity();
    glTranslatef(translateX, translateY, translateZ);
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale); // Apply uniform scaling

    drawCube();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x': angleX += 5.0; break;
    case 'X': angleX -= 5.0; break;
    case 'y': angleY += 5.0; break;
    case 'Y': angleY -= 5.0; break;
    case 'z': angleZ += 5.0; break;
    case 'Z': angleZ -= 5.0; break;
    case 's': scale += 0.1; break;
    case 'S': scale -= 0.1; break;
    case 't': translateX += 0.1; break;
    case 'T': translateX -= 0.1; break;
    case 'u': translateY += 0.1; break;
    case 'U': translateY -= 0.1; break;
    case 'v': translateZ += 0.1; break;
    case 'V': translateZ -= 0.1; break;
    case 27: exit(0); break; // ESC to exit
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Transformations");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
