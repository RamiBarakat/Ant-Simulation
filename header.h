
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

#define MAX_LENGTH 100

typedef struct {
    int id;
    double x, y; // Position
    char direction[2]; // Direction vector
    int speed; // Speed of the ant
    int food_smell; // Food smell intensity
    pthread_mutex_t mutex; // Mutex for synchronization
} ant;

typedef struct {
    double x, y; // Position
    pthread_mutex_t mutex; // Mutex for synchronization
} food;


#endif
