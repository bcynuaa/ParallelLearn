#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <stdio.h>
#include <stdlib.h>

struct matrix {
    int n_row;
    int n_col;
    int *data;
}typedef matrix;

void getItem(matrix *m, int row, int col, int *item) {
    *item = m->data[row * m->n_col + col];
    return;
}

void setItem(matrix *m, int row, int col, int item) {
    m->data[row * m->n_col + col] = item;
    return;
}

void initializeMatrix(matrix *m, int n_row, int n_col) {
    m->n_row = n_row;
    m->n_col = n_col;
    m->data = (int *)malloc(n_row * n_col * sizeof(int));
    for (int i = 0; i < n_row; i++) {
        for (int j = 0; j < n_col; j++) {
            setItem(m, i, j, 0);
        }
    }
    return;
}

void print(matrix *m) {
    for (int i = 0; i < m->n_row; i++) {
        printf("Row %d: ", i);
        for (int j = 0; j < m->n_col; j++) {
            printf("%d ", m->data[i * m->n_col + j]);
        }
        printf("\n");
    }
    return;
}

void printBreakLine() {
    for (int i=0; i<50; i++) {
        printf("=");
    }
    printf("\n");
    return;
}

#endif