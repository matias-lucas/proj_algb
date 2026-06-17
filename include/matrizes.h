#ifndef MATRIZES_H
#define MATRIZES_H

typedef struct {
    int linhas;
    int colunas;
    double valores[10][10];
} Matriz;

void lerMatriz(Matriz *m);
void imprimirMatriz(Matriz *m);

#endif