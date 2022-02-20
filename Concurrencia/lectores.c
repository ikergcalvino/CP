#include <pthread.h>

int lectores;
pthread_mutex_t excl;          // mutex de acceso "exclusivo"
pthread_mutex_t cont_lectores; // mutex var lectores

void prio_lector()
{
    while (1)
    {
        pthread_mutex_lock(&cont_lectores);
        lectores++;        // van entrando lectores a la "cola"
        if (lectores == 1) // una vez hay un lector tiene acceso exclusivo
            pthread_mutex_lock(&excl);
        pthread_mutex_unlock(&cont_lectores);

        leer();

        pthread_mutex_lock(&cont_lectores);
        lectores--;        // van saliendo lectores de la "cola"
        if (lectores == 0) // si no hay nadie leyendo se termina la exclusividad
            pthread_mutex_unlock(&excl);
        pthread_mutex_unlock(&cont_lectores);
    }
}

void escritor()
{
    while (1)
    {
        pthread_mutex_lock(&excl); // una vez hay un escritor tiene acceso exclusivo
        escribir();
        pthread_mutex_unlock(&excl); // termina la exclusividad
    }
}

int escritores;
pthread_mutex_t wp;              // mutex de espera (waiting point)
pthread_mutex_t cont_escritores; // mutex var lectores

void lector()
{
    while (1)
    {
        pthread_mutex_lock(&wp); // no puede entrar el escritor
        pthread_mutex_lock(&cont_lectores);
        lectores++;        // van entrando lectores a la "cola"
        if (lectores == 1) // una vez hay un lector tiene acceso exclusivo
            pthread_mutex_lock(&excl);
        pthread_mutex_unlock(&cont_lectores);
        pthread_mutex_unlock(&wp); // puede esperar el escritor

        leer();

        pthead_mutex_lock(&cont_lectores);
        lectores--;        // van saliendo lectores de la "cola"
        if (lectores == 0) // si no hay nadie leyendo se termina la exclusividad
            pthread_mutex_unlock(&excl);
        pthread_mutex_unlock(&cont_lectores);
    }
}

/* Tanto lector como escritor luchan por wp,
   la única prioridad es cuando ya tienen 
   los escritores wp que hacen "cola" */

void prio_escritor()
{
    while (1)
    {
        pthread_mutex_lock(&cont_escritores);
        escritores++;                // van entrando escritores a la "cola"
        if (escritores == 1)         // una vez hay un escritores tiene acceso exclusivo
            pthread_mutex_lock(&wp); // hace esperar al lector
        pthread_mutex_unlock(&cont_escritores);
        pthread_mutex_lock(&excl);

        escribir();

        pthread_mutex_unlock(&excl);
        pthread_mutex_lock(&cont_escritores);
        escritores--;                  // van saliendo escritores de la "cola"
        if (escritores == 0)           // si no hay nadie escritores se termina la exclusividad
            pthread_mutex_unlock(&wp); // puede entrar el lector
        pthread_mutex_unlock(&cont_escritores);
    }
}
