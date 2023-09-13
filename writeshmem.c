
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "shared_memory.h"

#define NUM_ITERATIONS 10

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("usage - %s [stuff to write]", argv[0]);
        return -1;
    }

    sem_t *sem_prod = sem_open(SEM_PRODUCER_FNAME, 0);
    if (sem_prod == SEM_FAILED)
    {
        perror("sem_open/producer");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_cons = sem_open(SEM_CONSUMER_FNAME, 0);
    if (sem_cons == SEM_FAILED)
    {
        perror("sem_open/consumer");
        exit(EXIT_FAILURE);
    }

    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL)
    {
        printf("ERROR: couldn't get block\n");
        return -1;
    }

    for (int i = 0; i < NUM_ITERATIONS; i++)
    {
        sem_wait(sem_cons); // consumidor esperar
        printf("Writing: \"%s\"\n", argv[1]);
        strncpy(block, argv[1], BLOCK_SIZE);
        sem_post(sem_prod); // sinalizar que algo foi produzido, ao read
    }

    sem_close(sem_prod);
    sem_close(sem_cons);
    detach_memory_block(block);

    return 0;
}