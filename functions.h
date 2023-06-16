#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"

void readArguments(char* filename){
    char line[200];
    char label[50];

    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL){
        perror("The file not exist\n");
        exit(-2);
    }

    char separator[] = "=";

    while(fgets(line, sizeof(line), file) != NULL){

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);


        if (strcmp(label, "SIMULATION_TIME") == 0){
            SIMULATION_TIME = atof(str);
        } else if (strcmp(label, "NUMBER_OF_ANTS") == 0){
            NUMBER_OF_ANTS = atoi(str);
        } else if (strcmp(label, "MAX_SPEED") == 0){
            MAX_SPEED = atoi(str);
        } else if (strcmp(label, "MIN_SPEED") == 0){
            MIN_SPEED = atoi(str);
        }else if (strcmp(label, "FOOD_ADD_TIME") == 0){
            FOOD_ADD_TIME = atoi(str);
        }else if (strcmp(label, "DISTANCE_ANT_FOOD") == 0){
            DISTANCE_ANT_FOOD = atof(str);
        }else if (strcmp(label, "DISTANCE_ANT_ANT") == 0){
            DISTANCE_ANT_ANT = atof(str);
        }else if (strcmp(label, "PORTION_PER_SECOND") == 0){
            PORTION_PER_SECOND = atoi(str);
        } 

    }

    fclose(file);
}

float calculateDistance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

int randomDirection() {
    int index = rand() % 8;
    return directions[index];
}

float randomFloat(float min, float max) {
    return min + ((float)rand() / RAND_MAX) * (max - min);
}

// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// set the text color to green
void green()
{
    printf("\033[32;1m");
    fflush(stdout);
}

// set the text color to red
void red()
{
    printf("\033[31;1m");
    fflush(stdout);
}

void blue()
{
    printf("\033[34;1m");
    fflush(stdout);
}

// set the text color to purple
void purple()
{
    printf("\033[35m");
    fflush(stdout);
}

// set the text color to yellow
void yellow()
{
    printf("\033[33;1m");
    fflush(stdout);
}

// reset the text attributes
void resetColor()
{
    printf("\033[0m");
    fflush(stdout);
}


void printLine()
{
    printf("|==========================================================================================|\n");
}

void printWordLine(char* word)
{
    printf("|===================================== %s ======================================|\n", word);
}





#endif