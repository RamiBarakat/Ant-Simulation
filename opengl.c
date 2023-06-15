#include <GL/glut.h>
#include <math.h>

#define PI 3.14159265

// Function to draw a plate with food items
void drawPlate(float x, float y, float radius, int quantity)
{
    // Set the plate color
    glColor3f(0.9, 0.9, 0.9); // Light Gray

    // Draw the plate
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * PI / 180.0;
        float px = x + radius * cos(angle);
        float py = y + radius * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    // Set the food color
    glColor3f(0.7, 0.9, 0.1); // Light Green

    // Calculate the angle between each food item
    float angleIncrement = 360.0 / quantity;

    // Draw the food items
    glColor3f(0.7, 0.9, 0.1); // Light Green
    for (int i = 0; i < quantity; i++)
    {
        float angle = i * angleIncrement * PI / 180.0;
        float fx = x + (radius * 0.8) * cos(angle);
        float fy = y + (radius * 0.8) * sin(angle);

        // Draw a small circle as a food item
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++)
        {
            float angle2 = j * PI / 180.0;
            float px = fx + 0.02 * cos(angle2);
            float py = fy + 0.02 * sin(angle2);
            glVertex2f(px, py);
        }
        glEnd();
    }
}

// OpenGL display function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Example usage: Draw a plate with 8 food items at position (0.5, 0.5) and radius 0.2
    int quantity = 1;
    
    drawPlate(0.5, 0.5, 0.2, quantity);

    // Additional code to draw other objects

    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Creative Food Drawing");
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set the background color (white)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
