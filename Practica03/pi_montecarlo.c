#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

long totalPuntos = 1000000;
int numHilos = 4;
long puntosDentro = 0; // COMPARTIDO
sem_t sem;

typedef struct {
    long puntosPorHilo;
    unsigned int seed;
} ThreadArg;

void *generar_puntos(void *arg) {
    ThreadArg *t = (ThreadArg *)arg;
    unsigned int seed = t->seed;

    for (long i = 0; i < t->puntosPorHilo; i++) {
        double x = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand_r(&seed) / RAND_MAX) * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            sem_wait(&sem);
            puntosDentro++;          // seccion critica
            sem_post(&sem);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc >= 2) numHilos = atoi(argv[1]);
    if (argc >= 3) totalPuntos = atol(argv[2]);

    pthread_t *hilos = malloc(sizeof(pthread_t) * numHilos);
    ThreadArg *args = malloc(sizeof(ThreadArg) * numHilos);
    long puntosPorHilo = totalPuntos / numHilos;

    sem_init(&sem, 0, 1);

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    for (int i = 0; i < numHilos; i++) {
        args[i].puntosPorHilo = puntosPorHilo;
        args[i].seed = (unsigned int)time(NULL) + i * 7919;
        pthread_create(&hilos[i], NULL, generar_puntos, &args[i]);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    double segundos = (fin.tv_sec - inicio.tv_sec) +
                       (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    double pi = 4.0 * (double)puntosDentro / (double)totalPuntos;

    printf("numHilos=%d totalPuntos=%ld puntosDentro=%ld pi=%f tiempo=%f\n",
           numHilos, totalPuntos, puntosDentro, pi, segundos);

    sem_destroy(&sem);
    free(hilos);
    free(args);

    return 0;
}
