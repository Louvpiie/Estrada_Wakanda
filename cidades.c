#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

// Função auxiliar para comparar cidades pela posição, usada para ordenar
int compararCidades(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *) a;
    Cidade *cidadeB = (Cidade *) b;
    return cidadeA->Posicao - cidadeB->Posicao;
}

// Função para inicializar a estrada e carregar as cidades do arquivo
Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s.\n", nomeArquivo);
        return NULL;
    }

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        fprintf(stderr, "Erro: Falha ao alocar memória para estrada.\n");
        fclose(arquivo);
        return NULL;
    }

    // Leitura do comprimento da estrada
    if (fscanf(arquivo, "%d", &estrada->T) != 1 || estrada->T < 3 || estrada->T > 1000000) {
        fprintf(stderr, "Erro: Comprimento da estrada (T) inválido ou fora do intervalo.\n");
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    // Leitura do número de cidades
    if (fscanf(arquivo, "%d", &estrada->N) != 1 || estrada->N < 2 || estrada->N > 10000) {
        fprintf(stderr, "Erro: Número de cidades (N) inválido ou fora do intervalo.\n");
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    // Alocação do vetor de cidades
    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        fprintf(stderr, "Erro: Falha ao alocar memória para o vetor de cidades.\n");
        fclose(arquivo);
        free(estrada);
        return NULL;
    }

    // Leitura das cidades e validação das posições
    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(arquivo, "%d %255s", &estrada->C[i].Posicao, estrada->C[i].Nome) != 2) {
            fprintf(stderr, "Erro: Falha na leitura da cidade %d.\n", i + 1);
            fclose(arquivo);
            free(estrada->C);
            free(estrada);
            return NULL;
        }
        // Verificar restrições das posições (Xi ≠ Xj):
        for (int j = 0; j < i; j++) {
            if (estrada->C[i].Posicao == estrada->C[j].Posicao) {
                fprintf(stderr, "Erro: Duas cidades possuem a mesma posição (%d).\n", estrada->C[i].Posicao);
                fclose(arquivo);
                free(estrada->C);
                free(estrada);
                return NULL;
            }
        }
    }
    fclose(arquivo);

    // Ordenar cidades pela posição
    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);
    return estrada;
}

// Função para calcular a menor vizinhança entre as cidades
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1.0;

    double menorVizinhanca = estrada->T;  // Inicializa com o comprimento máximo

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca;
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
        } else {
            double vizinhancaEsq = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
            double vizinhancaDir = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
            vizinhanca = vizinhancaEsq < vizinhancaDir ? vizinhancaEsq : vizinhancaDir;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);

    return menorVizinhanca;
}

// Função para encontrar o nome da cidade com a menor vizinhança
char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    char *cidadeMenorVizinhanca = NULL;
    double menorVizinhanca = estrada->T;

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca;
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
        } else {
            double vizinhancaEsq = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
            double vizinhancaDir = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
            vizinhanca = vizinhancaEsq < vizinhancaDir ? vizinhancaEsq : vizinhancaDir;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            cidadeMenorVizinhanca = estrada->C[i].Nome;
        }
    }

    // Aloca memória para copiar o nome da cidade com a menor vizinhança
    char *resultado = (char *)malloc(strlen(cidadeMenorVizinhanca) + 1);
    if (resultado) {
        strcpy(resultado, cidadeMenorVizinhanca);
    }

    free(estrada->C);
    free(estrada);

    return resultado;
}
