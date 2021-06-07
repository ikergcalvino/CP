#include <pthread.h>

#define N 8         // número de filósofos
#define MAX_WAIT 60 // tiempo máximo de espera

#define RIGHT(i) (i)            // filósofo N: tenedor drch (N)
#define LEFT(i) (((i) + 1) % N) // filósofo N: tenedor izqd (N-1)

pthread_mutex_t *tenedor[N]; // array de mutex (mutex x tenedor)

filosofo(int i)
{
    while (1)
    {
        think();
        pickup(i);
        eat();
        put_down(i);
    }
}

void pickup(int num)
{
    int left = LEFT(num);
    int right = RIGHT(num);
    int success = 0;
    do
    {
        pthread_mutex_lock(tenedor[left]);
        if (!pthread_mutex_trylock(tenedor[right]))
            success = 1;
        else
        {
            pthread_mutex_unlock(tenedor[left]);
            usleep(rand() % MAX_WAIT);
        }
    } while (!success); // repetir hasta tener los 2 tenedores
}

void put_down(int num)
{
    int left = LEFT(num);
    int right = RIGHT(num);
    pthread_mutex_unlock(tenedor[right]);
    pthread_mutex_unlock(tenedor[left]);
}
