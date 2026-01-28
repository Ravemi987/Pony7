#include <stdio.h>

/*
Somme des produits terme à terme de deux tableaux 1D
*/
float sum(float *a1, float *a2, int s) {
    float r = 0;

    for (int i = 0; i < s; i++) {
        r += a1[i] * a2[i];
    }

    return r;
}

void printFloatArray(float *a, int s) {
    for (int i = 0; i < s; ++i) {
        if (i < s-1) {
            printf("%f, ", a[i]);
        } else {
            printf("%f\n", a[i]);
        }
        
    }
}

void printIntArray(int *a, int s) {
    for (int i = 0; i < s; ++i) {
        if (i < s-1) {
            printf("%d, ", a[i]);
        } else {
            printf("%d\n", a[i]);
        }
        
    }
}
