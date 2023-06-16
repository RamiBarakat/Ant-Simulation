#include "header.h"
#include "functions.h"
#include "constants.h"

void moveAnt(int);
void lookForFood(int);
void smellPhermone(int);
void opengl();
void *antsAction(void *args);
void *foodCreation(void *arg);

struct Ant *ants;
struct Food *foods;

pthread_t *ants_threads;
pthread_t *foods_threads;
pthread_t opengl_thread;

float radius = 0.02;
int NUMBER_OF_FOOD;
int food_counter = 0;
int ant_counter = 0;

float speed = 0.000001;
float scalingFactor;
float scaledSpeed;

bool terminate = false;
const int numSegments = 100;
float *cosValues;
float *sinValues;

float plateRadius = 0.1;

char text_buffer[BUFF_SIZE];
char debug = 1;
float fps, font_height;

void handle_alarm(int signal);

int main()
{
    readArguments("arguments.txt");
    printf("SIMULATION_TIME is %.2f\n", SIMULATION_TIME);
    printf("NUMBER_OF_ANTS is %d\n", NUMBER_OF_ANTS);
    srand(time(NULL));

    scalingFactor = (float)NUMBER_OF_ANTS / 100;
    scaledSpeed = speed * scalingFactor;

    NUMBER_OF_FOOD = (SIMULATION_TIME * 60) / FOOD_ADD_TIME;

    ants_threads = malloc(NUMBER_OF_ANTS * sizeof(pthread_t));
    if (ants_threads == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ants_threads.\n");
        exit(-1); // Or handle the error in an appropriate way
    }

    ants = malloc(sizeof(struct Ant) * NUMBER_OF_ANTS);
    if (ants == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for ants.\n");
        exit(-1); // Or handle the error in an appropriate way
    }

    foods = malloc(sizeof(struct Food) * NUMBER_OF_FOOD);
    if (foods == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for foods.\n");
        exit(-1); // Or handle the error in an appropriate way
    }

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        struct Ant ant;
        ant.id = i + 1;
        ant.x = randomFloat(-SCREEN_WIDTH, SCREEN_WIDTH) / SCREEN_WIDTH;
        ant.y = randomFloat(-SCREEN_HEIGHT, SCREEN_HEIGHT) / SCREEN_HEIGHT;
        ant.speed = randomInt(MIN_SPEED, MAX_SPEED);
        ant.direction = randomDirection();
        ant.phermone = 0;
        if (pthread_mutex_init(&ant.mutex, NULL) != 0)
        {
            fprintf(stderr, "Failed to initialize mutex for ant %d.\n", ant.id);
            exit(EXIT_FAILURE); // Or handle the error in an appropriate way
        }

        ants[i] = ant;
        printf("Ant ID: %d, Position: (%f, %f), Speed: %d, Direction: %d\n",
               ant.id, ant.x, ant.y, ant.speed, ant.direction);

        int *a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&ants_threads[i], NULL, &antsAction, a) != 0)
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

    if (signal(SIGALRM, handle_alarm) == SIG_ERR)
    {
        perror("Failed to set signal handler for SIGALRM");
        exit(1);
    }
    unsigned int SIM_END = (unsigned int)(SIMULATION_TIME * 60.0f);
    alarm(SIM_END);

    while (1)
    {
        // Create new food
        struct Food food;
        food.id = food_counter + 1;
        food.x = randomFloat(-SCREEN_WIDTH, SCREEN_WIDTH) / SCREEN_WIDTH;
        food.y = randomFloat(-SCREEN_HEIGHT, SCREEN_HEIGHT) / SCREEN_HEIGHT;
        food.quantity = 100;
        if (pthread_mutex_init(&food.mutex, NULL) != 0)
        {
            fprintf(stderr, "Failed to initialize mutex for Food %d.\n", food.id);
            exit(EXIT_FAILURE); // Or handle the error in an appropriate way
        }

        foods[food_counter] = food;
        food_counter++;
        sleep(FOOD_ADD_TIME);
        if (terminate)
        {
            pthread_cancel(opengl_thread);

            for (int i = 0; i < NUMBER_OF_ANTS; i++)
            {
                pthread_cancel(ants_threads[i]);
            }
            for (int i = 0; i < NUMBER_OF_ANTS; i++)
            {
                pthread_mutex_destroy(&ants[i].mutex);
            }

            for (int i = 0; i < food_counter; i++)
            {
                pthread_mutex_destroy(&foods[i].mutex);
            }

            break;
        }
    }
    printf("Done\n");
    return 0;
}

