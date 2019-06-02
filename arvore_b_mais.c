#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <limits.h>

#include "arvore_b_mais.h"
#include "lista_pizzas.h"


int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//TODO: Inserir aqui o codigo do algoritmo
	
	//VAI OLHAR O ARQUIVO DE METADADOS
	//CASO ELE SEJA VAZIO, RETORNA FALSE
	
	//CASO ELE SEJA FOLHA (NÃO TEM PONTEIROS PARA O PROXIMO INDICE), LE O ARQUIVO DADOS E PROCURA
    //SE ENCONTRAR RETORNA TRUE

	//CASO POSSUA INDICE, VAI PROCURAR NO ARQUIVO INDICE O NUMERO OU O INTERVALO NO QUAL ELE SE ENCONTRA
	//A PARTIR DISSO, OLHA NO ARQUIVO DE DADOS E PROCURA
	//CASO ACHE, RETORNA TRUE
	//SE CHEGAR NO FINAL DO ARQUIVO DE DADOS, RETORNA FALSE
	
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