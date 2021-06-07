#include <pthread.h>

#define WORKING 0  // barbero no disponible
#define SLEEPING 1 // barbero disponible

#define MAX_CUSTOMERS 10 // número máximo de clientes

int barber_state;            // estado del barbero
int barberos_libres;         // número de barberos libres
int waiting_customers;       // número de clientes en la sala de espera
pthread_mutex_t mutex;       // mutex para proteger las variables
pthread_cond_t no_customers; // no hay clientes en la sala de espera
pthread_cond_t waiting_room; // avisar a un cliente de la sala de espera

void barbero()
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (!waiting_customers)
        {
            barber_state = SLEEPING;
            //barberos_libres++; /* implementación múltiples barberos */
            pthread_cond_wait(&no_customers, &mutex); // esperar a que haya cliente
        }
        else
        {
            pthread_cond_signal(&waiting_room); // avisar a un cliente
            waiting_customers--;
        }
        pthread_mutex_unlock(&mutex);
        cut_hair();
    }
}

void cliente()
{
    pthread_mutex_lock(&mutex);
    if (waiting_customers == MAX_CUSTOMERS)
        pthread_mutex_unlock(&mutex);
    else
    {
        if (barber_state == SLEEPING)
        {
            pthread_cond_signal(&no_customers); // avisar de que tiene que trabajar
            barber_state = WORKING;
            //barberos_libres--; /* implementación múltiples barberos */
        }
        else
        {
            waiting_customers++;
            pthread_cond_wait(&waiting_room, &mutex); // esperar a que haya barbero libre
        }
        pthread_mutex_unlock(&mutex);
        get_hair_cut();
    }
}
