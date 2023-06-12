#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


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