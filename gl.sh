#!/bin/sh
gcc -o opengl opengl.c -lGL -lGLU -lglut -lm
./opengl