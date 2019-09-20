#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define LIMIT 10

sem_t mutex;
sem_t empty;
sem_t full;
int item = 0;

void *producer(void *s) {
    printf("Item produzido\n");

    sem_wait(&empty);
    sem_wait(&mutex);
    item = item + 1;
    printf("Item produzido inserido. Estoque: %d\n", item);
    sem_post(&mutex);
    sem_post(&full);

    pthread_exit(NULL);
}

void *consumer(void *s) {
    sem_wait(&full);
    sem_wait(&mutex);
    item = item - 1;
    printf("Um item foi removido. Estoque: %d\n", item);
    sem_wait(&mutex);
    sem_wait(&empty);
    printf("Um item foi consumido.\n");

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_id, consumer_id;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, LIMIT);
    sem_init(&full, 0, 0);

    pthread_create(&consumer_id, NULL, consumer, NULL);
    pthread_create(&producer_id, NULL, producer, NULL);

    sleep(5);

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}