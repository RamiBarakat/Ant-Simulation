#include "header.h"
#include "constants.h"

// Window dimensions
const int windowWidth = 1000;
const int windowHeight = 400;
const float alignY = 0;
const float alignX = 0.3;
const float alignLeft = -0.90;
const float alignTop = -0.3;

float centerX = -0.03 + alignX; // X-coordinate of the center of the circle
float centerY = 0.18 + alignY;  // Y-coordinate of the center of the circle
float radius = 0.03;            // Radius of the circle
int numSegments = 100;
float movementRange = -0.65; // Range of movement for the circle
float movementSpeed = 0.01;  // Speed of the movement
bool moveDown = true;

float spyX = -0.15 + alignX; // X-coordinate of the spy of the circle
float spyY = -0.19 + alignY;
float spyRange = 0.1; // Range of movement for the circle
bool spyMove = true;
float spySpeed = 0.003; // Speed of the movement

float falseX = -0.6 + alignX;
float falseY = 0.15 + alignY;

float trueX = -0.65 + alignX;
float trueY = 0.05 + alignY;

key_t openGlKey;
int msgOGL;
struct message msg0;
struct message msg1;
struct message msg2;
struct message msg3;

int spies;
int helpers;
int columns = -1;
bool msg1Read = false;
bool msg2Read = false;
int cnt = 0;
bool gameIsFinished = false;

int received = -1; // 1 win and 0 loss


