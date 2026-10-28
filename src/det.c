#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "det.h"

double *A;
int N;
int task_count;
int next_task;
double result;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


double go(int row, int *used, double prod, int inv)
{
    if (row == N) {
        if (inv % 2 == 0)
            return prod;
        return -prod;
    }

    double sum = 0;
    for (int c = 0; c < N; c++) {
        if (used[c])
            continue;

        int add = 0;
        for (int k = c + 1; k < N; k++) {
            if (used[k])
                add++;
        }

        used[c] = 1;
        sum += go(row + 1, used, prod * A[row * N + c], inv + add);
        used[c] = 0;
    }
    return sum;
}

double do_task(int t)
{
    int c0 = t / N;
    int c1 = t % N;
    if (c0 == c1)
        return 0;

    int used[MAX_N] = {0};
    used[c0] = 1;
    used[c1] = 1;

    int inv = 0;
    if (c0 > c1)
        inv = 1;

    return go(2, used, A[0 * N + c0] * A[1 * N + c1], inv);
}

void *worker(void *arg)
{
    (void)arg;
    double local = 0;

    while (1) {
        pthread_mutex_lock(&mutex);
        int t = next_task;
        next_task++;
        pthread_mutex_unlock(&mutex);

        if (t >= task_count)
            break;

        local += do_task(t);
    }

    pthread_mutex_lock(&mutex);
    result += local;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

double det_calc(double *a, int n, int threads)
{
    if (n == 1)
        return a[0];

    A = a;
    N = n;
    task_count = n * n;
    next_task = 0;
    result = 0;

    pthread_t *tid = malloc(threads * sizeof(pthread_t));
    int created = 0;

    for (int i = 0; i < threads - 1; i++) {
        if (pthread_create(&tid[i], NULL, worker, NULL) != 0) {
            printf("Не удалось создать поток %d\n", i + 1);
            break;
        }
        created++;
    }

    worker(NULL);

    for (int i = 0; i < created; i++) {
        pthread_join(tid[i], NULL);
    }

    free(tid);
    return result;
}
