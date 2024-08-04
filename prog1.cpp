#include <stdio.h>
#include<math.h>
#include <GL/glut.h>

void myinit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
	glMatrixMode(GL_MODELVIEW);

}
void setPixel(int x, int y)
{
	glColor3f(1, 1, 0);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}
void lineBres(int x0, int y0, int xend, int yend)
{
	int dx = fabs(xend - x0), dy = fabs(yend - y0);
	int p = 2 * dy - dx;
	int twody = 2 * dy, twodyminusdx = 2 * (dy - dx);
	int	x,y;
	if (x0 > xend)
	{
		x = xend;
		y = yend;
		xend = x0;
	}
	else
	{
		x = x0;
		y = y0;
	}
	setPixel(x, y);
	while (x < xend)
	{
		x++;
		if (p < 0) {
			p += twody;
		}
		else {
			y++;
			p += twodyminusdx;
		}
		setPixel(x, y);
	}
}
void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	lineBres(50, 100, 200, 300);
}
void main()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 10);
	glutCreateWindow("first");
	myinit();
	glutDisplayFunc(display);
	glutMainLoop();
}
