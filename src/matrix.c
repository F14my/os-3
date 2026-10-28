#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

double *matrix_read(const char *filename, int *n)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Не удалось открыть файл %s\n", filename);
        return NULL;
    }

    if (fscanf(f, "%d", n) != 1 || *n < 1) {
        printf("Неверный размер матрицы\n");
        fclose(f);
        return NULL;
    }

    double *a = malloc(*n * *n * sizeof(double));
    for (int i = 0; i < *n * *n; i++) {
        if (fscanf(f, "%lf", &a[i]) != 1) {
            printf("В файле не хватает чисел\n");
            free(a);
            fclose(f);
            return NULL;
        }
    }

    fclose(f);
    return a;
}

double *matrix_random(int n)
{
    double *a = malloc(n * n * sizeof(double));
    for (int i = 0; i < n * n; i++) {
        a[i] = rand() % 19 - 9;
    }
    return a;
}

void matrix_print(double *a, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6g ", a[i * n + j]);
        }
        printf("\n");
    }
}
