#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t fork0;
pthread_mutex_t fork1;

void *phil0(void *arg) {
    while (1) {
        printf("Philosopher 0 thinking\n");
        sleep(1);
        pthread_mutex_lock(&fork0);
        pthread_mutex_lock(&fork1);
        printf("Philosopher 0 eating\n");
        sleep(1);
        pthread_mutex_unlock(&fork1);
        pthread_mutex_unlock(&fork0);
    }
}

void *phil1(void *arg) {
    while (1) {
        printf("Philosopher 1 thinking\n");
        sleep(1);
        pthread_mutex_lock(&fork1);
        pthread_mutex_lock(&fork0);
        printf("Philosopher 1 eating\n");
        sleep(1);
        pthread_mutex_unlock(&fork0);
        pthread_mutex_unlock(&fork1);
    }
}

int main() {
    pthread_t t0, t1;
    pthread_mutex_init(&fork0, NULL);
    pthread_mutex_init(&fork1, NULL);
    pthread_create(&t0, NULL, phil0, NULL);
    pthread_create(&t1, NULL, phil1, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    return 0;
}
