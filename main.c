#include "header.h"
#include "functions.h"
#include "constants.h"

bool openGlIsFinished = false;

void opengl();
void *antsAction(struct Ant *args);
struct Ant **ants;
float centerX = -0.03; // X-coordinate of the center of the circle
float centerY = 0.18;  // Y-coordinate of the center of the circle
float radius = 0.03;            // Radius of the circle
int numSegments = 100;

int main()
{

    readArguments("arguments.txt");
    printf("SIMULATION_TIME is %.2f\n", SIMULATION_TIME);
    printf("NUMBER_OF_ANTS is %d\n", NUMBER_OF_ANTS);
    srand(time(NULL));

    pthread_t opengl_thread;
    pthread_t ants_threads[NUMBER_OF_ANTS];
    ants = malloc(sizeof(struct Ant *) * NUMBER_OF_ANTS);

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant *ant;
        ant = malloc(sizeof(struct Ant));
        if (ant == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for ant %d.\n", i);
            exit(-1);
        }
        ants[i] = ant;

        (*ant).id = i + 1;
        ant->x = randomFloat(0, SCREEN_WIDTH);
        ant->y = randomFloat(0, SCREEN_HEIGHT);
        ant->speed = randomInt(MIN_SPEED, MAX_SPEED);
        ant->direction = randomDirection();

        printf("Ant ID: %d, Position: (%f, %f), Speed: %d, Direction: %d\n",
               ant->id, ant->x, ant->y, ant->speed, ant->direction);

        if (pthread_create(&ants_threads[i], NULL, (void *)antsAction, ant) != 0)
        {
            fprintf(stderr, "Failed to create thread for ant %d.\n", i);
            exit(-1);
        }
    }

    sleep(1);
    if (pthread_create(&opengl_thread, NULL, (void *)opengl, NULL))
    {
        fprintf(stderr, "Failed to create opengl thread");
        exit(-1);
    }

    // sleep(5);
    // openGlIsFinished = true;

    while (1)
        ;

    return 0;
}

void *antsAction(struct Ant *args)
{
    struct Ant *data = (struct Ant *)args;
    printf("Ant ID in thread: %d, Position: (%f, %f), Speed: %d, Direction: %d\n",
           data->id, data->x, data->y, data->speed, data->direction);

    // free(data);
}

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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Set the color to black
    glRasterPos2f(0, 0);
    glColor3f(0.0, 0.0, 0.0);
    // Draw a rectangle that covers the entire window
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, -1.0);
    glEnd();

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant *ant = ants[i];
        glRasterPos2f(ant->x, ant->y);
        drawFilledCircle();
        printf("Ant %d - Position: (%f, %f)\n", ant->id, ant->x, ant->y);
    }

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
}

void opengl()
{
    printf("Hi im in opengl\n");
    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant *ant = ants[i];
        printf("OpenGL Ant ID: %d\n", ant->id);
    }
    int c = 1;
    glutInit(&c, NULL);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Project 3: POSIX THREADS");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    while (!openGlIsFinished)
    {
        glutMainLoop();
    }
}