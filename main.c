#include "header.h"
#include "functions.h"
#include "constants.h"

bool openGlIsFinished = false;

void opengl();
void *antsAction(struct Ant *args);
struct Ant **ants;
float radius = 0.02; // Radius of the circle
int numSegments = 200;

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
        ant->x = randomFloat(-SCREEN_WIDTH, SCREEN_WIDTH) / SCREEN_WIDTH;
        ant->y = randomFloat(-SCREEN_HEIGHT, SCREEN_HEIGHT) / SCREEN_HEIGHT;
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

void drawFilledCircle(float centerX, float centerY)
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
        glVertex2f(ant->x, ant->y);
        drawFilledCircle(ant->x, ant->y);
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

float getAngle(int direction)
{
    float angle;
    switch (direction)
    {
    case NORTH:
        angle = M_PI / 2;
        break;
    case SOUTH:
        angle = 3 * M_PI / 2;
        break;
    case EAST:
        angle = 0;
        break;
    case WEST:
        angle = M_PI;
        break;
    case NORTH_EAST:
        angle = M_PI / 4;
        break;
    case NORTH_WEST:
        angle = 3 * M_PI / 4;
        break;
    case SOUTH_EAST:
        angle = 7 * M_PI / 4;
        break;
    case SOUTH_WEST:
        angle = 5 * M_PI / 4;
        break;
    default:
        break;
    }
    return angle;
}

enum Direction getNewDirection(enum Direction direction)
{
    enum Direction newDirection;
    switch (direction)
    {
    case NORTH:
        newDirection = (randomInt(0, 1) == 0) ? SOUTH_WEST : SOUTH_EAST;
        break;
    case SOUTH:
        newDirection = (randomInt(0, 1) == 0) ? NORTH_WEST : NORTH_EAST;
        break;
    case EAST:
        newDirection = (randomInt(0, 1) == 0) ? NORTH_WEST : SOUTH_WEST;
        break;
    case WEST:
        newDirection = (randomInt(0, 1) == 0) ? NORTH_EAST : SOUTH_EAST;
        break;
    case NORTH_EAST:
        newDirection = (randomInt(0, 1) == 0) ? SOUTH_WEST : WEST;
        break;
    case NORTH_WEST:
        newDirection = (randomInt(0, 1) == 0) ? SOUTH_EAST : EAST;
        break;
    case SOUTH_EAST:
        newDirection = (randomInt(0, 1) == 0) ? NORTH_WEST : WEST;
        break;
    case SOUTH_WEST:
        newDirection = (randomInt(0, 1) == 0) ? NORTH_EAST : EAST;
        break;
    }

    return newDirection;
}

void update(int value)
{

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant *ant = ants[i];
        float angle = getAngle(ant->direction);

        double dx = 0.001 * ant->speed * cos(angle);
        double dy = 0.001 * ant->speed * sin(angle);

        // Update the ant's position
        ant->x += dx;
        ant->y += dy;
        if (ant->x < -0.9 || ant->x > 0.9 || ant->y < -0.9 || ant->y > 0.9)
        {
            ant->direction = getNewDirection(ant->direction);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
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