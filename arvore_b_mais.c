#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
 
#include <limits.h>
#include <string.h>
#include <stdlib.h>
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


int insere(int cod, char *nome, char *categoria, float preco, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados, int d)
{
	
//chamar a busca e pega o que ela retorna, se o nó estiver inserido erro na inserção, senão inserção válida !!!
//fseek na posição retornada pela busca e verificação se é válida ou não a inserção
//fseek de novo para salvar o novo item
	//ABRE O ARQUIVO DE INDICE
	FILE * fi = fopen(nome_arquivo_indice, "rb+");
	FILE * fd = fopen(nome_arquivo_dados, "rb+");
	    
	//TPizza *newPizza = pizza(cod, nome, categoria, preco);

	int buscaNo = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

	fseek(fd, buscaNo, SEEK_SET);

	TNoFolha *noFolha = le_no_folha(d, fd);

	if(noFolha->m < (2 * d)){
		
		//TNoFolha *noFolha = le_no_folha(d, fd);
		
		TPizza *aux = pizza(cod, nome, categoria, preco);
		
		if(noFolha->m != 0){
		
			TPizza *aux_2 = pizza(cod, nome, categoria, preco);

			for(int i = 0; i < noFolha->m; i++){
				
				//CASO O NÓ JÁ EXISTA NA ARVORE
				if(noFolha->pizzas[i]->cod == cod){
					return -1;
				}
				
				//CASO CONTRÁRIO, INSERE NO DEVIDO LOCAL
				if(noFolha->pizzas[i]->cod > aux->cod){
					
					//TROCA O VALOR DO AUX PELO DA PIZZA MAIOR
					aux->cod = noFolha->pizzas[i]->cod;
					strcpy(aux->nome, noFolha->pizzas[i]->nome);
					strcpy(aux->categoria, noFolha->pizzas[i]->categoria);
					aux->preco = noFolha->pizzas[i]->preco;
					
					//TROCA O VALOR DA PIZZA PELA QUE DEVE SER INSERIDA
					noFolha->pizzas[i]->cod = aux_2->cod;
					strcpy(noFolha->pizzas[i]->nome, aux_2->nome);
					strcpy(noFolha->pizzas[i]->categoria, aux_2->categoria);
					noFolha->pizzas[i]->preco = aux_2->preco;
					
					//ACERTA O AUX 2 
					aux_2->cod = aux->cod;
					strcpy(aux_2->nome, aux->nome);
					strcpy(aux_2->nome, aux->nome);
					aux_2->preco = aux->preco;
				}
			}
			
			free(aux_2);
		}
		
		noFolha->pizzas[noFolha->m] = aux;
		noFolha->m++;
		
		TNoFolha * noFolha_aux = no_folha(d, noFolha->m, noFolha->pont_pai, noFolha->pont_prox);
		for(int i = 0; i < noFolha->m; i++){
			
			noFolha_aux->pizzas[i] = pizza(noFolha->pizzas[i]->cod, noFolha->pizzas[i]->nome, noFolha->pizzas[i]->categoria, noFolha->pizzas[i]->preco);
		}
		
		fseek(fd, buscaNo, SEEK_SET);
		salva_no_folha(d, noFolha_aux, fd);
		fclose(fd);
		
		free(aux);
		free(noFolha);
		free(noFolha_aux);
		
		return buscaNo;
	}
	else{
		
		//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
		fseek(fi, noFolha->pont_pai, SEEK_SET);
		
		TPizza *aux = pizza(cod, nome, categoria, preco);
		
		//CRIAR NOVO NÓ FOLHA VAZIO
		TNoFolha * novo_noFolha = no_folha_vazio(d);
		int troca = 0;
		
		//ADICIONAR AS PRIMEIRAS D PIZZAS(ORDENADAS POR CODIGO) NO noFolha
		TPizza *aux_2 = pizza(cod, nome, categoria, preco);

		for(int i = 0; i < noFolha->m; i++){
			
			//CASO O NÓ JÁ EXISTA NA ARVORE
			if(noFolha->pizzas[i]->cod == cod){
				return -1;
			}
			
			//CASO CONTRÁRIO, INSERE NO DEVIDO LOCAL
			if(noFolha->pizzas[i]->cod > aux->cod){
				
				//TROCA O VALOR DO AUX PELO DA PIZZA MAIOR
				aux->cod = noFolha->pizzas[i]->cod;
				strcpy(aux->nome, noFolha->pizzas[i]->nome);
				strcpy(aux->categoria, noFolha->pizzas[i]->categoria);
				aux->preco = noFolha->pizzas[i]->preco;
				
				//TROCA O VALOR DA PIZZA PELA QUE DEVE SER INSERIDA
				noFolha->pizzas[i]->cod = aux_2->cod;
				strcpy(noFolha->pizzas[i]->nome, aux_2->nome);
				strcpy(noFolha->pizzas[i]->categoria, aux_2->categoria);
				noFolha->pizzas[i]->preco = aux_2->preco;
				
				//ACERTA O AUX 2 
				aux_2->cod = aux->cod;
				strcpy(aux_2->nome, aux->nome);
				strcpy(aux_2->nome, aux->nome);
				aux_2->preco = aux->preco;
			}
		}
		
		free(aux_2);
		
		noFolha->pizzas[noFolha->m] = aux;
		noFolha->m++;
		
		//ADICIONAR AS OUTRAS PIZZAS NO novo_noFolha
		for(int i = d; i < (2*d + 1); i++){
			
			novo_noFolha->pizzas[i - d] = pizza(noFolha->pizzas[i]->cod, noFolha->pizzas[i]->nome, noFolha->pizzas[i]->categoria, noFolha->pizzas[i]->preco);
			noFolha->pizzas[i] = NULL;
		}
		
		noFolha->m = d;
		novo_noFolha->m = d + 1;
		
		//MODIFICAR O ARQUIVO DE INDICE PARA TER UM PONTEIRO AO novo_noFolha
		//   lembrete: O NOVO NO FOLHA FICARA NECESSÁRIAMENTE APOS O ANTIGO NO ARQUIVO
		//CASO O ARQUIVO DE INDICE TENHA ESPAÇO PARA INSERÇÃO
		TNoInterno * noInterno = le_no_interno(d, fi);
		int chave = novo_noFolha->pizzas[0]->cod;
		int aux_chave = chave;
		
		if(noInterno->m < 2*d){
			
			printf("\n NO INTERNO ANTES : \n");
			imprime_no_interno(d, noInterno);
			
			for(int i = 0; i < noInterno->m; i++){
				
				if(noInterno->chaves[i] > chave){
					
					aux_chave = noInterno->chaves[i];
					noInterno->chaves[i] = chave;
					chave = aux_chave;
				}
			}
			
			noInterno->chaves[noInterno->m] = chave;
			noInterno->m ++;
			noInterno->p[noInterno->m] = noInterno->p[noInterno->m] + tamanho_no_folha(d); 
			
			printf("\n NO INTERNO DEPOIS : \n");
			imprime_no_interno(d, noInterno);
			
			//SALVAR ARQUIVO DE INDICE
			fseek(fi, noFolha->pont_pai, SEEK_SET);
			salva_no_interno(d, noInterno, fi);
			//FECHAR ARQUIVO DE INDICE
			fclose(fi);
			
			
			novo_noFolha->pont_pai = noFolha->pont_pai;
			novo_noFolha->pont_prox = noFolha->pont_prox + tamanho_no_folha(d);
			
			//SALVAR ARQUIVO DE DADOS
			fseek(fd, buscaNo, SEEK_SET);
			
			printf("\n NO FOLHA INSERIDO PRIMEIRO: \n");
			imprime_no_folha(d, noFolha);
			
			salva_no_folha(d, noFolha, fd);
			
			fseek(fd, noFolha->pont_prox, SEEK_SET);
			TNoFolha * aux_folha = le_no_folha(d, fd);
			
			fseek(fd, noFolha->pont_prox, SEEK_SET);
			
			printf("\n NO FOLHA INSERIDO SEGUNDO: \n");
			imprime_no_folha(d, novo_noFolha);
			
			salva_no_folha(d, novo_noFolha, fd);
			
			TNoFolha * aux_folha_02; int loop = 0;
			
			printf("\n RESULTADO DO FSEEK DO WHILE : %i \n", fseek(fd, noFolha->pont_prox + (loop * tamanho_no_folha(d)), SEEK_SET));
			
			while(fseek(fd, noFolha->pont_prox + (loop * tamanho_no_folha(d)), SEEK_SET) == 0){
				
				printf("\n NO FOLHA INSERIDO : \n");
				imprime_no_folha(d, aux_folha);
				
				aux_folha_02 = le_no_folha(d, fd);
				fseek(fd, noFolha->pont_prox + (loop * tamanho_no_folha(d)), SEEK_SET);
				salva_no_folha(d, aux_folha, fd);
				
				printf("\n AUX_02 : \n");
				imprime_no_folha(d, aux_folha_02);
				
				free(aux_folha);
				aux_folha = aux_folha_02;
				
				loop ++;
			}
			
			printf("\n NO FOLHA INSERIDO FINAL : \n");
			imprime_no_folha(d, aux_folha);
			
			salva_no_folha(d, aux_folha, fd);
			
			//FECHAR ARQUIVO DE DADOS
			fclose(fd);
			
			int ret = noFolha->pont_prox;
			
			free(aux_folha);
			free(noFolha);
			free(novo_noFolha);
			
			return ret;
		}
		//CASO CONTRARIO, FAZER O PARTICIONAMENTO TOMANDO CUIDADO COM A PROPAGAÇÃO
		else{
		
		
		
		}
		
	}
	
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