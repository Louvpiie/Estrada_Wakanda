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

    // Aloca memoria para a estrutura Estrada
    Estrada *estrada = malloc(sizeof(Estrada));
    if (!estrada) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a estrutura Estrada.\n");
        fclose(arquivo);
        return NULL;
    }

    // Le o comprimento da estrada (T) e o número de cidades (N)
    if (fscanf(arquivo, "%d %d", &(estrada->T), &(estrada->N)) != 2) {
        fprintf(stderr, "Erro: Arquivo mal formatado.\n");
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Valida os valores de T
    if (estrada->T < 3 || estrada->T > 1000000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Valida os valores de N
    if (estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Aloca memoria para o vetor de cidades
    estrada->C = malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        fprintf(stderr, "Erro: Falha ao alocar memória para as cidades.\n");
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    // Le os dados das cidades
    for (int i = 0; i < estrada->N; i++) {
        // Verifica se consegue ler os dados corretamente
        if (fscanf(arquivo, "%d ", &(estrada->C[i].Posicao)) != 1 ||
            !fgets(estrada->C[i].Nome, sizeof(estrada->C[i].Nome), arquivo)) {
            fprintf(stderr, "Erro: Falha ao ler os dados da cidade %d.\n", i + 1);
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        // Remove o caractere de nova linha ('\n') do nome, se presente
        char *newline = strchr(estrada->C[i].Nome, '\n');
        if (newline) *newline = '\0';
    }

    fclose(arquivo);

    // Ordena o vetor de cidades pela posição
    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    return estrada;
}

// Função para calcular a menor vizinhança entre as cidades
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1.0;

    double menorVizinhanca = estrada->T; // Inicializa com o comprimento máximo

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
