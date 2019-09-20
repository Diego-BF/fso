#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_READERS 3
#define MAX_DATA 1000

sem_t mutex, db;
int reader_count = 0;
int num_data = 0;

void *reader(void *s) {
    while (num_data < MAX_DATA) {
        sem_wait(&mutex);
        reader_count++;
        if (reader_count == 1) {
            sem_wait(&db);
        }
        sem_post(&mutex);
        printf("Lendo dado %d...\n", num_data);

        sem_wait(&mutex);
        reader_count--;
        if (reader_count == 0) {
            sem_post(&db);
        }
        sem_post(&mutex);
        printf("\tUsando os dados lidos...\n");
        usleep(100);
    }
    pthread_exit(NULL);
}

void *writer(void *s) {
    while (num_data < MAX_DATA) {
        sem_wait(&db);
        printf("Escrevendo dado %d...\n", num_data);
        num_data++;
        sem_post(&db);
        usleep(100);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t writer_id, readers_id[NUM_READERS];
    int i;

    sem_init(&mutex, 0, 1);
    sem_init(&db, 0, 1);

    pthread_create(&writer_id, NULL, writer, NULL);
    for (i = 0; i < NUM_READERS; i++) {
        printf("!!! Criando leitor %d\n", i);
        pthread_create(&readers_id[i], NULL, reader, NULL);
    }

    sleep(3);

    sem_destroy(&mutex);
    sem_destroy(&db);

    return 0;
}