#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"

char *attachSharedMemory(int shmid)
{
    char *sharedMemory;

    // Attach to the shared memory
    sharedMemory = shmat(shmid, NULL, 0);
    if (sharedMemory == (char *)-1)
    {
        perror("shmat");
        exit(1);
    }

    return sharedMemory;
}

void detachSharedMemory(char *sharedMemory)
{
    // Detach from the shared memory
    if (shmdt(sharedMemory) == -1)
    {
        perror("shmdt");
        exit(1);
    }
}

void deleteSharedMemory(int shmid)
{
    // Delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        perror("shmctl -- delete");
        exit(1);
    }
}

int open_shmem()
{
    // Generate a unique key for the shared memory segment
    key_t sh_key = ftok(".", MEM_SEED);

    int shmid;
    // Attempt to open an existing shared memory segment or create a new one
    if ((shmid = shmget(sh_key, 0, 0)) == -1)
    {
        perror("Error opening shared memory from Parent");
        exit(-2);
    }
    // Return the shared memory identifier
    return shmid;
}

int open_sem()
{
    // Generate a unique key for the semaphore
    key_t semKey = ftok(".", SEM_SEED);

    int semid;
    // Attempt to open an existing semaphore or create a new one
    semid = semget(semKey, 1, IPC_CREAT | 0666);
    if (semid == -1)
    {
        perror("semget -- sim_system");
        exit(-1);
    }
    // Return the semaphore identifier
    return semid;
}

void lock(int semid)
{
    // Define a semaphore operation to acquire a lock
    struct sembuf acquire = {0, -1, SEM_UNDO};
    // Attempt to perform the semaphore operation to acquire the lock
    if (semop(semid, &acquire, 1) == -1)
    {
        perror("semop -- lock");
        exit(1);
    }
}

void unlock(int semid)
{
    // Define a semaphore operation to release the lock
    struct sembuf release = {0, 1, SEM_UNDO};
    // Attempt to perform the semaphore operation to release the lock
    if (semop(semid, &release, 1) == -1)
    {
        perror("semop -- unlock");
        exit(1);
    }
}

// remove the semaphore
void removeSemaphore(int seed)
{
    key_t semKey = ftok(".", seed);

    int semid = semget(semKey, 1, 0666);
    if (semid == -1)
    {
        perror("semget -- remove");
        exit(-1);
    }

    if (semctl(semid, 0, IPC_RMID, 0) == -1)
    {
        perror("semctl -- remove");
        exit(1);
    }

    printf("Semaphore is removed\n");
}

