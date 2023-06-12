#!/bin/sh

# This file only to run the project quickly for testing not for final production
# sed -i -e 's/\r$//' run.sh
#run chmod 777 run.sh to make it executable

gcc -g main.c -o main -lpthread -lrt -lglut -lGLU -lGL -lm


./main