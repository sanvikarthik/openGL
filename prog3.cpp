#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

float rot = 0, rot2 = 0, move_x = 0, move_y = 0, move_z = 0;
float v[8][3] = {
    {-1, -1, -1},
    {-1, -1, +1},
    {+1, -1, +1},
    {+1, -1, -1},
    {-1, +1, -1},
    {-1, +1, +1},
    {+1, +1, +1},
    {+1, +1, -1}
};

void face(int a, int b, int c, int d)
{
    
    glBegin(GL_QUADS);
    glVertex3fv(v[a]);
    glVertex3fv(v[b]);
    glVertex3fv(v[c]);
    glVertex3fv(v[d]);
    glEnd();
    glFlush();
}

void myinit()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4, 4, -4, 4, -4, 4);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void cube(float tx, float ty, float tz, float rx, float ry, float rz, float scale, float c)
{
    float x, y, z, t;
    rx += rot2;
    ry += rot;
    
    for (int i = 0; i < 8; i++)
    {
        x = v[i][0];
        y = v[i][1];
        z = v[i][2];
        x = x * scale + tx;
        y = y * scale + ty;
        z = z * scale + tz;

        t = y * cos(rx) - z * sin(rx);
        z = y * sin(rx) + z * cos(rx);
        y = t;

        t = z * cos(ry) - x * sin(ry);
        x = z * sin(ry) + x * cos(ry);
        z = t;

        t = x * cos(rz) - y * sin(rz);
        y = x * sin(rz) + y * cos(rz);
        x = t;

        v[i][0] = x + move_x;
        v[i][1] = y + move_y;
        v[i][2] = z + move_z;
    }
    glColor3f(c, 0, 0);
    face(0, 1, 2, 3);
    glColor3f(c, c, 0);
    face(0, 1, 5, 4);
    glColor3f(0, c, 0);
    face(1, 2, 6, 5);
    glColor3f(0, c, c);
    face(2, 3, 7, 6);
    glColor3f(0, 0, c);
    face(3, 0, 4, 7);
    glColor3f(c, 0, c);
    face(4, 5, 6, 7);
}

void display()
{
    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cube(0, 0, 0, 0, 0, 0, 1, 1);
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': move_y += 0.05; break;
    case 'a': move_x -= 0.05; break;
    case 's': move_y -= 0.05; break;
    case 'd': move_x += 0.05; break;
    case '.': rot--; break;
    case ',': rot++; break;
    case '[': rot2--; break;
    case ']': rot2++; break;
    case 'r': move_x = move_z = rot = rot2 = 0; break;
    case 'q': exit(0);
    }
    glutPostRedisplay();
}

void main()
{
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(300, 300);
    glutCreateWindow("3D Operation");
    myinit();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}
