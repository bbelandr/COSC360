// Demonstrates calculating primes, but getting the results
// in order.
// This is an upgrade from ordered_primes using a thread pool.
#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
    long prime_test;
} Work;

typedef struct {
    bool is_prime;
} Result;

typedef struct {
    pthread_t tid;
    Work *work;
    Result *result;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
    int *next;
} Thread;

int NUM_TESTS;
int THREADS;

static void *worker(void *arg);
static bool is_prime(long p);

int main(int argc, char *argv[])
{
    int i;
    Thread *threads;
    Work *work;
    Result *result;
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    int next = 0;

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

    work = calloc(NUM_TESTS, sizeof(Work));
    result = calloc(NUM_TESTS, sizeof(Result));
    threads = calloc(THREADS, sizeof(Thread));

    for (i = 0;i < THREADS;i+=1) {
        threads[i].lock = &lock;
        threads[i].cond = &cond;
        threads[i].work = work;
        threads[i].result = result;
        threads[i].next = &next;
        pthread_create(&threads[i].tid, NULL, worker, threads + i);
    }

    for (i = 0;i < NUM_TESTS;i+=1) {
        work[i].prime_test = 2 + i;
    }

    // We have to keep sending the broadcast signal until 
    while (next < NUM_TESTS) {
        // By doing this, we don't need to pause, and we can keep signaling
        // even if nobody is waiting on it.
        pthread_cond_signal(&cond);
    }

    // If we get here next >= NUM_TESTS, meaning all have done their work.
    // The threads die after next >= NUM_TESTS
    pthread_cond_broadcast(&cond);
    for (i = 0;i < THREADS;i+=1) {
        pthread_join(threads[i].tid, NULL);
    }
    free(threads);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    printf("Primes\n~~~~~~\n");
    int k = 0;
    for (i =0 ;i < NUM_TESTS;i+=1) {
        if (result[i].is_prime) {
            printf("%-5ld ", work[i].prime_test);
            if (k++ > 10) {
                k = 0;
                printf("\n");
            } 
        }
    }
    if (k != 0) {
        printf("\n");
    }
    free(work);
    free(result);
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
    int mine;
    (void)arg;

    for (;;) {
        pthread_mutex_lock(t->lock);
        pthread_cond_wait(t->cond, t->lock);
        if (*t->next >= NUM_TESTS) {
            pthread_mutex_unlock(t->lock);
            break;
        }
        // Mine must be locked
        mine = (*t->next)++;
        pthread_mutex_unlock(t->lock);
        t->result[mine].is_prime = is_prime(t->work[mine].prime_test);
    }


    return NULL;
}

