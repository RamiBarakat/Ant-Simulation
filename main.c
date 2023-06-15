#include "header.h"
#include "functions.h"
#include "constants.h"

bool openGlIsFinished = false;

void opengl();
void *antsAction(struct Ant *args);
void *foodCreation(void *arg);
struct Ant **ants;
struct Food **foods;
float radius = 0.02; // Radius of the circle
int numSegments = 200;
int NUMBER_OF_FOOD;
int food_counter = 0;
int ant_counter = 0;
void moveAnt(struct Ant *ant);
float speed = 0.00000001;

int main()
{

    readArguments("arguments.txt");
    printf("SIMULATION_TIME is %.2f\n", SIMULATION_TIME);
    printf("NUMBER_OF_ANTS is %d\n", NUMBER_OF_ANTS);
    srand(time(NULL));

    NUMBER_OF_FOOD = (SIMULATION_TIME * 60) / FOOD_ADD_TIME;

    pthread_t opengl_thread;
    pthread_t ants_threads[NUMBER_OF_ANTS];
    pthread_t foods_threads[NUMBER_OF_FOOD];

    ants = malloc(sizeof(struct Ant *) * NUMBER_OF_ANTS);
    foods = malloc(sizeof(struct Food *) * NUMBER_OF_FOOD);

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
        ant->phermone = 0;
        pthread_mutex_init(&ant->mutex, NULL);
        ant->flag = 0;

        printf("Ant ID: %d, Position: (%f, %f), Speed: %d, Direction: %d\n",
               ant->id, ant->x, ant->y, ant->speed, ant->direction);

        if (pthread_create(&ants_threads[i], NULL, (void *)antsAction, ant) != 0)
        {
            fprintf(stderr, "Failed to create thread for ant %d.\n", i);
            exit(-1);
        }
        ant_counter++;
    }

    sleep(1);
    if (pthread_create(&opengl_thread, NULL, (void *)opengl, NULL))
    {
        fprintf(stderr, "Failed to create opengl thread");
        exit(-1);
    }

    while (1)
    {
        // Create new food
        struct Food *food = malloc(sizeof(struct Food));
        food->id = food_counter + 1;
        food->x = randomFloat(-SCREEN_WIDTH, SCREEN_WIDTH) / SCREEN_WIDTH;
        food->y = randomFloat(-SCREEN_HEIGHT, SCREEN_HEIGHT) / SCREEN_HEIGHT;
        food->quantity = 100;
        pthread_mutex_init(&food->mutex, NULL);

        foods[food_counter] = food;

        if (pthread_create(&foods_threads[food_counter], NULL, (void *)foodCreation, NULL) != 0)
        {
            fprintf(stderr, "Failed to create thread for ant %d.\n", food_counter);
            exit(-1);
        }
        food_counter++;
        sleep(FOOD_ADD_TIME);

        // if (food_counter == 3)
        // {
        //     sleep(5);

        //     for (int i = 0; i < NUMBER_OF_ANTS; i++)
        //     {
        //         pthread_cancel(ants_threads[i]);
        //     }

        //     for (int i = 0; i < NUMBER_OF_FOOD; i++)
        //     {
        //         pthread_cancel(foods_threads[i]);
        //     }
        //     free(foods);
        //     free(ants);
        //     pthread_cancel(opengl_thread);
        //     break;
        // }
    }

    // sleep(5);
    // openGlIsFinished = true;

    return 0;
}

void *antsAction(struct Ant *ant)
{
    printf("HI\n");
    while (1)
    {
        moveAnt(ant);

        for (int i = 0; i < food_counter; i++)
        {
            struct Food *food = foods[i];
            float distance = calculateDistance(ant->x, ant->y, food->x, food->y);

            if (distance <= DISTANCE_ANT_FOOD && food->quantity > 0)
            {
                float pheromoneAmount = 1.0 / distance;
                ant->phermone += pheromoneAmount;

                double dx = food->x - ant->x;
                double dy = food->y - ant->y;
                double angle = atan2(dy, dx);
                ant->direction = angle * (180.0 / M_PI);

                ant->x += cos(angle) * ant->speed * speed;
                ant->y += sin(angle) * ant->speed * speed;

                ant->phermone = 0;

                if (distance < 0.05 && food->quantity > 0)
                {
                    pthread_mutex_lock(&food->mutex);
                    ant->speed = 0;
                    food->quantity -= 5;
                    sleep(1);
                    ant->speed = randomInt(MIN_SPEED, MAX_SPEED);
                    pthread_mutex_unlock(&food->mutex);
                }
            }
        }

        for (int i = 0; i < ant_counter; i++)
        {
            if (i + 1 == ant->id)
                continue;

            float distance = calculateDistance(ant->x, ant->y, ants[i]->x, ants[i]->y);
            if (distance <= DISTANCE_ANT_ANT && ants[i]->phermone > PHERMONE_MIN)
            {
                double dx = ants[i]->x - ant->x;
                double dy = ants[i]->y - ant->y;
                double angle = atan2(dy, dx);
                ant->direction = angle * (180.0 / M_PI);
                ant->x += cos(angle) * ant->speed * speed;
                ant->y += sin(angle) * ant->speed * speed;
            }
            else if (ant->phermone < PHERMONE_MIN && distance <= DISTANCE_ANT_ANT && ant->phermone != 0)
            {
                ant->direction = 10 * (180.0 / M_PI);
                ant->x += cos(10) * ant->speed * speed;
                ant->y += sin(10) * ant->speed * speed;
            }
        }
    }
}

void moveAnt(struct Ant *ant)
{

    if (!(ant->x < -1 || ant->x > 1 || ant->y < -1 || ant->y > 1))
    {
        ant->flag = 0;
    }
    float radian = (ant->direction) * M_PI / 180.0; // Convert angle to radians

    double dx = 0.000000005 * ant->speed * cos(radian);
    double dy = 0.000000005 * ant->speed * sin(radian);
    // Update the ant's position
    ant->x += dx;
    ant->y += dy;
    if (ant->x < -1 || ant->x > 1 || ant->y < -1 || ant->y > 1)
    {
        if (!ant->flag)
        {
            // printf("hiiiiiiiiiii\n");
            // ant->direction += (rand() % 2 == 0) ? 135 : -135;
            ant->direction += 135;
            ant->direction = ant->direction % 360;
            ant->flag = 1;
        }
    }
}

void *foodCreation(void *arg)
{
    // while(1){

    //     if (food->occupied)
    //     {
    //         food->quantity -= 0.01;

    //     }

    //     sleep(1);
    // }
}

void deleteThreads()
{
}

void drawFilledCircle(float centerX, float centerY, float radius)
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
        drawFilledCircle(ant->x, ant->y, 0.02);
        // printf("Ant %d - Position: (%f, %f)\n", ant->id, ant->x, ant->y);
        //  printf("Ant %d - Position: (%d)\n", ant->id, ant->direction);
    }

    for (int i = 0; i < food_counter; i++)
    {
        struct Food *food = foods[i];
        glVertex2f(food->x, food->y);
        // printf("food->id %d food->quantity %f \n", food->id,food->quantity);
        if (food->quantity > 0)
            drawFilledCircle(food->x, food->y, food->quantity * 0.0004);
        // printf("Food %d - Position: (%f, %f)\n", food->id, food->x, food->y);
        // printf("Ant %d - Position: (%d)\n", ant->id, ant->direction);
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
    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

void opengl()
{
    printf("Hi im in opengl\n");
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