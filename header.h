
/*
 * local header file for pipe client-server
*/


#ifndef LIBRARIES
#define LIBRARIES

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/shm.h>
#include <errno.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST
};

struct Ant{
    int id;
    float x, y; // Position
    enum Direction direction;
    int speed; // Speed of the ant
    int food_smell; // Food smell intensity
    pthread_mutex_t mutex; // Mutex for synchronization
};

struct Food {
    float x, y; // Position
    int quantity;
    pthread_mutex_t mutex; // Mutex for synchronization
};


#endif
