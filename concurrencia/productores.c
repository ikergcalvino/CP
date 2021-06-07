#include <pthread.h>

typedef void *elemento;

void insert(elemento e);
elemento remove();
int elements();    // Número de elementos en el buffer
int buffer_size(); // Capacidad máxima del buffer

pthread_mutex_t *buffer_lock;

void productor()
{
    while (1)
    {
        elemento e = crear_elemento();
        pthread_mutex_lock(buffer_lock); // lock sección crítica
        insert(e);
        pthread_mutex_unlock(buffer_lock); // unlock sección crítica
    }
}

void consumidor()
{
    while (1)
    {
        elemento e;
        pthread_mutex_lock(buffer_lock); // lock sección crítica
        e = remove();
        pthread_mutex_unlock(buffer_lock); // unlock sección crítica
        // Hacer algo con el elemento :)
    }
}

void productor_limitbuffer()
{
    int inserted;

    while (1)
    {
        elemento e = crear_elemento();
        inserted = 0;
        do
        {
            pthread_mutex_lock(buffer_lock); // lock sección crítica
            if (elements() < buffer_size())  // si hay sitio en el buffer
            {
                insert(e);
                inserted = 1;
            }
            pthread_mutex_unlock(buffer_lock); // unlock sección crítica
        } while (!inserted);                   // espera activa hasta poder insertar
    }
}

void consumidor_limitbuffer()
{
    int removed;

    while (1)
    {
        elemento e;
        removed = 0;
        do
        {
            pthread_mutex_lock(buffer_lock); // lock sección crítica
            if (elements() > 0)              // si hay algún elemento en el buffer
            {
                e = remove();
                removed = 1;
            }
            pthread_mutex_unlock(buffer_lock); // unlock sección crítica
        } while (!removed);                    // espera activa hasta poder eliminar
        // Hacer algo con el elemento :)
    }
}

pthread_cond_t *buffer_full;  // condición cuando el buffer tiene mínimo un hueco
pthread_cond_t *buffer_empty; // condición cuando el buffer tiene mínimo un elemento

void productor_cond()
{
    while (1)
    {
        elemento e = crear_elemento();
        pthread_mutex_lock(buffer_lock);    // lock sección crítica
        while (elements() == buffer_size()) // Esperar por sitio
            pthread_cond_wait(buffer_full, buffer_lock);
        insert(e);
        if (elements() == 1)
            pthread_cond_broadcast(buffer_empty); // buffer casi vacío
        pthread_mutex_unlock(buffer_lock);        // unlock sección crítica
    }
}

void consumidor_cond()
{
    while (1)
    {
        elemento e;
        pthread_mutex_lock(buffer_lock); // lock sección crítica
        while (elements() == 0)          // Esperar por elementos
            pthread_cond_wait(buffer_empty, buffer_lock);
        e = remove();
        if (elements() == buffer_size() - 1)
            pthread_cond_broadcast(buffer_full); // buffer casi lleno
        pthread_mutex_unlock(buffer_lock);       // unlock sección crítica
        // Hacer algo con el elemento :)
    }
}
