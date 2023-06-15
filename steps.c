/*
Ants
[0,45,90,135,180,225,270,315,360]

 1) A user-defined number of ants are created when the simulation starts (e.g. 100 ants).
 2) randomly located on the screen and walk in random directions and random speeds
 3) Directions: North (N), South (S), East(E), West (W), North-East (NE), North-West (NW), South-East (SE), South-West(SW)
 4) The speed range [1. . . 10].
 5) ant hits the limit window, it continues walking with the same speed but with an additional 45◦ angle either CW or CCW (random)
 6) ant can smell the presence of food if the food is placed anywhere within a user-defined distance from the ant
7) Once it smells the presence of food, it releases a pheromone
8) all ants whose distance with the ant that smelled the food is below a user-defined distance will shift direction and head towards the food
9) These ants will release a pheromone that is less powerful but will help in propagating the social response in more ants (the released pheromone amount is
indirectly proportional to the distance to the food)

10) However, as these ants get closer to the food source, they will release bigger quantities of pheromone.
11) Ants that smell pheromone but with lesser quantities will change direction by an angle of 5◦in the direction of the food position per second.
12) If the pheromone smelled by an ant drops below a user-defined quantity, it continues in the current direction of movement.
13) If an ant gets pheromone smell from 2 different food locations, it will favor the one with the higher smell.
14) Of course food eating should be monitored so 2 ants do not eat the same food portion.
15) Assume each ant can eat a user-defined portion (in percent) of the food per second


Food
1) food will be placed in random locations every user-defined amount of time


arguments: 
1) A user-defined number of ants
2) Pieces of food will be placed in random locations every user-defined amount of time.
3) ant can smell the presence of food, if f the food is placed anywhere within a user-defined distance from the ant
4) user-defined quantity for minimum value for phormone
5) Assume each ant can eat a user-defined portion (in percent) of the food per second
6) The simulation ends when a user-defined amount of time (in minutes) has elapsed.

check this 

In order to avoid hard-coding values in your programs, think of creating a text file
that contains all the values that should be user-defined and give the file name as an
argument to the main program. That will spare you from having to change your
code permanently and re-compile.













*/