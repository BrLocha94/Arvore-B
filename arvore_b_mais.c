#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include "metadados.h"
#include "lista_nos_folhas.h"
#include "arvore_b_mais.h"
#include "lista_pizzas.h"


int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo
	
	FILE *fm = fopen(nome_arquivo_metadados, 'r');
	//FILE *fi = fopen(nome_arquivo_indice, 'r');
	//FILE *fd = fopen(nome_arquivo_dados, 'r');
	
	TMetadados *metadados = le_metadados(fm);

	if(metadados->raiz_folha == 1){
		TListaNosFolhas * nos_folhas = le_nos_folhas(metadados->d, nome_arquivo_dados);

		int i;
		for(i = 0; i < nos_folhas->lista[0]->m; i++){
            
			if(nos_folhas)

		}
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