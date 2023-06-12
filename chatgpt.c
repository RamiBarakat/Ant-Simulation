#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 400
#define NUMBER_OF_ANTS 100

struct Position {
    double x;
    double y;
};

struct Ant {
    int id;
    struct Position position;
    int speed;
    int foodPortion;
    pthread_t thread;
};

struct Food {
    struct Position position;
    int quantity;
    pthread_mutex_t mutex;
};

struct Simulation {
    struct Ant ants[NUMBER_OF_ANTS];
    struct Food *foods;
    int numFoods;
    double smellThreshold;
};

// Function to generate a random double between min and max (inclusive)
double randomDouble(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Function to generate a random integer between min and max (inclusive)
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Function to calculate the Euclidean distance between two positions
double calculateDistance(struct Position pos1, struct Position pos2) {
    double dx = pos1.x - pos2.x;
    double dy = pos1.y - pos2.y;
    return sqrt(dx * dx + dy * dy);
}

// Function to move the ant in a random direction
void moveAnt(struct Ant *ant) {
    double angle = randomDouble(0, 2 * M_PI);
    double distance = randomDouble(0, ant->speed);

    ant->position.x += distance * cos(angle);
    ant->position.y += distance * sin(angle);

    // Check if the ant hits the window limits
    if (ant->position.x < 0 || ant->position.x > SCREEN_WIDTH || ant->position.y < 0 || ant->position.y > SCREEN_HEIGHT) {
        // Continue walking with the same speed but change direction by 45 degrees CW or CCW randomly
        angle += (randomInt(0, 1) == 0) ? M_PI / 4 : -M_PI / 4;
        ant->position.x += distance * cos(angle);
        ant->position.y += distance * sin(angle);
    }
}

// Function to check if the ant can smell the presence of food
int canSmellFood(struct Ant *ant, struct Food *food, double smellDistance) {
    double distance = calculateDistance(ant->position, food->position);
    return distance <= smellDistance;
}

// Function to update the ant's direction towards the food
void updateDirectionTowardsFood(struct Ant *ant, struct Food *food) {
    double dx = food->position.x - ant->position.x;
    double dy = food->position.y - ant->position.y;
    double angle = atan2(dy, dx);
    ant->position.x += ant->speed * cos(angle);
    ant->position.y += ant->speed * sin(angle);
}

// Function to release pheromone from an ant
void releasePheromone(struct Ant *ant, double pheromoneAmount) {
    // Release pheromone by increasing the food's quantity with synchronization
    pthread_mutex_lock(&ant->foodMutex);
    ant->foodPortion += pheromoneAmount;
    pthread_mutex_unlock(&ant->foodMutex);
}

// Function to eat food by an ant
void eatFood(struct Ant *ant) {
    // Decrease the food's quantity with synchronization
    pthread_mutex_lock(&ant->foodMutex);
    if (ant->foodPortion > 0) {
        ant->foodPortion--;
        // Simulate eating time
        usleep(1000000);
    }
    pthread_mutex_unlock(&ant->foodMutex);
}

// Function executed by each ant's thread
void *antThreadFunction(void *arg) {
    struct Ant *ant = (struct Ant *)arg;
    struct Simulation *simulation = (struct Simulation *)ant->simulation;

    while (1) {
        moveAnt(ant);

        if (canSmellFood(ant, &simulation->food, simulation->smellThreshold)) {
            updateDirectionTowardsFood(ant, &simulation->food);
            releasePheromone(ant, ant->foodPortion);
            eatFood(ant);
        }

        // Simulate ant's action time
        usleep(50000);
    }

    return NULL;
}

// Function executed by the food creation thread
void *foodCreationThreadFunction(void *arg) {
    struct Simulation *simulation = (struct Simulation *)arg;

    while (1) {
        // Create new food
        struct Food *food = (struct Food *)malloc(sizeof(struct Food));
        food->position.x = randomDouble(0, SCREEN_WIDTH);
        food->position.y = randomDouble(0, SCREEN_HEIGHT);
        food->quantity = 100;
        pthread_mutex_init(&food->mutex, NULL);

        // Add food to the simulation's array of foods
        pthread_mutex_lock(&simulation->foodMutex);
        simulation->foods = (struct Food *)realloc(simulation->foods, (simulation->numFoods + 1) * sizeof(struct Food));
        simulation->foods[simulation->numFoods] = *food;
        simulation->numFoods++;
        pthread_mutex_unlock(&simulation->foodMutex);

        // Sleep for 2 seconds before creating the next food
        sleep(2);
    }

    return NULL;
}

// Function to initialize the simulation
void initializeSimulation(struct Simulation *simulation) {
    srand(time(NULL));

    // Initialize food
    simulation->foods = NULL;
    simulation->numFoods = 0;
    pthread_mutex_init(&simulation->foodMutex, NULL);

    // Create the food creation thread
    pthread_create(&simulation->foodCreationThread, NULL, foodCreationThreadFunction, simulation);

    // Initialize ants
    for (int i = 0; i < NUMBER_OF_ANTS; i++) {
        struct Ant *ant = &simulation->ants[i];
        ant->id = i;
        ant->position.x = randomDouble(0, SCREEN_WIDTH);
        ant->position.y = randomDouble(0, SCREEN_HEIGHT);
        ant->speed = randomInt(1, 10);
        ant->foodPortion = 0;
        ant->simulation = simulation;
        pthread_create(&ant->thread, NULL, antThreadFunction, ant);
    }
}

// Function to destroy the simulation and free resources
void destroySimulation(struct Simulation *simulation) {
    // Cancel the food creation thread
    pthread_cancel(simulation->foodCreationThread);

    // Free the foods array and destroy the food mutexes
    pthread_mutex_lock(&simulation->foodMutex);
    for (int i = 0; i < simulation->numFoods; i++) {
        pthread_mutex_destroy(&simulation->foods[i].mutex);
    }
    free(simulation->foods);
    pthread_mutex_unlock(&simulation->foodMutex);
    pthread_mutex_destroy(&simulation->foodMutex);

    // Cancel the ants' threads
    for (int i = 0; i < NUMBER_OF_ANTS; i++) {
        pthread_cancel(simulation->ants[i].thread);
    }
}

int main() {
    struct Simulation simulation;
    simulation.smellThreshold = 50.0;

    initializeSimulation(&simulation);

    // Simulate the ants' behavior for a certain duration (e.g., 30 seconds)
    sleep(30);

    destroySimulation(&simulation);

    return 0;
}
