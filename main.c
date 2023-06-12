#include "header.h"
#include "functions.h"
#include "constants.h"

bool openGlIsFinished = false;

void opengl();
void *antsAction(struct Ant *args);

int main()
{

    readArguments("arguments.txt");
    printf("SIMULATION_TIME is %.2f\n", SIMULATION_TIME);
    printf("NUMBER_OF_ANTS is %d\n", NUMBER_OF_ANTS);
    srand(time(NULL));
    pthread_t opengl_thread;
    pthread_t ants[NUMBER_OF_ANTS];

    if (pthread_create(&opengl_thread, NULL, (void *)opengl, NULL))
    {
        fprintf(stderr, "Failed to create opengl thread");
        exit(-1);
    }

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant *ant;
        ant = malloc(sizeof(struct Ant));
        if (ant == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for ant %d.\n", i);
            exit(-1);
        }

        (*ant).id = i;
        ant->x = randomFloat(0, SCREEN_WIDTH);
        ant->y = randomFloat(0, SCREEN_HEIGHT);
        ant->speed = randomInt(MIN_SPEED, MAX_SPEED);
        ant->direction = randomDirection();

        printf("Ant ID: %d, Position: (%f, %f), Speed: %d, Direction: %d\n",
               ant->id, ant->x, ant->y, ant->speed, ant->direction);

        printf("Ant ID in for loop: %d\n", ant->id);

        if (pthread_create(&ants[i], NULL, (void *)antsAction, ant) != 0)
        {
            fprintf(stderr, "Failed to create thread for ant %d.\n", i);
            exit(-1);
        }
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
    printf("Ant ID: %d\n", args->id);
    printf("Ant x and y %.2f %.2f\n", args->x, args->y);

    // free(data);
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

    // The text to be displayed
    char str[50];
    sprintf(str, "Number of Ants = %.2f", SIMULATION_TIME);
    printf("im str %s \n", str);
    for (int i = 0; i < strlen(str); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
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