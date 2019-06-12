#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
 
#include <limits.h>
#include "no_interno.h"
#include "no_folha.h"
#include "metadados.h"
#include "arvore_b_mais.h"


int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//ABRE OS ARQUIVOS
	FILE *fm = fopen(nome_arquivo_metadados, "r");
	FILE *fi = fopen(nome_arquivo_indice, "r");
	FILE *fd = fopen(nome_arquivo_dados, "r");
	
	//RECEBE O METADADOS USANDO O ARQUIVO DE DADOS
	TMetadados *metadados = le_metadados(fm);

	//CHECA SE A RAIZ É FOLHA, CASO SEJA NÃO EXISTE ARQUIVO DE INDICE
	if(metadados->raiz_folha == 1){

		//LEVA O PONTEIRO DO ARQUIVO AO LOCAL EXATO DA LEITURA
		fseek(fd, metadados->pont_raiz, SEEK_SET);
		
		//RECEBE O STRUCT NO_FOLHA COM TODAS AS INFORMAÇÕES QUE DEVEM SER CHECADAS
		TNoFolha *noFolha = le_no_folha(d, fd);
		for(int i = 0; i < noFolha->m; i++){
			
			//CASO A PIZZA PROCURADA SEJA ENCONTRADA, RETORNA O NO QUE A POSSUI
			if(noFolha->pizzas[i]->cod == cod){
				return(metadados->pont_raiz);
			}
		}
		
	}
	//CASO CONTRARIO, DEVE-SE CHECAR PRIMEIRO O ARQUIVO DE INDICES PARA ENTÃO OLHAR OS DADOS
	else{
		
		//PRIMEIRO LEVA-SE O CURSOR DO ARQUIVO DE INDICE AO LOCAL DE INICIO
		//PARA A LEITURA DOS INDICES ATÉ ACHAR A FOLHA, UM LOOP É NECESSARIO
		fseek(fi, metadados->pont_raiz, SEEK_SET);
		int loop = 0;
		//RECEBE O STRUCT DO NO INTERNO COM AS INFORMAÇÕES A SEREM CHECADAS
		TNoInterno * noInterno = le_no_interno(d, fi);
		int seek;

		while(loop == 0){
			
			//CHECA UMA A UMA AS CHAVES DO NO INTERNO 
			for(int i = 0; i < noInterno->m; i++){
				
				//CASO O CODIGO SEJA MENOR, ISSO QUER DIZER QUE DEVEMOS RETORNAR O PONTEIRO DA ESQUERDA
				//DA CHAVE CHECADA
				if(noInterno->chaves[i] > cod){

					//SE O NO INTERNO FOR FOLHA, PREPARAMOS O ARQUIVO DE DADOS E TERMINAMOS O LOOP 
					if(noInterno->aponta_folha == 1){
						fseek(fd, noInterno->p[i], SEEK_SET);
						loop = 1;
						seek = noInterno->p[i];
						printf("\n SEEK %i \n", seek);
						break;
					}
					//CASO CONTRARIO, O ARQUIVO DE INDICE DEVE SER CHECADO NOVAMENTE
					else{
						fseek(fi, noInterno->p[i], SEEK_SET);
						noInterno = le_no_interno(d, fi);
						break;
					}
				}

				//SE CHEGARMOS A ULTIMA CHECAGEM DO FOR, QUER DIZER QUE OLHAMOS TODOS OS PONTEIROS DA ESQUERDA
				//SENDO ASSIM, DEVE-SE PASSAR O ULTIMO PONTEIRO, OU SEJA, O PONTEIRO M(NESSE CASO I+1)
				if(i == (noInterno->m -1)){

					//SE O NO INTERNO FOR FOLHA, PREPARAMOS O ARQUIVO DE DADOS E TERMINAMOS O LOOP 
					if(noInterno->aponta_folha == 1){
						fseek(fd, noInterno->p[m], SEEK_SET);
						loop = 1;
						seek = noInterno->p[m];
						printf("\n SEEK %i \n", seek);
						break;
					}
					//CASO CONTRARIO, O ARQUIVO DE INDICE DEVE SER CHECADO NOVAMENTE
					else{
						fseek(fi, noInterno->p[m], SEEK_SET);
						noInterno = le_no_interno(d, fi);
						break;
					}
				}
			}
		}

		//RECEBE O STRUCT NO_FOLHA COM TODAS AS INFORMAÇÕES QUE DEVEM SER CHECADAS
		TNoFolha *noFolha = le_no_folha(d, fd);
		for(int i = 0; i < noFolha->m; i++){
			
			//CASO A PIZZA PROCURADA SEJA ENCONTRADA, RETORNA O NO QUE A POSSUI
			if(noFolha->pizzas[i]->cod == cod){
				return(seek);
			}
		}
		
	}

	//CASO NÃO SEJA ENCONTRADA A INFORMAÇÃO PROCURADA, RETORNA-SE O INT MAX
	return 0;
}

int insere(int cod, char *nome, char *descricao, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//ABRE OS ARQUIVOS
	FILE *fm = fopen(nome_arquivo_metadados, 'r');
	FILE *fi = fopen(nome_arquivo_indice, 'r');
	FILE *fd = fopen(nome_arquivo_dados, 'r');
	
	//RECEBE O METADADOS USANDO O ARQUIVO DE DADOS
	TMetadados *metadados = le_metadados(fm);
	
	//CHECA SE A RAIZ É FOLHA, CASO SEJA NÃO EXISTE ARQUIVO DE INDICE
	if(metadados->raiz_folha == 1){
	
	
	
	
	
	
	
	}
	else{
	
	
	
	
	
	
	
	}
	
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