void *antsAction(void *arg)
{
    int index = *(int *)arg;
    while (!terminate)
    {
        if (pthread_mutex_lock(&ants[index].mutex) != 0)
        {
            fprintf(stderr, "Couldn't unlock the mutex of Ant");
            exit(-1);
        }
        moveAnt(index);
        lookForFood(index);
        smellPhermone(index);
        if (pthread_mutex_unlock(&ants[index].mutex) != 0)
        {
            fprintf(stderr, "Couldn't unlock the mutex of Ant");
            exit(-1);
        }
    }
    return 0;
}

void moveAnt(int index)
{
    // Convert angle to radians
    float radian = (ants[index].direction) * M_PI / 180.0;
    float dx = scaledSpeed * ants[index].speed * cosf(radian);
    float dy = scaledSpeed * ants[index].speed * sinf(radian);
    // Update the ants[index]'s position
    ants[index].x += dx;
    ants[index].y += dy;
    if (ants[index].x < -1 || ants[index].x > 1 || ants[index].y < -1 || ants[index].y > 1)
    {
        ants[index].direction += (rand() % 2 == 0) ? 135 : -135;
        ants[index].direction = ants[index].direction % 360;
    }
}

void lookForFood(int index)
{
    int closestFood = -1;
    float closestDistance = 0.0;

    for (int i = 0; i < food_counter; i++)
    {
        float distance = calculateDistance(ants[index].x, ants[index].y, foods[i].x, foods[i].y);
        if (distance <= DISTANCE_ANT_FOOD && foods[i].quantity > 0)
        {
            if (closestFood == -1 || distance < closestDistance)
            {
                closestFood = i;
                closestDistance = distance;
            }
        }
    }
    if (closestFood != -1)
    {
        float distance = closestDistance;
        float pheromoneAmount = 1.0 / distance;
        ants[index].phermone += pheromoneAmount;

        float dx = foods[closestFood].x - ants[index].x;
        float dy = foods[closestFood].y - ants[index].y;
        float angle = atan2(dy, dx);
        ants[index].direction = angle * (180.0 / M_PI);

        ants[index].phermone = 0;
        if (distance < (0.05 + plateRadius) && foods[closestFood].quantity > 0)
        {
            ants[index].speed = 0;
            if (pthread_mutex_lock(&foods[closestFood].mutex) != 0)
            {
                fprintf(stderr, "Couldn't lock the mutex of food");
                exit(-1);
            }
            foods[closestFood].quantity -= PORTION_PER_SECOND;
            if (foods[closestFood].quantity > 0)
                usleep(500000);
            if (pthread_mutex_unlock(&foods[closestFood].mutex) != 0)
            {
                fprintf(stderr, "Couldn't unlock the mutex of food");
                exit(-1);
            }
            ants[index].speed = randomInt(MIN_SPEED, MAX_SPEED);
        }
    }
}

void smellPhermone(int index)
{
    for (int i = 0; i < ant_counter; i++)
    {
        if (i + 1 == ants[index].id)
            continue;

        float distance = calculateDistance(ants[index].x, ants[index].y, ants[i].x, ants[i].y);
        if (distance <= DISTANCE_ANT_ANT && ants[i].phermone > PHERMONE_MIN)
        {
            float dx = ants[i].x - ants[index].x;
            float dy = ants[i].y - ants[index].y;
            float angle = atan2(dy, dx);
            ants[index].direction = angle * (180.0 / M_PI);
        }
        else if (ants[index].phermone < PHERMONE_MIN && distance <= DISTANCE_ANT_ANT && ants[index].phermone != 0)
        {
            ants[index].direction += 10 * (180.0 / M_PI);
        }
    }
}

void handle_alarm(int signum)
{
    terminate = true;
    printf("Signal Received: Program Finished!\n");
}

/****************************************** OPENGL ************************************************/

void initializeCircleValues()
{
    const float angleIncrement = 2.0 * M_PI / numSegments;
    float angle = 0.0;
    for (int i = 0; i < numSegments; i++)
    {
        cosValues[i] = cos(angle);
        sinValues[i] = sin(angle);
        angle += angleIncrement;
    }
}

