#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/sistemas.h"
#include "include/matrizes.h"
#include "include/menu.h"
#include "include/util.h"
#include "include/arquivos.h"

void parseSistema(Sistema *sistema){        // Interpreta as equações em texto e monta a matriz aumentada
    Matriz *matriz = &sistema->matriz;
    matriz->linhas = sistema->qtdLinhas;
    matriz->colunas = sistema->qtdIcog + 1;     // Colunas = incógnitas + termo independente

    char variaveis[10];
    for(int i=0;i<sistema->qtdIcog;i++){        // Define a ordem das variáveis: x, y, z, ...
        variaveis[i] = 'x' + i;
    }

    for(int i=0;i<sistema->qtdLinhas;i++){
        for(int j=0;j<matriz->colunas;j++){     // Zera a linha antes de preencher
            matriz->matriz[i][j] = 0;
        }

        char *equacao = sistema->equacoes[i];
        int sinal = 1;
        int lado = 1;       // 1 = antes do '=', -1 = depois do '='
        int j = 0;
        while(equacao[j] != '\0'){
            if(equacao[j] == '+'){
                sinal = 1;
                j++;
            } else if(equacao[j] == '-'){
                sinal = -1;
                j++;
            } else if(equacao[j] == '='){
                lado = -1;
                sinal = 1;
                j++;
            } else {
                double numero = 1;      // Coeficiente do termo (1 quando não vem número escrito)
                if((equacao[j] >= '0' && equacao[j] <= '9') || equacao[j] == '.'){
                    numero = atof(&equacao[j]);
                    while((equacao[j] >= '0' && equacao[j] <= '9') || equacao[j] == '.'){
                        j++;
                    }
                }
                if(equacao[j] >= 'a' && equacao[j] <= 'z'){      // Termo com variável
                    int coluna = 0;
                    while(variaveis[coluna] != equacao[j]){
                        coluna++;
                    }
                    matriz->matriz[i][coluna] += lado * sinal * numero;
                    j++;
                } else {        // Termo constante: vai para a última coluna, trocando de lado
                    matriz->matriz[i][sistema->qtdIcog] -= lado * sinal * numero;
                }
            }
        }
    }
}

void lerSistema(){
    Sistema sistema;
    printf("\tQuantas linhas tem o sistema?\n  -> ");
    scanf("%d", &sistema.qtdLinhas);
    printf("\tQuantas icógnitas tem o sistema?\n  -> ");
    scanf("%d", &sistema.qtdIcog);
    limparTela();
    getchar();
    escreveTitulo(tituloPrinc, "- LEITURA DE SISTEMA");
    printf("\tEscreva um sistema com %d equações e %d icógnitas:\n", sistema.qtdLinhas, sistema.qtdIcog);
    escreverLinha(divisa);
    for(int i=0;i<sistema.qtdLinhas;i++){
        printf("\t|  ");
        fgets(sistema.equacoes[i], sizeof(sistema.equacoes[i]), stdin);
        sistema.equacoes[i][strcspn(sistema.equacoes[i], "\n")] = '\0';
    }
    limpaEspacos(sistema.equacoes, sistema.qtdLinhas);
    gravaSistema(&sistema);

    parseSistema(&sistema);     // Monta a matriz aumentada
    printf("\n\tMatriz aumentada do sistema:\n");
    escreverLinha(divisa);
    imprimirMatriz(&sistema.matriz);
    escreverLinha(divisa);
}
