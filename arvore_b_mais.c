#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
 
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "no_interno.h"
#include "no_folha.h"

#include "lista_nos_folhas.h"

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
		
		int trocou_folha = 0;
		
		//ADICIONAR AS OUTRAS PIZZAS NO novo_noFolha
		for(int i = d; i < (2*d + 1); i++){
			
			if(noFolha->pizzas[i]->cod == cod) trocou_folha = 1;
			
			novo_noFolha->pizzas[i - d] = pizza(noFolha->pizzas[i]->cod, noFolha->pizzas[i]->nome, noFolha->pizzas[i]->categoria, noFolha->pizzas[i]->preco);
			noFolha->pizzas[i] = NULL;
		}
		
		noFolha->m = d;
		novo_noFolha->m = d + 1;
		
		int ret;
			
		if(trocou_folha == 0){
			ret = buscaNo;
		}
		else{
			ret = noFolha->pont_prox;
		}
		
		//ABRE O ARQUIVO DE METADADOS PARA ATUALIZAR AS REFERENCIAS
		TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
		//printf("\n METADADOS ANTES \n");
		//imprime_metadados(metadados);
		
		if(metadados->raiz_folha == 0){
			
			//MODIFICAR O ARQUIVO DE INDICE PARA TER UM PONTEIRO AO novo_noFolha
			//   lembrete: O NOVO NO FOLHA FICARA NECESSÁRIAMENTE APOS O ANTIGO NO ARQUIVO
			//CASO O ARQUIVO DE INDICE TENHA ESPAÇO PARA INSERÇÃO
			fseek(fi, noFolha->pont_pai, SEEK_SET);
			TNoInterno * noInterno = le_no_interno(d, fi);
			int chave = novo_noFolha->pizzas[0]->cod;
			int aux_chave = chave;
			
			if(noInterno->m < 2*d){
				
				//printf("\n NO INTERNO ANTES : \n");
				//imprime_no_interno(d, noInterno);
				
				int flag = 0;
				int aux_pont; int aux_pont_02;
				
				for(int i = 0; i < noInterno->m; i++){
					
					if(noInterno->chaves[i] > chave){
						
						if(flag == 0){
							aux_pont = noInterno->p[i + 1];
							noInterno->p[i + 1] = -2;
							flag = 1;
						}
						else{
							aux_pont_02 = noInterno->p[i + 1];
							noInterno->p[i + 1] = aux_pont;
							aux_pont = aux_pont_02;
						}
						aux_chave = noInterno->chaves[i];
						noInterno->chaves[i] = chave;
						chave = aux_chave;
						
					}
				}
				
				noInterno->chaves[noInterno->m] = chave;
				noInterno->m ++;
				
				if(flag == 0){
					noInterno->p[noInterno->m] = -2; 
				}
				else{
					noInterno->p[noInterno->m] = aux_pont;
				}
				
				//printf("\n NO INTERNO DEPOIS : \n");
				//imprime_no_interno(d, noInterno);
			
				int pont_novo;
				
				pont_novo = metadados->pont_prox_no_folha_livre;
				
				novo_noFolha->pont_pai = noFolha->pont_pai;
				novo_noFolha->pont_prox = noFolha->pont_prox;
				noFolha->pont_prox = pont_novo;
				
				//printf("\n NO FOLHA : \n");
				//imprime_no_folha(d, noFolha);
				
				//printf("\n NOVO NO FOLHA : \n");
				//imprime_no_folha(d, novo_noFolha);
				
				//SALVAR ARQUIVO DE DADOS
				fseek(fd, buscaNo, SEEK_SET);
				salva_no_folha(d, noFolha, fd);
				
				fseek(fd, pont_novo, SEEK_SET);
				salva_no_folha(d, novo_noFolha, fd);
				
				//FECHA ARQUIVO DE DADOS
				fclose(fd);
				
				if(flag == 0){
					noInterno->p[noInterno->m] = pont_novo; 
				}
				else{
					for(int i = 0; i < noInterno->m; i++){
				
						if(noInterno->p[i] == -2){
							noInterno->p[i] = pont_novo;
							break;
						}
					}
				}
				
				//printf("\n NO INTERNO ANTES DE SALVAR : \n");
				//imprime_no_interno(d, noInterno);
				
				//SALVAR ARQUIVO DE INDICE
				fseek(fi, noFolha->pont_pai, SEEK_SET);
				salva_no_interno(d, noInterno, fi);
				//FECHAR ARQUIVO DE INDICE
				fclose(fi);
				
				//SALVAR ARQUIVO METADADOS
				metadados->pont_prox_no_folha_livre = noFolha->pont_prox + tamanho_no_folha(d);
				
				//printf("\n METADADOS SALVOS \n");
				//imprime_metadados(metadados);
				
				salva_arq_metadados(nome_arquivo_metadados, metadados);
				
				//printf("\n RET : %i \n", ret);
				
				free(noFolha);
				free(novo_noFolha);
				
				//TListaNosFolhas * lista_folhas = le_nos_folhas(d, nome_arquivo_dados);
				//imprime_nos_folhas(d, lista_folhas);
				
				return ret;
				
			}
			//CASO CONTRARIO, FAZER O PARTICIONAMENTO TOMANDO CUIDADO COM A PROPAGAÇÃO
			else{
				
				int pont_novo;
				
				pont_novo = metadados->pont_prox_no_folha_livre;
				
				novo_noFolha->pont_pai = noFolha->pont_pai;
				novo_noFolha->pont_prox = noFolha->pont_prox;
				noFolha->pont_prox = pont_novo;
				
				//SALVAR ARQUIVO DE DADOS
				fseek(fd, buscaNo, SEEK_SET);
				salva_no_folha(d, noFolha, fd);
				
				fseek(fd, pont_novo, SEEK_SET);
				salva_no_folha(d, novo_noFolha, fd);
				
				int pont_pai_01 = noFolha->pont_pai;
				int pont_pai_02 = metadados->pont_prox_no_interno_livre;
				
				free(noFolha);
				free(novo_noFolha);
				
				int loop = 0;
				
				while(loop == 0){
					
					printf("\n NO INTERNO ANTES : \n");
					imprime_no_interno(d, noInterno);
					
					//PRIMEIRO ORDENAR O NÓ QUE JÁ TEMOS
					
					int flag = 0;                    //chave =  nova chave adicionada = aux_chave
					int aux_pont; int aux_pont_02;
					
					for(int i = 0; i < noInterno->m; i++){
						
						if(noInterno->chaves[i] > chave){
							
							if(flag == 0){
								aux_pont = noInterno->p[i + 1];
								noInterno->p[i + 1] = -2;
								flag = 1;
							}
							else{
								aux_pont_02 = noInterno->p[i + 1];
								noInterno->p[i + 1] = aux_pont;
								aux_pont = aux_pont_02;
							}
							aux_chave = noInterno->chaves[i];
							noInterno->chaves[i] = chave;
							chave = aux_chave;
							
						}
					}
					
					noInterno->chaves[noInterno->m] = chave;
					noInterno->m ++;
					
					if(flag == 0){
						noInterno->p[noInterno->m] = -2; 
					}
					else{
						noInterno->p[noInterno->m] = aux_pont;
					}
					
					//ACERTAR O PONTEIRO PARA, CASO SEJA FOLHA, RECEBER A POSIÇÃO CORRETA
					//E CASO SEJA NÓ INTERNO, RECEBER O PONTEIRO PARA O ULTIMO ADICIONADO
					if(flag == 0){
						if(noInterno->aponta_folha == 1){
							noInterno->p[noInterno->m] = metadados->pont_prox_no_folha_livre; 
						}
						else{
							noInterno->p[noInterno->m] = metadados->pont_prox_no_interno_livre;
						}
					}
					else{
						for(int i = 0; i < noInterno->m; i++){
					
							if(noInterno->p[i] == -2){
								if(noInterno->aponta_folha == 1){
										noInterno->p[i] = metadados->pont_prox_no_folha_livre; 
								}
								else{
									noInterno->p[i] = metadados->pont_prox_no_interno_livre;
								}
								break;
							}
						}
					}	
					
					printf("\n NO INTERNO ANTES DE PARTICIONAR : \n");
					imprime_no_interno(d, noInterno);
					
					//DEPOIS, CRIAR O NOVO NÓ E ADICIONA AS CHAVES CORRESPONDENTES PARA ELE
					TNoInterno * novo_noInterno = no_interno_vazio(d);
					
					//ADICIONAR OS PONTEIROS E CHAVES NO NOVO NO INTERNO
					for(int i = d; i < (2*d + 1); i++){
						
						novo_noInterno->p[i - d] = noInterno->p[i];
						noInterno->p[i] = -1;
						novo_noInterno->chaves[i - d] = noInterno->chaves[i];
						noInterno->chaves[i] = -1;				
					}
					
					noInterno->m = d;
					novo_noInterno->m = d + 1;
					
					if(noInterno->aponta_folha == 1){
						
						novo_noInterno->aponta_folha = 1;
						
						for(int i = 0; i < (noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fd, noInterno->p[i], SEEK_SET);
							//LE NO FOLHA DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							TNoFolha * noFolha = le_no_folha(d, fd);
							noFolha->pont_pai = pont_pai_01;
							salva_no_folha(d, noFolha, fd);
						}
						
						for(int i = 0; i < (novo_noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fd, noInterno->p[i], SEEK_SET);
							//LE NO FOLHA DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							TNoFolha * noFolha = le_no_folha(d, fd);
							noFolha->pont_pai = pont_pai_02;
							salva_no_folha(d, noFolha, fd);
						}
					}
					else{
						
						novo_noInterno->aponta_folha = 0;
						
						for(int i = 0; i < (noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fi, noInterno->p[i], SEEK_SET);
							//LE NO INTERNO DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							TNoInterno * noInterno_01 = le_no_interno(d, fi);
							noInterno_01->pont_pai = pont_pai_01;
							salva_no_interno(d, noInterno_01, fi);
							free(noInterno_01);
						}
						
						for(int i = 0; i < (novo_noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fi, noInterno->p[i], SEEK_SET);
							//LE NO INTERNO DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							TNoInterno * noInterno_01 = le_no_interno(d, fi);
							noInterno_01->pont_pai = pont_pai_02;
							salva_no_interno(d, noInterno_01, fi);
							free(noInterno_01);
						}
					}
					
					printf("\n NO INTERNO DEPOIS : \n");
					imprime_no_interno(d, noInterno);
					
					printf("\n NOVO NO INTERNO : \n");
					imprime_no_interno(d, noInterno);
					
					//SALVAR NOS NO ARQUIVO
					//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
					fseek(fi, pont_pai_01, SEEK_SET);
					salva_no_interno(d, noInterno, fi);
					
					//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
					fseek(fi, pont_pai_02, SEEK_SET);
					salva_no_interno(d, novo_noInterno, fi);
					
					//AVANÇA O PONTEIRO DO METADADOS DE INDICE
					metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
					
					//PROPAGAR A CHAVE DO NOVO NO INTERNO PARA O PAI
					chave = novo_noInterno->chaves[0];
					pont_pai_01 = noInterno->pont_pai;
					free(noInterno);
					free(novo_noInterno);
					
					//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
					fseek(fi, pont_pai_01, SEEK_SET);
					noInterno = le_no_interno(d, fi);
					
					if(noInterno != NULL){
					
						if(noInterno->m < 2*d){
							
							loop = 1;
							
							//PRIMEIRO ORDENAR O NÓ QUE JÁ TEMOS
						
							int flag = 0;                    //chave =  nova chave adicionada = aux_chave
							int aux_pont; int aux_pont_02;
							
							for(int i = 0; i < noInterno->m; i++){
								
								if(noInterno->chaves[i] > chave){
									
									if(flag == 0){
										aux_pont = noInterno->p[i + 1];
										noInterno->p[i + 1] = -2;
										flag = 1;
									}
									else{
										aux_pont_02 = noInterno->p[i + 1];
										noInterno->p[i + 1] = aux_pont;
										aux_pont = aux_pont_02;
									}
									aux_chave = noInterno->chaves[i];
									noInterno->chaves[i] = chave;
									chave = aux_chave;
									
								}
							}
							
							noInterno->chaves[noInterno->m] = chave;
							noInterno->m ++;
							
							if(flag == 0){
								noInterno->p[noInterno->m] = -2; 
							}
							else{
								noInterno->p[noInterno->m] = aux_pont;
							}
							
							//ACERTAR O PONTEIRO PARA, CASO SEJA FOLHA, RECEBER A POSIÇÃO CORRETA
							//E CASO SEJA NÓ INTERNO, RECEBER O PONTEIRO PARA O ULTIMO ADICIONADO
							if(flag == 0){
								noInterno->p[noInterno->m] = metadados->pont_prox_no_interno_livre;
							}
							else{
								for(int i = 0; i < noInterno->m; i++){
							
									if(noInterno->p[i] == -2){
										noInterno->p[i] = metadados->pont_prox_no_interno_livre;
										break;
									}
								}
							}
						}
						
					}
					else{
						
						loop = 1;
						
						noInterno = no_interno_vazio(d);
						noInterno->p[0] = pont_pai_01;
						noInterno->p[1] = pont_pai_02;
						noInterno->chaves[0] = chave;
						
						//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
						fseek(fi, metadados->pont_prox_no_interno_livre, SEEK_SET);
						salva_no_interno(d, novo_noInterno, fi);
						
						metadados->pont_raiz = metadados->pont_prox_no_interno_livre;
						metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
					}
					
					printf("\n METADADOS FINAL DO LOOP \n");
					imprime_metadados(metadados);
				}
				
				//SALVAR ARQUIVO METADADOS
				metadados->pont_prox_no_folha_livre = metadados->pont_prox_no_folha_livre + tamanho_no_folha(d);
				
				printf("\n METADADOS SALVOS \n");
				imprime_metadados(metadados);
				
				fclose(fd);
				fclose(fi);
				
				salva_arq_metadados(nome_arquivo_metadados, metadados);
				free(metadados);
				
				return ret;
			}
		}
		else{
			
			TNoInterno * noInterno = no_interno_vazio(d);
			noInterno->p[0] = buscaNo;
			noInterno->p[1] = metadados->pont_prox_no_folha_livre;
			noInterno->chaves[0] = novo_noFolha->pizzas[0]->cod;
			noInterno->m = 1;
			noInterno->aponta_folha = 1;
			
			fseek(fi, 0, SEEK_SET);
			salva_no_interno(d, noInterno, fi);
			free(noInterno);
			
			fclose(fi);
			
			//SALVAR ARQUIVO DE DADOS
			noFolha->pont_prox = metadados->pont_prox_no_folha_livre;
			noFolha->pont_pai = 0;
			
			fseek(fd, buscaNo, SEEK_SET);
			salva_no_folha(d, noFolha, fd);
			
			novo_noFolha->pont_pai = 0;
			
			fseek(fd, metadados->pont_prox_no_folha_livre, SEEK_SET);
			salva_no_folha(d, novo_noFolha, fd);
			
			fclose(fd);
			
			free(noFolha);
			free(novo_noFolha);
			
			metadados->pont_raiz = 0;
			metadados->raiz_folha = 0;
			metadados->pont_prox_no_interno_livre = tamanho_no_interno(d);
			metadados->pont_prox_no_folha_livre = metadados->pont_prox_no_folha_livre + tamanho_no_folha(d);
			
			salva_arq_metadados(nome_arquivo_metadados, metadados);
			free(metadados);
			
			return ret;
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