void drawFilledCircle()
{
    // Set the color to blue
    glColor3f(0.1, 0.7, 1.0);
    // Draw the filled circle
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSegments; i++)
    {
        float theta = 2.0 * M_PI * i / numSegments;
        float x = centerX + radius * cos(theta);
        float y = centerY + radius * sin(theta);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawTrue()
{
    glColor3f(0.0, 1.0, 0.0); // RGB values for green
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(trueX + 0, trueY + 0);
    glVertex2f(trueX + 0.05, trueY + 0.1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(trueX + 0, trueY - 0.003);
    glVertex2f(trueX + -0.01, trueY + 0.05);
    glEnd();
}

void drawFalse()
{
    glColor3f(1.0, 0.0, 0.0); // RGB values for green
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(falseX + 0, falseY + 0);
    glVertex2f(falseX + -0.03, falseY - 0.1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(falseX + -0.03, falseY + 0);
    glVertex2f(falseX + 0, falseY - 0.10);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Set the color to black
    glColor3f(0.0, 0.0, 0.0);

    // Draw a rectangle that covers the entire window
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, -1.0);
    glEnd();

    // Set the color to white for the text
    glColor3f(0.1, 0.7, 1.0);

    // Set the position for the text
    glRasterPos2f(alignLeft, 0.8);

    // The text to be displayed
    char text[] = "Message encoding/decoding";

    // Loop through each character of the text and display it
    int length = sizeof(text);
    for (int i = 0; i < length; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }

    glColor3f(1.0, 1.0, 1.0);

    glRasterPos2f(alignLeft + 0.06, 0.6 + alignTop);
    // The text to be displayed

    char test[] = "Number of Helpers: ";

    // Loop through each character of the text and display it
    for (int i = 0; i < sizeof(test); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, test[i]);
    }

    char helperstext[10];
    sprintf(helperstext, "%d", helpers);

    // Loop through each character of the `cntText` and display it
    for (int i = 0; i < strlen(helperstext); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, helperstext[i]);
    }

    // 00000000000000000
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(alignLeft + 0.08, 0.4 + alignTop);
    // The text to be displayed

    char test2[] = "Number of Spies: ";

    // Loop through each character of the text and display it
    for (int i = 0; i < sizeof(test2); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, test2[i]);
    }

    char spiesText[10];
    sprintf(spiesText, "%d", spies);
        glColor3f(1.0, 0, 0);
    // Loop through each character of the `cntText` and display it
    for (int i = 0; i < strlen(spiesText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, spiesText[i]);
    }

    // 00000000000000000

    // 00000000000000000
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(alignLeft + 0.06, 0.2 + alignTop);
    // The text to be displayed

    char test3[] = "Number of Messages: ";

    // Loop through each character of the text and display it
    for (int i = 0; i < sizeof(test3); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, test3[i]);
    }

    char columnsText[10];
    sprintf(columnsText, "%d", columns);

    // Loop through each character of the `cntText` and display it
    for (int i = 0; i < strlen(columnsText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, columnsText[i]);
    }

    // 00000000000000000
    glColor3f(0.1, 0.7, 1.0);

    // Set the position for the text
    glRasterPos2f(0.1 + alignX, 0.3 + alignY);

    // Convert the `columns` variable to a string
    char cntText[10];
    sprintf(cntText, "%d", cnt);

    // Loop through each character of the `cntText` and display it
    for (int i = 0; i < strlen(cntText); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, cntText[i]);
    }

    if (gameIsFinished)
    {
        if (received == 1)
        {
            drawTrue();
        }
        else if (received == 0)
        {
            drawFalse();
        }
    }
    ////////////////

    // Set the color to white for the text
    glColor3f(1.0, 1.0, 1.0);

    // Set the position for the text
    glRasterPos2f(-0.1 + alignX, 0.8 + alignY);

    // The text to be displayed
    char text1[] = "Parent";

    // Loop through each character of the text1 and display it
    for (int i = 0; i < sizeof(text1); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text1[i]);
    }

    // Line
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.03 + alignX, 0.47 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.03 + alignX, 0.7 + alignY);  // Ending point (x, y) at the top
    glEnd();

    // Sender Text start

    glRasterPos2f(-0.1 + alignX, 0.3 + alignY);

    // The text to be displayed
    char text2[] = "Sender";

    // Loop through each character of the text2 and display it
    for (int i = 0; i < sizeof(text2); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
    }

    glRasterPos2f(-0.11 + alignX, -0.8 + alignY);

    // The text to be displayed
    char text3[] = "Receiver";

    // Loop through each character of the text3 and display it
    for (int i = 0; i < sizeof(text3); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text3[i]);
    }

    glRasterPos2f(-0.4 + alignX, -0.2 + alignY);

    // The text to be displayed
    char text4[] = "Spy Master";

    // Loop through each character of the text4 and display it
    for (int i = 0; i < sizeof(text4); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text4[i]);
    }

    glColor3f(1.0, 0.0, 0.0);

    glRasterPos2f(spyX + alignX, spyY + alignY);
    // The text to be displayed
    char text5[] = "SPY";

    // Loop through each character of the text5 and display it
    for (int i = 0; i < sizeof(text5); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text5[i]);
    }

    // Set the color to green
    drawFilledCircle();

    // Lines
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.15 + alignX, -0.76 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.55 + alignX, -0.76 + alignY); // Ending point (x, y) at the top
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.55 + alignX, -0.76 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.55 + alignX, 0.85 + alignY);  // Ending point (x, y) at the top
    glEnd();

    glLineWidth(5);
    glBegin(GL_LINES);
    glVertex2f(-0.55 + alignX, 0.85 + alignY); // Starting point (x, y) at the bottom
    glVertex2f(-0.15 + alignX, 0.85 + alignY); // Ending point (x, y) at the top
    glEnd();

    glFlush();
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void update(int value)
{
    // read number of columns
    if (!msg1Read)
    {
        if (msgrcv(msgOGL, &msg1, sizeof(msg1.text), 1, 0) == -1)
        {
            perror("msgrcv");
            exit(-1);
        }
        else
        {
            columns = atoi(msg1.text);
            msg1Read = true;
        }
    }
    // read who won
    if (!msg2Read && msg1Read)
    {
        if (msgrcv(msgOGL, &msg2, sizeof(msg2.text), 2, 0) == -1)
        {
            perror("msgrcv");
            exit(-1);
        }
        else
        {
            received = atoi(msg2.text);
            msg2Read = true;
        }
    }

    // Update the vertical position of the circle
    if (moveDown && cnt < columns)
    {
        centerY -= movementSpeed;
        if (centerY <= movementRange)
        {
            moveDown = false;
            cnt++;
        }
    }
    else
    {
        centerY = 0.18 + alignY;
        moveDown = true;
        if (cnt == columns)
        {
            gameIsFinished = true;
        }
    }

    if (spyMove && cnt < columns)
    {
        spyX += spySpeed;
        if (spyX >= spyRange)
            spyMove = false;
    }
    else
    {
        spyX = -0.15 + alignY;
        spyMove = true;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // Update at 60 frames per second
}

void openMsgQueue()
{
    openGlKey = ftok(".", '9'); // For OpenGL
    msgOGL = msgget(openGlKey, IPC_CREAT | 0666);
    if (msgOGL == -1)
    {
        perror("msgget");
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    helpers = atoi(argv[1]);
    spies = atoi(argv[2]);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Project 2: IPC");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    openMsgQueue();
    glutTimerFunc(0, update, 0);
    glutMainLoop();

    return 0;
}