int checkColumns(int arr[], int columns)
{

    for (int i = 0; i < columns; i++)
    {
        if (arr[i] == 0)
            return 0;
    }

    return 1;
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

char *encodeMessage(char *message, int column)
{
    char *encodedMessage = malloc((strlen(message) + 1) * sizeof(char)); // Allocate memory for encoded message
    encodedMessage[0] = '\0';                                            // Initialize encodedMessage as an empty string

    // Add the column number to the encoded message
    char column_number[10];
    sprintf(column_number, "%d ", column);
    strcat(encodedMessage, column_number);

    char *token = strtok(message, " "); // Get the first token (word)
    while (token != NULL)
    {
        int message_length = strlen(token);
        int shift = 0;
        char *encodedWord = malloc((message_length + 1) * sizeof(char)); // Allocate memory for encoded word
        encodedWord[0] = '\0';                                           // Initialize encodedWord as an empty string
        int number = atoi(token);
        if (number)
        {
            number = 1000000 - number;
            sprintf(encodedWord, "%d", number);
        }
        else
        {
            for (int i = 0; i < message_length; i++)
            {
                if (token[i] >= 'a' && token[i] <= 'z')
                {
                    shift += column;
                    encodedWord[i] = (token[i] - 'a' + shift) % 26 + 'a';
                }
                else if (token[i] >= 'A' && token[i] <= 'Z')
                {
                    shift += column;
                    encodedWord[i] = (token[i] - 'A' + shift) % 26 + 'A';
                }
                else if (token[i] == '!')
                {
                    encodedWord[i] = '1';
                }
                else if (token[i] == '?')
                {
                    encodedWord[i] = '2';
                }
                else if (token[i] == ',')
                {
                    encodedWord[i] = '3';
                }
                else if (token[i] == ';')
                {
                    encodedWord[i] = '4';
                }
                else if (token[i] == ':')
                {
                    encodedWord[i] = '5';
                }
                else if (token[i] == '%')
                {
                    encodedWord[i] = '6';
                }
                else
                {
                    encodedWord[i] = token[i];
                }
            }
            encodedWord[message_length] = '\0';
        }
        strcat(encodedMessage, encodedWord);
        strcat(encodedMessage, " ");
        //free(encodedWord);         // Free memory allocated for encoded word
        token = strtok(NULL, " "); // Get the next token (word)
    }
    encodedMessage[strlen(encodedMessage) - 1] = '\0'; // Remove the last space
    return encodedMessage;
}

char *decodeMessage(char *encodedMessage)
{
    char *token = strtok(encodedMessage, " "); // Get the first token (column number)
    int column = atoi(token);

    char *decodedMessage = NULL; // Initialize decodedMessage as NULL
    size_t decodedLength = 0;

    token = strtok(NULL, " "); // Get the next token (encoded word)
    while (token != NULL)
    {
        int message_length = strlen(token);
        int shift = 0;
        char *decodedWord = malloc((message_length + 1) * sizeof(char)); // Allocate memory for decoded word
        decodedWord[0] = '\0';                                           // Initialize decodedWord as an empty string

        int number = atoi(token);
        if (number)
        {
            number = 1000000 - number;
            sprintf(decodedWord, "%d", number);
        }
        else
        {
            for (int i = 0; i < message_length; i++)
            {
                if (token[i] >= 'a' && token[i] <= 'z')
                {
                    shift += column;
                    decodedWord[i] = (token[i] - 'a' - shift + 26 + 26 + 26) % 26 + 'a';
                }
                else if (token[i] >= 'A' && token[i] <= 'Z')
                {
                    shift += column;
                    decodedWord[i] = (token[i] - 'A' - shift + 26 + 26 + 26) % 26 + 'A';
                }
                else if (token[i] == '1')
                {
                    decodedWord[i] = '!';
                }
                else if (token[i] == '2')
                {
                    decodedWord[i] = '?';
                }
                else if (token[i] == '3')
                {
                    decodedWord[i] = ',';
                }
                else if (token[i] == '4')
                {
                    decodedWord[i] = ';';
                }
                else if (token[i] == '5')
                {
                    decodedWord[i] = ':';
                }
                else if (token[i] == '6')
                {
                    decodedWord[i] = '%';
                }
                else
                {
                    decodedWord[i] = token[i]; // Copy non-decodable characters as is
                }
            }
            decodedWord[message_length] = '\0'; // Add null-terminator to the decoded word
        }

        size_t decodedWordLength = strlen(decodedWord);
        decodedMessage = realloc(decodedMessage, (decodedLength + decodedWordLength + 2) * sizeof(char));

        // Concatenate decodedWord to decodedMessage
        strcpy(decodedMessage + decodedLength, decodedWord);
        decodedLength += decodedWordLength;

        // Append a space to separate words
        decodedMessage[decodedLength] = ' ';
        decodedLength++;

        //free(decodedWord);         // Free the memory allocated for decodedWord
        token = strtok(NULL, " "); // Get the next token (encoded word)
    }

    if (decodedLength > 0)
    {
        // Remove the trailing space character
        decodedMessage[decodedLength - 1] = '\0';
    }
    else
    {
        // If the decoded message is empty, free the memory and set decodedMessage to NULL
        //free(decodedMessage);
        decodedMessage = NULL;
    }

    return decodedMessage;
}


char ** ColumntoRow(char arrange[][MAX_MSG_SIZE], int columns, int* maxsize)
{
    char* temp = (char*)malloc(MAX_MSG_SIZE * sizeof(char));
    int size = 0;
    char array[MAX_MSG_SIZE][MAX_MSG_SIZE];
    int maxSize = 0;  // Variable to store the maximum size

    for (int i = 0; i < MAX_MSG_SIZE; i++)
    {
        memset(array[i], 0, MAX_MSG_SIZE);
    }

    int k = 0;
    for (int i = 0; i < columns; i++)
    {
        size = 0;
        k = 0;
        for (int j = 0; j < MAX_MSG_SIZE; j++)
        {
            if (arrange[i][j] != ' ')
            {
                temp[k] = arrange[i][j];
                k++;
            }
            else if (arrange[i][j] == ' ')
            {
                temp[k] = '\0';
                k = 0;
                if (strcmp(temp, "Alright") != 0)
                {
                    strcat(array[size], temp);
                    strcat(array[size], " ");
                }
                size++;
                memset(temp, 0, MAX_MSG_SIZE); // Reset temp to an empty string
            }

            if (arrange[i][j] == ' ' && arrange[i][j + 1] == ' ')
            {
                strcat(array[size], " ");
                k = 0;
                strcat(array[size], " ");
                size++;
                memset(temp, 0, MAX_MSG_SIZE);
                break;
            }
        }
        if (strlen(temp) > 0)
        {
            if (strcmp(temp, "Alright") != 0)
            {
                strcat(array[size], temp);
                strcat(array[size], " ");
            }
            size++;
            memset(temp, 0, MAX_MSG_SIZE);
        }
        // Update maxSize if size is larger
        if (size > maxSize)
        {
            maxSize = size;
        }
    }

    *maxsize = maxSize; // Update the value pointed by maxsize

    char** output = (char**)malloc(maxSize * sizeof(char*));
    for (int i = 0; i < maxSize; i++)
    {
        if (array[i][0] != '\0')
        {
            output[i] = (char*)malloc(MAX_MSG_SIZE * sizeof(char));
            strcpy(output[i], array[i]);
            // printf("%s\n", array[i]);
            // printf("last row %d %s\n\n\n", maxSize ,array[maxSize - 1]);
        }
        else
            break;
    }

    //free(temp); // Free dynamically allocated memory
    return output; // No need to return output since it's modified through a pointer
}

void writeFile(char **arrange, int columns, int maxsize, int rows, const char* filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++)
    {
        fprintf(file, "%s\n", arrange[i]);
    }

    fclose(file);
}





#endif