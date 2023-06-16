# Ants 🐜 Searching for Food 🍰 🍴 Simulation

This project aims to build a multi-threading application that simulates the behavior of a group of ants once any of them smell the presence of food. The simulation involves randomly located ants walking in random directions and speeds, the placement of food in random locations, and the release of pheromones to trigger a social response in neighboring ants.


## 🚀 Simulation Features

- Creation of a user-defined number of ants with random locations, walking directions, and speeds.
- Simulation of ant behavior when hitting the simulation window limits, including changing direction with an additional 45° angle.
- Placement of food in random locations at user-defined time intervals.
- Detection of food presence by ants within a user-defined distance.
- Release of pheromones by ants that smell food, triggering a social response in neighboring ants.
- Directional changes in ants based on pheromone smell and quantity.
- Prioritization of food sources based on pheromone intensity.
- Ant gathering on top of food and eating portions until food is consumed.
- Simulation termination after a user-defined amount of time.


## 📋 Simulation Process

The simulation follows the following process:

1. Initialization:
   - Create a user-defined number of ants with random locations, walking directions, and speeds.
   - Set simulation parameters, including simulation window limits and time intervals for food placement.
   - Initialize the graphical representation of the simulation using elements from the OpenGL library.

2. Ant Movement:
   - Each ant moves randomly in one of the eight directions: North (N), South (S), East (E), West (W), North-East (NE), North-West (NW), South-East (SE), South-West (SW).
   - If an ant hits the simulation window limits, it continues walking with the same speed but with an additional 45° angle, either clockwise (CW) or counterclockwise (CCW).

3. Food Placement:
   - Randomly place food in the simulation environment at user-defined time intervals.
   - Ensure that the distance between the food and the ants is within a user-defined range for food detection.

4. Pheromone Release and Social Response:
   - When an ant detects the presence of food, it releases a pheromone.
   - Ants within a user-defined distance from the pheromone release change their direction and head towards the food.
   - These ants release a less powerful pheromone that helps propagate the social response in more ants, with the released pheromone amount inversely proportional to the distance to the food.
   - Ants that smell pheromones but with lesser quantities change direction by 5° in the direction of the food position per second.
   - If the pheromone smelled by an ant drops below a user-defined quantity, it continues in the current direction of movement.
   - Ants favor food sources with higher pheromone smell if they receive pheromone smell from multiple food locations.

5. Food Consumption:
   - When ants gather on top of a piece of food, they stop walking and each eat a small portion of the food until it is consumed.
   - Monitor food consumption to avoid multiple ants eating the same food portion.
   - Define a user-defined portion size (in percent) of food consumed per second by each ant.

6. Simulation Termination:
   - The simulation continues until a user-defined amount of time (in minutes) has elapsed.
   - Once the time limit is reached, the simulation ends, and the results can be analyzed.


## 🤖 Tech Stack

The simulation is implemented using the following technologies:

- Linux OS
- Docker
- VNC client
- C programming language
- OpenGL library

🚧 This project is a work in progress. Further updates and enhancements will be made.

# TODO List

- [ ] Initialization:
  - [ ] Create a user-defined number of ants with random locations, walking directions, and speeds.
  - [ ] Set simulation parameters, including simulation window limits and time intervals for food placement.
  - [ ] Initialize the graphical representation of the simulation using elements from the OpenGL library.

- [ ] Ant Movement:
  - [ ] Implement random movement for each ant in one of the eight directions.
  - [ ] Handle ant behavior when hitting the simulation window limits, including changing direction with an additional 45° angle.

- [ ] Food Placement:
  - [ ] Randomly place food in the simulation environment at user-defined time intervals.
  - [ ] Ensure that the distance between the food and the ants is within a user-defined range for food detection.

- [ ] Pheromone Release and Social Response:
  - [ ] Implement pheromone release by ants when they detect the presence of food.
  - [ ] Handle directional changes in ants within a user-defined distance from the pheromone release.
  - [ ] Propagate the social response by releasing pheromones with an amount inversely proportional to the distance to the food.
  - [ ] Adjust ants' direction based on pheromone smell and quantity.
  - [ ] Implement preference for food sources with higher pheromone smell if multiple sources are detected.

- [ ] Food Consumption:
  - [ ] Implement food consumption by ants when they gather on top of a food source.
  - [ ] Monitor food portions to avoid multiple ants consuming the same portion simultaneously.
  - [ ] Define a user-defined portion size (in percent) of food consumed per second by each ant.

- [ ] Simulation Termination:
  - [ ] Set a user-defined time limit (in minutes) for the simulation.
  - [ ] End the simulation when the time limit is reached.
  - [ ] Analyze and evaluate the simulation results.

- [ ] Additional Enhancements:
  - [ ] Implement a text file to store user-defined values for simulation parameters.
  - [ ] Use graphical elements from the OpenGL library to visualize the simulation environment.
  - [ ] Ensure realistic choices and behaviors for the ants.
  - [ ] Test and debug the program, using the gdb debugger if necessary.
  - [ ] Refactor the code for clarity, modularity, and efficiency.
  - [ ] Add error handling and edge cases to enhance the robustness of the simulation.

