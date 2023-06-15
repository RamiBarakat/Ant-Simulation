#include "header.h"
#include "functions.h"
#include "constants.h"

void drawCircle(float x, float y, float radius)
{
    int numSegments = 100;
    float angle;

    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++)
    {
        angle = 2.0 * M_PI * i / numSegments;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}

void drawRectangle(float x, float y, float width, float height)
{
    glBegin(GL_POLYGON);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawAnt(float x, float y, float rotateAngel, float scaleFactor)
{
    glScalef(scaleFactor, scaleFactor, 1.0); // Scale the ant
    glTranslatef(x, y, 0.0); // Translate the ant
    glRotatef(rotateAngel, 0.0, 0.0, 1.0);


    glColor3f(0.0, 0.0, 0.0); 
    drawCircle(0.0, 0.0, 0.2);

    glColor3f(0.0, 0.0, 0.0); 
    drawCircle(0.3, 0.0, 0.1);

    // Draw legs (rectangles)
    glRotatef(45, 0.0, 0.0, 1.0); 
    glColor3f(0.0, 0.0, 0.0); 
    drawRectangle(-0.0, 0.15, 0.05, 0.25);
    drawRectangle(0.15, 0.05, 0.05, 0.25);
    glRotatef(-90, 0.0, 0.0, 1.0); 
    drawRectangle(-0.0, -0.4, 0.05, 0.25);
    drawRectangle(0.15, -0.3, 0.05, 0.25);


}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawAnt(-0.6, -0.4, -90, 0.1);

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("test");

    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}