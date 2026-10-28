#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "matrix.h"
#include "det.h"


double now_sec(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(int argc, char *argv[])
{
    int threads = 0;
    int n = 0;
    char *filename = NULL;
    int print = 0;

    int opt;
    while ((opt = getopt(argc, argv, "t:f:n:p")) != -1) {
        if (opt == 't')
            threads = atoi(optarg);
        else if (opt == 'f')
            filename = optarg;
        else if (opt == 'n')
            n = atoi(optarg);
        else if (opt == 'p')
            print = 1;
        else {
            printf("Использование: %s -t потоки (-f файл | -n размер) [-p]\n", argv[0]);
            return 1;
        }
    }

    if (threads < 1 || threads > 256) {
        printf("Число потоков (-t) должно быть от 1 до 256\n");
        return 1;
    }

    double *a;
    if (filename != NULL) {
        a = matrix_read(filename, &n);
        if (a == NULL)
            return 1;
    } else if (n >= 1) {
        srand(1);
        a = matrix_random(n);
    } else {
        printf("Укажите -f файл или -n размер\n");
        return 1;
    }

    if (n > MAX_N) {
        printf("Слишком большая матрица (максимум %d)\n", MAX_N);
        free(a);
        return 1;
    }

    if (print)
        matrix_print(a, n);

    printf("PID: %d\n", getpid());
    printf("n = %d, threads = %d\n", n, threads);
    fflush(stdout);

    double start = now_sec();
    double det = det_calc(a, n, threads);
    double end = now_sec();

    printf("det = %.10g\n", det);
    printf("time = %.6f\n", end - start);

    free(a);
    return 0;
}
