// Demonstrates calculating primes, but getting the results
// in order.
// This uses a thread pool, but now we can specifically target
// specific threads.

// This uses a lot less memory than the primes_pool does since
// we only allocate the work and result per thread.
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef enum {
    WORK,
    RESULT,
    DIE
} WorkCommand;

typedef struct {
    long number;
    bool is_prime;
} Data;

typedef struct {
    pthread_t tid;
    WorkCommand command;
    Data *data;
    pthread_mutex_t lock;
} Thread;

int NUM_TESTS;
int THREADS;

static void *worker(void *arg);
static bool is_prime(long p);

int main(int argc, char *argv[])
{
    int i;
    int k;
    int tests;
    int queued;
    Thread *threads;
    Data *data;

    if (argc < 3) {
        printf("Usage: %s <num threads> <num tests>\n", argv[0]);
        return -1;
    }

    if (sscanf(argv[1], "%d", &THREADS) != 1) {
        printf("Invalid number of threads specified '%s'.\n", argv[1]);
        return -1;
    }

    if (sscanf(argv[2], "%d", &NUM_TESTS) != 1) {
        printf("Invalid number of tests specified '%s'.\n", argv[2]);
        return -1;
    }

    data = calloc(THREADS, sizeof(Data));
    threads = calloc(THREADS, sizeof(Thread));

    for (i = 0;i < THREADS;i+=1) {
        pthread_mutex_init(&threads[i].lock, NULL);
        pthread_mutex_lock(&threads[i].lock);
        threads[i].data = data + i;
        pthread_create(&threads[i].tid, NULL, worker, threads + i);
    }

    tests = 0;
    k = 0;
    while (tests < NUM_TESTS) {
        // Give work
        queued = 0;
        for (i = 0;i < THREADS && tests < NUM_TESTS;i+=1) {
            threads[i].data->number = 2 + tests;
            threads[i].command = WORK;
            tests += 1;
            queued += 1;
            // Unlock the mutex to get the thread to start.
            pthread_mutex_unlock(&threads[i].lock);
        }

        // Get results
        for (i = 0;i < queued;i+=1) {
            while (threads[i].command != RESULT) {
                // Give a little pause if we don't have the command yet.
                // We can't continue on since these need to be in order.
                nanosleep(&(struct timespec){0, 250000}, NULL);
            }
            // If we get here, the prime has been calculated
            if (threads[i].data->is_prime) {
                printf("%-5ld ", threads[i].data->number);
                if (k++ > 10) {
                    k = 0;
                    printf("\n");
                } 
            }
        }
    }
    if (k != 0) {
        puts("");
    }

    for (i = 0;i < THREADS;i+=1) {
        threads[i].command = DIE;
        pthread_mutex_unlock(&threads[i].lock);
        pthread_join(threads[i].tid, NULL);
        pthread_mutex_destroy(&threads[i].lock);
    }
    free(threads);
    free(data);
}

static bool is_prime(long p)
{
    long i;
    for (i = 2;i * i <= p;i+=1) {
        if ((p % i) == 0) {
            return false;
        }
    }
    return true;
}

static void *worker(void *arg)
{
    Thread *t = (Thread *)arg;

    for (;;) {
        // Our signal is the mutex here. The mutex is initially locked,
        // so we block here until the marshaller unlocks the mutex. After,
        // we are able to acquire it. When we are done with work, the loop
        // goes back and waits on the already locked mutex, since we never
        // unlock it in this loop.
        pthread_mutex_lock(&t->lock);
        if (t->command == DIE) {
            break;
        }
        else if (t->command == WORK) {
            t->data->is_prime = is_prime(t->data->number);
            t->command = RESULT;
        }
    }

    return NULL;
}

