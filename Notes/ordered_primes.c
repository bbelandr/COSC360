// Demonstrates calculating primes, but getting the results
// in order.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_SPAN   100
#define THREADS      5

typedef struct {
    long prime_start;
    long prime_end;
} Work;

typedef struct {
    long num_primes_found;
    long *primes;
} Result;

typedef struct {
    pthread_t tid;
    Work work;
    Result result;
} Thread;

static void *worker(void *arg);
static void add(long **primes, long *cur_size, long prime);
static bool is_prime(long p);

int main(void)
{
    int i;
    Thread threads[THREADS];

    for (i = 0;i < THREADS;i+=1) {
        threads[i].work.prime_start = i * PRIME_SPAN + 2;
        threads[i].work.prime_end = threads[i].work.prime_start + (PRIME_SPAN - 1);
        pthread_create(&threads[i].tid, NULL, worker, threads + i);
    }

    for (i = 0;i < THREADS;i+=1) {
        pthread_join(threads[i].tid, NULL);
        Result *r = &threads[i].result;
        for (long j = 0;j < r->num_primes_found;j += 1) {
            printf(" %ld\n", r->primes[j]);
        }
        free(r->primes);
    }
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

    t->result.primes = NULL;
    t->result.num_primes_found = 0;

    long i;

    for (i = t->work.prime_start;i <= t->work.prime_end;i+=1) {
        if (is_prime(i)) {
            add(&t->result.primes, &t->result.num_primes_found, i);
        }
    }

    return NULL;
}

static void add(long **primes, long *cur_size, long prime)
{
    long *old = *primes;
    long *new = calloc(*cur_size + 1, sizeof(long));
    long i = 0;

    if (old != NULL) {
        for (;i < *cur_size;i+=1) {
            new[i] = old[i];
        }
    }

    new[i] = prime;
    *primes = new;
    *cur_size += 1;
    free(old);
}
