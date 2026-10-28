#ifndef MATRIX_H
#define MATRIX_H

double *matrix_read(const char *filename, int *n);
double *matrix_random(int n);
void matrix_print(double *a, int n);

#endif
