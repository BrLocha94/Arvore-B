#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>
#include "no_folha.h"
#include "metadados.h"
#include "arvore_b_mais.h"


int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo
	
	FILE *fm = fopen(nome_arquivo_metadados, 'r');
	FILE *fi = fopen(nome_arquivo_indice, 'r');
	FILE *fd = fopen(nome_arquivo_dados, 'r');
	
	TMetadados *metadados = le_metadados(fm);

	if(metadados->raiz_folha == 1){
		fseek(fd, metadados->pont_raiz, SEEK_SET);
		
		TNoFolha *noFolha = le_no_folha(d, fd);

		for(int i = 0; i < noFolha->m; i++){
			if(noFolha->pizzas[i]->cod == cod){
				return(metadados->pont_raiz);
			}
		}
	}
	else{
		/* code */
	}
	

    return INT_MAX;
}

int insere(int cod, char *nome, char *descricao, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo de insercao
    return INT_MAX;
}

int exclui(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo de remocao
    return INT_MAX;
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
    //TODO: Implementar essa funcao
}