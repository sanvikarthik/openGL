#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>


float vertices[3][2] = {
    {0.0, 0.5},
    {-0.5, -0.5},
    {0.5, -0.5}
};


float tx = 0.0, ty = 0.0; 
float sx = 1.0, sy = 1.0; 
float angle = 0.0;        
void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 5, 0, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void display() {
    glClearColor(1, 0, 1,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 3; i++) {
        
        float x = vertices[i][0];
        float y = vertices[i][1];

       
        x *= sx;
        y *= sy;

       
        
        float x_rot = x * cos(angle) - y * sin(angle);
        float y_rot = x * sin(angle) + y * cos(angle);
        x = x_rot;
        y = y_rot;

        x += tx;
        y += ty;

        glVertex2f(x, y);
    }
    glEnd();

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w': ty += 0.1; break;
    case 's': ty -= 0.1; break;
    case 'a': tx -= 0.1; break; 
    case 'd': tx += 0.1; break; 
    case '+':
        sx += 0.1;
        sy += 0.1;
        if (sx > 2.0) sx = 2.0; 
        if (sy > 2.0) sy = 2.0;
        break; 
    case '-':
        sx -= 0.1;
        sy -= 0.1;
        if (sx < 0.1) sx = 0.1; 
        if (sy < 0.1) sy = 0.1;
        break; 
    case 'r': angle += 5.0; break; 
    case 'l': angle -= 5.0; break; 
    case 27: exit(0); 
    }
    glutPostRedisplay();
}

void reshape(int width, int height) {
    
    if (height == 0) {
        height = 1;
    }
    
    glViewport(0, 0, width, height);

   
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height) {
        gluOrtho2D(-1.0 * width / height, 1.0 * width / height, -1.0, 1.0);
    }
    else {
        gluOrtho2D(-1.0, 1.0, -1.0 * height / width, 1.0 * height / width);
    }

    glMatrixMode(GL_MODELVIEW);
}



void main() {
   
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Transformations - Translation, Scaling, Rotation");

    myinit();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);  
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    
}
