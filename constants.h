#ifndef __CONSTANTS__
#define __CONSTANTS__

#define PI 3.14159265

#define RGB_GRAY 0.5f, 0.5f, 0.5f
#define RGB_MOREGRAY 0.7f, 0.7f, 0.7f
#define RGB_GREEN 0.0f, 1.0f, 0.0f
#define RGB_LIGHTRED 0.9f, 0.5f, 0.5f
#define RGB_RED 0.9f, 0.0f, 0.0f
#define RGB_LIGHTGRAY 0.94f, 0.94f, 0.94f

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

float SIMULATION_TIME;
int NUMBER_OF_ANTS;
int MAX_SPEED;
int MIN_SPEED;
int FOOD_ADD_TIME;
float DISTANCE_ANT_FOOD;
float DISTANCE_ANT_ANT;
float PHERMONE_MIN = 1.5;

int directions[] = {0, 45, 90, 135, 180, 225, 270, 315, 360};

#endif