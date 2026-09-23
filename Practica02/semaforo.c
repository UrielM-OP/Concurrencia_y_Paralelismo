// Semaforo
// Autor: Uriel Quetz Mendoza - 75457
// Fecha: 08/09/26
// Programa que simula un cocinero y dos meseros usando hilos y un semaforo

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10 // Numero de comidas

// Funciones de los hilos
static void * cocinar(void* arg);
static void * servir(void* arg);

static int counter = 0; // Contador

sem_t sem1; // Semaforo para sincronizar los hilos

int main(void)
{
  // Hilos del programa
  pthread_t cocinero, mesero1, mesero2;

  // Inicializa el semaforo en 0
  sem_init(&sem1, 0, 0);

  // Crea el hilo del cocinero
  pthread_create(&cocinero, NULL, cocinar, NULL);

  // Crea los hilos de los meseros
  pthread_create(&mesero1, NULL, servir, NULL);
  pthread_create(&mesero2, NULL, servir, NULL);

  // Espera a que terminen los hilos
  pthread_join(cocinero, NULL);
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);

  printf("Contador %d \n", counter);

  return 0;
}

// Funcion del cocinero
// Recibe un argumento y devuelve NULL
static void * cocinar(void* arg) {
  for (int i = 0; i < NR_LOOP; i++) // Prepara 10 comidas
  {
    printf("COCINERO: Comida preparada \n");

    sem_post(&sem1); // Indica que hay comida disponible

    usleep(300000); // Espera 0.3 segundos
  }

  return NULL;
}

// Funcion de los meseros
// Recibe un argumento y devuelve NULL
static void * servir(void* arg) {
  for (int i = 0; i < NR_LOOP / 2; i++) // Cada mesero sirve 5 comidas
  {
    sem_wait(&sem1); // Espera a que haya comida

    printf("MESERO: Comida servida \n");

    usleep(200000); // Espera 0.2 segundos
  }

  return NULL;
}