void drawPlate(float x, float y, float radius, int quantity, float scaleFactor)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glScalef(scaleFactor, scaleFactor, 1.0);

    glColor3f(RGB_LIGHTGRAY);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * PI / 180.0;
        float px = radius * cos(angle);
        float py = radius * sin(angle);
        glVertex2f(px, py);
    }
    glEnd();

    glColor3f(RGB_GREEN);

    float angleIncrement = 360.0 / quantity;

    for (int i = 0; i < quantity; i++)
    {
        float angle = i * angleIncrement * PI / 180.0;
        float fx = (radius * 0.8) * cos(angle);
        float fy = (radius * 0.8) * sin(angle);

        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++)
        {
            float angle2 = j * PI / 180.0;
            float px = fx + 0.015 * cos(angle2);
            float py = fy + 0.015 * sin(angle2);
            glVertex2f(px, py);
        }
        glEnd();
    }

    glPopMatrix();
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

void drawAnt(float x, float y, float rotateAngle, float scaleFactor)
{
    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glRotatef(rotateAngle, 0.0, 0.0, 1.0);
    glScalef(scaleFactor, scaleFactor, 1.0);

    // Body
    glColor3f(RGB_BLACK);
    drawCircle(0.0, 0.0, 0.2);

    // Head
    glColor3f(RGB_MOREGRAY);
    drawCircle(0.25, 0.0, 0.1);

    // Eyes
    glColor3f(RGB_BLACK);
    drawCircle(0.3, 0.05, 0.02);
    drawCircle(0.3, -0.05, 0.02);

    // Antennae (lines)
    glColor3f(RGB_BLACK);
    glBegin(GL_LINES);
    glVertex2f(0.3, 0.05);
    glVertex2f(0.4, 0.1);
    glVertex2f(0.3, -0.05);
    glVertex2f(0.4, -0.1);
    glEnd();

    glPopMatrix();
}

void draw_text(float x, float y, char *string)
{
    char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
    }
}

void draw_debug()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    if (fps < (float)FPS * 0.8f)
        glColor3f(RGB_RED);
    snprintf(text_buffer, BUFF_SIZE, "FPS: %f (%d)", fps, FPS);
    draw_text(-1, -0.8, text_buffer);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    snprintf(text_buffer, BUFF_SIZE, "Speed(Min: %d, Max: %d)",
             MIN_SPEED, MAX_SPEED);
    draw_text(-1, -0.85, text_buffer);

    snprintf(text_buffer, BUFF_SIZE, "Food_count: %d", food_counter);
    draw_text(-1, -1, text_buffer);

    snprintf(text_buffer, BUFF_SIZE, "Ants count: %d", ant_counter);
    draw_text(-1, -0.95, text_buffer);

    snprintf(text_buffer, BUFF_SIZE, "Field(width: %d, height: %d)",
             SCREEN_WIDTH, SCREEN_HEIGHT);
    draw_text(-1, -0.9, text_buffer);
}

void calculate_fps()
{
    static int current_time, previous_time, frame_count;
    frame_count++;

    current_time = glutGet(GLUT_ELAPSED_TIME);

    int time_interval = current_time - previous_time;

    if (time_interval > 1000)
    {
        fps = frame_count / (time_interval / 1000.0f);

        previous_time = current_time;

        frame_count = 0;
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Set the background color to White
    glRasterPos2f(0, 0);
    glColor3f(RGB_WHITE);

    // Draw a rectangle that covers the entire window
    glBegin(GL_QUADS);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, -1.0);
    glEnd();

    calculate_fps();
    if (debug)
    {
        draw_debug();
    }

    for (int i = 0; i < food_counter; i++)
    {
        if (foods[i].quantity > 0)
            drawPlate(foods[i].x, foods[i].y, plateRadius, (int)foods[i].quantity / 10, 0.8);
    }

    for (int i = 0; i < NUMBER_OF_ANTS; i++)
    {
        drawAnt(ants[i].x, ants[i].y, ants[i].direction, 0.1);
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

void update()
{
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void opengl()
{
    font_height = 10.0f;

    // Dynamically allocate memory for the arrays
    cosValues = (float *)malloc(numSegments * sizeof(float));
    sinValues = (float *)malloc(numSegments * sizeof(float));

    initializeCircleValues();

    int c = 1;
    glutInit(&c, NULL);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Project 3: POSIX THREADS");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    while (!terminate)
    {
        glutMainLoop();
    }
}