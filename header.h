
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

struct Ant{
    int id;
    double x, y; // Position
    int direction;
    int speed; // Speed of the ant
    int phermone; // Food smell intensity
    int flag;
    pthread_mutex_t mutex; // Mutex for synchronization
};

struct Food {
    int id;
    float x, y; // Position
    int quantity;
    pthread_mutex_t mutex; // Mutex for synchronization
};


#endif
