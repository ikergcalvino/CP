#include <pthread.h>

#define N 8
#define MAX_WAIT 60

#define THINKING 0
#define HUNGRY 1
#define EATING 2

#define LEFT(x) (((x) + 1) % N)
#define RIGHT(x) (((x) == 0) ? (N - 1) : ((x) - 1)))

pthread_cond_t waiting[N]; // array de condiciones
pthread_mutex_t mutex;     // mutex de la mesa
int state[N];              // estado filósofo

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

void pickup(int i)
{
    pthread_mutex_lock(&mutex);

    state[i] = HUNGRY;
    while (state[LEFT(i)] == EATING || state[RIGHT(i)] == EATING)
        pthread_cond_wait(&waiting[i], &mutex);
    state[i] = EATING;

    pthread_mutex_unlock(&mutex);
}

void put_down(int i)
{
    pthread_mutex_lock(&mutex);

    state[i] = THINKING;

    if (state[LEFT(i)] == HUNGRY)
        pthread_cond_signal(&waiting[LEFT(i)]);
    if (state[RIGHT(i)] == HUNGRY)
        pthread_cond_signal(&waiting[RIGHT(i)]);

    pthread_mutex_unlock(&mutex);
}
