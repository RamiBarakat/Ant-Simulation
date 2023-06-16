#ifndef __CONSTANTS__
#define __CONSTANTS__

#define PI 3.14159265

#define RGB_GRAY 0.5f, 0.5f, 0.5f
#define RGB_MOREGRAY 0.8f, 0.8f, 0.8f
#define RGB_GREEN 0.3f, 0.9f, 0.3f
#define RGB_LIGHTRED 0.9f, 0.5f, 0.5f
#define RGB_RED 0.9f, 0.0f, 0.0f
#define RGB_LIGHTGRAY 0.94f, 0.94f, 0.94f
#define RGB_WHITE 1.0f, 1.0f, 1.0f
#define RGB_BLACK 0.0f, 0.0f, 0.0f

#define BUFF_SIZE 100
#define FPS 60

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

float SIMULATION_TIME;
int NUMBER_OF_ANTS;
int MAX_SPEED;
int MIN_SPEED;
int FOOD_ADD_TIME;
float DISTANCE_ANT_FOOD;
float DISTANCE_ANT_ANT;
float PORTION_PER_SECOND;
float PHERMONE_MIN = 1.5;

int directions[] = {0, 45, 90, 135, 180, 225, 270, 315, 360};

struct Ant
{
    int id;
    double x, y; // Position
    int direction;
    int speed;      // Speed of the ant
    float phermone; // Food smell intensity
    pthread_mutex_t mutex; // Mutex for synchronization
};

struct Food
{
    int id;
    float x, y; // Position
    float quantity;
    float phermone;
    pthread_mutex_t mutex; // Mutex for synchronization
};

#endif