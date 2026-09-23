#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define TOTAL_PUNTOS 1000000
#define NUM_HILOS 4

int totalPuntos = TOTAL_PUNTOS;
int numHilos = NUM_HILOS;
long puntosDentro = 0; // COMPARTIDO

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
            puntosDentro++; // acceso SIN proteger
        }
    }
    return NULL;
}

int main(void) {
    pthread_t hilos[NUM_HILOS];
    ThreadArg args[NUM_HILOS];
    long puntosPorHilo = totalPuntos / numHilos;

    for (int i = 0; i < numHilos; i++) {
        args[i].puntosPorHilo = puntosPorHilo;
        args[i].seed = (unsigned int)time(NULL) + i;
        pthread_create(&hilos[i], NULL, generar_puntos, &args[i]);
    }

    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    double pi = 4.0 * (double)puntosDentro / (double)totalPuntos;
    printf("puntosDentro = %ld\n", puntosDentro);
    printf("pi aproximado = %f\n", pi);

    return 0;
}
