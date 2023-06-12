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
            SIMULATION_TIME = atoi(str);
        } 
        // else if (strcmp(label, "RANDOM_NUMBER") == 0){
        //     RANDOM_NUMBER = atoi(str);
        // }else if(strcmp(label, "THE_PRICE") == 0){
        //     THE_PRICE = atof(str);
        // }else if(strcmp(label, "THE_LEADER_NAME") == 0){
        //     strncpy(THE_LEADER_NAME, str, sizeof(THE_LEADER_NAME));
        // }



    }

    fclose(file);
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