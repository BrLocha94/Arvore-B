#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
 
#include <limits.h>
#include "no_interno.h"
#include "no_folha.h"
#include "pizza.h"
#include "metadados.h"
#include "arvore_b_mais.h"


int busca(int cod, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	//ABRE O ARQUIVO DE INDICE
	FILE * fi = fopen(nome_arquivo_indice, "rb");
	
	//RECEBE O METADADOS
	TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);

	//CHECA SE A RAIZ É FOLHA, CASO SEJA NÃO EXISTE ARQUIVO DE INDICE, LOGO, DEVE-SE RETORNAR O PONTEIRO DA RAIZ
	if(metadados->raiz_folha == 1){
		return metadados->pont_raiz;
	}
	//CASO CONTRARIO, DEVE-SE PROCURAR NO ARQUIVO DE INDICE O DADO
	else{
		
		//PRIMEIRO LEVA-SE O CURSOR DO ARQUIVO DE INDICE AO LOCAL DE INICIO
		//PARA A LEITURA DOS INDICES ATÉ ACHAR A FOLHA, UM LOOP É NECESSARIO
		fseek(fi, metadados->pont_raiz, SEEK_SET);
		int loop = 0;
		
		//RECEBE O STRUCT DO NO INTERNO COM AS INFORMAÇÕES A SEREM CHECADAS
		TNoInterno * noInterno = le_no_interno(d, fi);

		while(loop == 0){
			
			//CHECA UMA A UMA AS CHAVES DO NO INTERNO 
			for(int i = 0; i < noInterno->m; i++){
				
				//CASO O CODIGO SEJA MENOR, ISSO QUER DIZER QUE DEVEMOS RETORNAR O PONTEIRO DA ESQUERDA
				//DA CHAVE CHECADA
				if(noInterno->chaves[i] > cod){

					//SE O NO INTERNO FOR FOLHA, PREPARAMOS O ARQUIVO DE DADOS E TERMINAMOS O LOOP 
					if(noInterno->aponta_folha == 1){
						return noInterno->p[i];
					}
					//CASO CONTRARIO, O ARQUIVO DE INDICE DEVE SER CHECADO NOVAMENTE
					else{
						fseek(fi, noInterno->p[i], SEEK_SET);
						noInterno = le_no_interno(d, fi);
						break;
					}
				}

				//SE CHEGARMOS A ULTIMA CHECAGEM DO FOR, QUER DIZER QUE OLHAMOS TODOS OS PONTEIROS DA ESQUERDA
				//SENDO ASSIM, DEVE-SE PASSAR O ULTIMO PONTEIRO, OU SEJA, O PONTEIRO M
				if(i == (noInterno->m -1)){

					//SE O NO INTERNO FOR FOLHA, PREPARAMOS O ARQUIVO DE DADOS E TERMINAMOS O LOOP 
					if(noInterno->aponta_folha == 1){
						return noInterno->p[noInterno->m];
					}
					//CASO CONTRARIO, O ARQUIVO DE INDICE DEVE SER CHECADO NOVAMENTE
					else{
						fseek(fi, noInterno->p[noInterno->m], SEEK_SET);
						noInterno = le_no_interno(d, fi);
						break;
					}
				}
			}
		}
	}

	//CASO NÃO SEJA ENCONTRADA A INFORMAÇÃO PROCURADA, RETORNA-SE O INT MAX
	return INT_MAX;
}


int insere(int cod, char *nome, char *descricao, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	
//chamar a busca e pega o que ela retorna, se o nó estiver inserido erro na inserção, senão inserção válida !!!
//fseek na posição retornada pela busca e verificação se é válida ou não a inserção
//fseek de novo para salvar o novo item
	//ABRE O ARQUIVO DE INDICE
	FILE * fi = fopen(nome_arquivo_indice, "rb+");
	FILE * fd = fopen(nome_arquivo_dados, "rb+");
	    
	TPizza *newPizza = pizza(cod, nome, descricao, preco);

	int buscaNo = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

	fseek(fd, buscaNo, SEEK_SET);

	TNoFolha *noFolha = le_no_folha(d, fd);

	if(noFolha->m < (2 * d)){
		TPizza *aux = newPizza;
		TPizza *aux_2 = NULL;

		for(int i = 0; i < noFolha->m; i++){
			
			if(noFolha->pizzas[i]->cod == cod){
				return -1;
			}
			
			if(noFolha->pizzas[i]->cod > aux->cod){
				aux_2 = noFolha->pizzas[i];
				noFolha->pizzas[i] = aux;
				aux = aux_2;
			}
		}
		noFolha->pizzas[noFolha->m] = aux;
		noFolha->m++;

		fseek(fd, buscaNo, SEEK_SET);
		salva_no_folha(d, noFolha, fd);
		
		return buscaNo;
	}
	
	/* 
	//RECEBE O METADADOS
	TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
	    
	//CHECA SE A RAIZ É FOLHA, CASO SEJA NÃO EXISTE ARQUIVO DE INDICE
	if(metadados->raiz_folha == 1){
		
		//USA O SEEK PARA SETAR O ARQUIVO
		fseek(fd, metadados->pont_raiz, SEEK_SET);
		//RECEBE O STRUCT DO NO INTERNO COM AS INFORMAÇÕES A SEREM CHECADAS
		TNoFolha * noFolha = le_no_folha(d, fd);
		
		//CASO TENHA UM ESPAÇO VAZIO NO NÓ PARA INSERÇÃO, ADICIONA A PIZZA NO LUGAR CORRETO E AUMENTA O N DE CHAVES(M)
		if(noFolha->m < (2 * d)){
			
			//CRIA PONTEIROS PARA AUXILIAREM AS TROCAS
			TPizza * aux = newPizza;
			TPizza * aux_2 = NULL;
			
			//PERCORRE A LISTA ORIGINAL DE PIZZAS
			for(int i = 0; i < noFolha->m; i++){
				
				//CASO O NO JÁ EXISTA, RETORNAR ERRO (-1)
				if(noFolha->pizzas[i]->cod == cod) return -1;
				
				//REALIZA UM SORT DE MODO QUE A NOVA PIZZA SEJA INSERIDA NO LOCAL CORRETO (N-1 < N < N+1)
				if(noFolha->pizzas[i]->cod > aux->cod){
					aux_2 = noFolha->pizzas[i];
					noFolha->pizzas[i] = aux;
					aux = aux_2;
				}
			}
			//ADICIONA A PIZZA DE MAIOR CÓDIGO POR ULTIMO
			noFolha->pizzas[noFolha->m] = aux;
			noFolha->m++;
			
			//USA O SEEK PARA SETAR O ARQUIVO, SALVA, E RETORNA O PONTEIRO PARA A PIZZA
			fseek(fd, metadados->pont_raiz, SEEK_SET);
			salva_no_folha(d, noFolha, fd);
			
			//PARA SE CHEGAR NA PIZZA DEVE-SE IR ATÉ O INICIO DO NÓ (METADADOS->PONT_RAIZ NO CASO), E (+)
			//     ANDAR ATÉ A PIZZA DESEJADA (TAMANHO_NO_FOLHA)
			return (metadados->pont_raiz);
		}
		//CASO NÃO TENHA ESPAÇO VAZIO PARA INSERIR O NÓ, DEVE-SE ALTERAR A ESTRUTURA DA ARVORE
		else{
			
		
		}
	}
	else{
	
	
	
	
	
	
	
	}*/
	
    //CASO NÃO SEJA ENCONTRADA A INFORMAÇÃO PROCURADA, RETORNA-SE O INT MAX
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