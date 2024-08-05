#include <stdlib.h>
#include <GL/glut.h>

float rotX = 0.0, rotY = 0.0, rotZ = 0.0;
float scale = 1.0;
float moveX = 0.0, moveY = 0.0, moveZ = 0.0;

void myinit() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
}

void face(float v0[], float v1[], float v2[], float v3[], float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex3fv(v0);
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
    glEnd();
}

void drawCube() {
    float v0[] = { -0.5, -0.5,  0.5 };
    float v1[] = { 0.5, -0.5,  0.5 };
    float v2[] = { 0.5,  0.5,  0.5 };
    float v3[] = { -0.5,  0.5,  0.5 };
    float v4[] = { -0.5, -0.5, -0.5 };
    float v5[] = { 0.5, -0.5, -0.5 };
    float v6[] = { 0.5,  0.5, -0.5 };
    float v7[] = { -0.5,  0.5, -0.5 };

    face(v0, v1, v2, v3, 1.0, 0.0, 0.0); // Front face
    face(v4, v7, v6, v5, 0.0, 1.0, 0.0); // Back face
    face(v4, v0, v3, v7, 0.0, 0.0, 1.0); // Left face
    face(v1, v5, v6, v2, 1.0, 1.0, 0.0); // Right face
    face(v3, v2, v6, v7, 0.0, 1.0, 1.0); // Top face
    face(v4, v5, v1, v0, 1.0, 0.0, 1.0); // Bottom face
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(moveX, moveY, moveZ);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotZ, 0.0, 0.0, 1.0);
    glScalef(scale, scale, scale);

    drawCube();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': moveY += 0.05; break;
    case 'a': moveX -= 0.05; break;
    case 's': moveY -= 0.05; break;
    case 'd': moveX += 0.05; break;
    case '.': rotY -= 5.0; break;
    case ',': rotY += 5.0; break;
    case '[': rotX -= 5.0; break;
    case ']': rotX += 5.0; break;
    case 'r': moveX = moveY = moveZ = rotX = rotY = rotZ = 0; scale = 1.0; break;
    case 'q': exit(0); break;
    }
    glutPostRedisplay();
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Transformations");

    myinit();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}
