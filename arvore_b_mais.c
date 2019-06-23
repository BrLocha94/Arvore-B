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
	
	int buscaNo = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);

	fseek(fd, buscaNo, SEEK_SET);

	TNoFolha *noFolha = le_no_folha(d, fd);

	if(noFolha->m < (2 * d)){
		
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
					strcpy(aux_2->categoria, aux->categoria);
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
				strcpy(aux_2->categoria, aux->categoria);
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
		
		//ABRE O ARQUIVO DE METADADOS PARA ATUALIZAR AS REFERENCIAS
		TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
		
		int ret;
			
		if(trocou_folha == 0){
			ret = buscaNo;
		}
		else{
			ret = metadados->pont_prox_no_folha_livre;
		}
		
		if(metadados->raiz_folha == 0){
			
			//MODIFICAR O ARQUIVO DE INDICE PARA TER UM PONTEIRO AO novo_noFolha
			//   lembrete: O NOVO NO FOLHA FICARA NECESSÁRIAMENTE APOS O ANTIGO NO ARQUIVO
			//CASO O ARQUIVO DE INDICE TENHA ESPAÇO PARA INSERÇÃO
			fseek(fi, noFolha->pont_pai, SEEK_SET);
			TNoInterno * noInterno = le_no_interno(d, fi);
			int chave = novo_noFolha->pizzas[0]->cod;
			int aux_chave = chave;
			
			if(noInterno->m < 2*d){
				
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
				
				//SALVAR ARQUIVO DE INDICE
				fseek(fi, noFolha->pont_pai, SEEK_SET);
				salva_no_interno(d, noInterno, fi);
				//FECHAR ARQUIVO DE INDICE
				fclose(fi);
				
				//SALVAR ARQUIVO METADADOS
				metadados->pont_prox_no_folha_livre = noFolha->pont_prox + tamanho_no_folha(d);
				
				salva_arq_metadados(nome_arquivo_metadados, metadados);
				
				free(noFolha);
				free(novo_noFolha);
				
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

				int loop = 0; int chave_mestra;
				
				while(loop == 0){
		
					//PRIMEIRO ORDENAR O NÓ QUE JÁ TEMOS
					int flag = 0;
					int aux_pont = -2; int aux_pont_02;
					
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
					//DEPOIS, CRIAR O NOVO NÓ E ADICIONA AS CHAVES CORRESPONDENTES PARA ELE
					TNoInterno * novo_noInterno = no_interno_vazio(d);
					
					chave_mestra = noInterno->chaves[d];
					
					//ADICIONAR OS PONTEIROS E CHAVES NO NOVO NO INTERNO
					for(int i = d; i < (2*d + 1); i++){
						
						novo_noInterno->p[i - d] = noInterno->p[i + 1];
						if(i != d){
							noInterno->p[i] = -1;
						}
						if(i != 2*d){
							novo_noInterno->chaves[i - d] = noInterno->chaves[i + 1];
							noInterno->chaves[i] = -1;
						}				
					}
					
					noInterno->m = d;
					novo_noInterno->m = d;
					
					if(noInterno->aponta_folha == 1){
						
						novo_noInterno->aponta_folha = 1;
						
						TNoFolha * noFolha_aux = NULL;

						for(int i = 0; i < (noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fd, noInterno->p[i], SEEK_SET);
							//LE NO FOLHA DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							noFolha_aux = le_no_folha(d, fd);
							noFolha_aux->pont_pai = pont_pai_01;

							fseek(fd, noInterno->p[i], SEEK_SET);
							salva_no_folha(d, noFolha_aux, fd);

							free(noFolha_aux);
						}
						
						for(int i = 0; i < (novo_noInterno->m + 1); i ++){
						
							//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
							fseek(fd, novo_noInterno->p[i], SEEK_SET);
							//LE NO FOLHA DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							noFolha_aux = le_no_folha(d, fd);
							noFolha_aux->pont_pai = pont_pai_02;

							fseek(fd, novo_noInterno->p[i], SEEK_SET);
							salva_no_folha(d, noFolha_aux, fd);
							
							free(noFolha_aux);
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
							fseek(fi, novo_noInterno->p[i], SEEK_SET);
							
							//LE NO INTERNO DO ARQUIVO PARA ALTERAR O PONTEIRO DO PAI
							TNoInterno * noInterno_01 = le_no_interno(d, fi);
							noInterno_01->pont_pai = pont_pai_02;
							salva_no_interno(d, noInterno_01, fi);
							
							free(noInterno_01);
						}
					}
					
					//SALVAR NOS NO ARQUIVO
					//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
					fseek(fi, pont_pai_01, SEEK_SET);
					salva_no_interno(d, noInterno, fi);
					
					//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
					novo_noInterno->pont_pai = noInterno->pont_pai;
					fseek(fi, pont_pai_02, SEEK_SET);
					salva_no_interno(d, novo_noInterno, fi);
					
					//AVANÇA O PONTEIRO DO METADADOS DE INDICE
					metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
					
					//PROPAGAR A CHAVE DO NOVO NO INTERNO PARA O PAI
					int checador_de_ponteiro = noInterno->pont_pai;
					free(noInterno);
					free(novo_noInterno);
					
					if(checador_de_ponteiro != -1){
						
						//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
						fseek(fi, pont_pai_01, SEEK_SET);
						noInterno = le_no_interno(d, fi);
						
						if(noInterno->m < 2*d){
							
							loop = 1;
							
							//PRIMEIRO ORDENAR O NÓ QUE JÁ TEMOS
						
							int flag = 0;
							int aux_pont; int aux_pont_02;
							
							for(int i = 0; i < noInterno->m; i++){
								
								if(noInterno->chaves[i] > chave_mestra){
									
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
									noInterno->chaves[i] = chave_mestra;
									chave_mestra = aux_chave;
									
								}
							}
							
							noInterno->chaves[noInterno->m] = chave_mestra;
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
						noInterno->chaves[0] = chave_mestra;
						noInterno->m = 1;
						noInterno->aponta_folha = 0;
						
						TNoInterno * tni;
						
						//DA O SEEK NO ARQUIVO DE INDICE ATÉ O CORRESPONDENTE
						fseek(fi, metadados->pont_prox_no_interno_livre, SEEK_SET);
						salva_no_interno(d, noInterno, fi);
						
						fseek(fi, noInterno->p[0], SEEK_SET);
						tni = le_no_interno(d, fi);
						tni->pont_pai = metadados->pont_prox_no_interno_livre; 
						
						fseek(fi, noInterno->p[0], SEEK_SET);
						salva_no_interno(d, tni, fi);
						
						free(tni);
						
						fseek(fi, noInterno->p[1], SEEK_SET);
						tni = le_no_interno(d, fi);
						tni->pont_pai = metadados->pont_prox_no_interno_livre; 
						
						fseek(fi, noInterno->p[1], SEEK_SET);
						salva_no_interno(d, tni, fi);
						
						free(tni);
						
						free(noInterno);
						
						metadados->pont_raiz = metadados->pont_prox_no_interno_livre;
						metadados->pont_prox_no_interno_livre = metadados->pont_prox_no_interno_livre + tamanho_no_interno(d);
					}
				}
				
				//SALVAR ARQUIVO METADADOS
				metadados->pont_prox_no_folha_livre = metadados->pont_prox_no_folha_livre + tamanho_no_folha(d);
				
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
	//ABRE O ARQUIVO DE INDICE
	FILE * fi = fopen(nome_arquivo_indice, "rb+");
	FILE * fd = fopen(nome_arquivo_dados, "rb+");
	
	//USA A FUNÇÃO DE BUSCA PARA ACHAR O NÓ CORRETO
	int buscaNo = busca(cod, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
	
	//LE O NO CORRESPONDENTE
	fseek(fd, buscaNo, SEEK_SET);
	TNoFolha *noFolha = le_no_folha(d, fd);
	
	//PARAMETROS DE CONTROLE
	int possui_chave = 0;
	int pos_chave = 0;
	
	//PROCURA A CHAVE PASSADA NA FUNÇÃO NO NOFOLHA
	for(int i = 0; i < noFolha->m; i++){
	
		if(noFolha->pizzas[i]->cod == cod){
			possui_chave = 1;
			pos_chave = i;
			break;
		}
	}
	
	//CASO A CHAVE EXISTA, EXECUTA A REMOÇÃO
	if(possui_chave == 1){
		
		//CASO A FOLHA TENHA MAIS DE D PIZZAS, A REMOÇÃO SERÁ SIMPLES
		if(noFolha->m > d){
			
			//printf("\n NO FOLHA ANTES: \n");
			//imprime_no_folha(d, noFolha);
			
			//REORDENA A FOLHA PARA SUMIR COM A PIZZA QUE POSSUI O COD PASSADO
			for(int i = pos_chave; i < noFolha->m - 1; i++){
				noFolha->pizzas[i]->cod = noFolha->pizzas[i + 1]->cod;
				strcpy(noFolha->pizzas[i]->nome, noFolha->pizzas[i + 1]->nome);
				strcpy(noFolha->pizzas[i]->categoria, noFolha->pizzas[i + 1]->categoria);
				noFolha->pizzas[i]->preco = noFolha->pizzas[i + 1]->preco;
			}
			
			noFolha->pizzas[noFolha->m] = NULL; 
			noFolha->pizzas[noFolha->m - 1] = NULL; 
			
			//DECRESCE O NUMERO DE CHAVES
			noFolha->m = noFolha->m - 1;
			
			//SALVA A NOVA FOLHA
			fseek(fd, buscaNo, SEEK_SET);
			salva_no_folha(d, noFolha, fd);
			
			free(noFolha);
			
			//FECHA OS ARQUIVOS ABERTOS
			fclose(fd);
			fclose(fi);
			
			//RETORNA O PONTEIRO PARA A FOLHA NA QUAL FOI EXECUTADA A REMOÇÃO
			return buscaNo;
		}
		//CASO TENHA MENOS PIZZAS QUE D, SERÁ NECESSÁRIA UMA DAS OPERAÇÕES DE REORGANIZAÇÃO DA ARVORE B+ 
		else{
			
			//LE O METADADOS CORRESPONDENTE DA ÁRVORE
			TMetadados *metadados = le_arq_metadados(nome_arquivo_metadados);
			
			//CASO A RAIZ SEJA UMA FOLHA, SIMPLESENTE EXCLUI O NÓ SEM PREOCUPAÇÕES
			if(metadados->raiz_folha == 1){
				
				//REORDENA A FOLHA PARA SUMIR COM A PIZZA QUE POSSUI O COD PASSADO
				for(int i = pos_chave; i < noFolha->m - 1; i++){
					noFolha->pizzas[i]->cod = noFolha->pizzas[i + 1]->cod;
					strcpy(noFolha->pizzas[i]->nome, noFolha->pizzas[i + 1]->nome);
					strcpy(noFolha->pizzas[i]->categoria, noFolha->pizzas[i + 1]->categoria);
					noFolha->pizzas[i]->preco = noFolha->pizzas[i + 1]->preco;
				}
				
				noFolha->pizzas[noFolha->m] = NULL; 
				noFolha->pizzas[noFolha->m - 1] = NULL; 
				
				//DECRESCE O NUMERO DE CHAVES
				noFolha->m = noFolha->m - 1;
				
				//SALVA A NOVA FOLHA
				fseek(fd, buscaNo, SEEK_SET);
				salva_no_folha(d, noFolha, fd);
				
				free(noFolha);
				
			}
			//CHECA OS CASOS DE EXCLUSÃO COM INDICE
			else{
				
				//EXISTEM DOIS CASOS: REDISTRIBUIÇÃO E CONCATENAÇÃO
				//REDISTRIBUIÇÃO: MUDAM O CONTEUDO DO INDICE E DAS FOLHAS, MAS NÃO SE PROPAGAM
				//CONCATENAÇÃO: ALÉM DE MUDAR AS FOLHAS, SE PROPAGAM PELA ÁRVORE
				
				//LE O NO INTERNO PAI(PRIMEIRO AFETADO)
				fseek(fi, noFolha->pont_pai, SEEK_SET);
				TNoInterno *noInterno = le_no_interno(d, fi);
				
				int pos = -1;
				//LOCALIZA A POSIÇÃO DA CHAVE NO NÓ INTERNO
				for(int i = 0; i < noInterno->m; i++){
				
					if(noInterno->chaves[i] > noFolha->pizzas[0]->cod){
						pos = i;
						break;
					}
				}
				TNoFolha * vizinho;
				
				//OLHA PRIMEIRO A DIREITA DO PONTEIRO, CASO ELA EXISTA
				if(pos != -1){
				
					fseek(fd, noInterno->p[pos + 1], SEEK_SET);
					vizinho = le_no_folha(d, fd);
					
					//CASO SEJA POSSIVEL PEGAR UMA CHAVE DO VIZINHO SEM DESBALANCEAR A ARVORE
					if(vizinho->m > d){
						
						//COPIA A PIZZA PARA O NÓ FOLHA NA POSIÇÃO CORRETA
						noFolha->pizzas[noFolha->m - 1]->cod = vizinho->pizzas[0]->cod;
						strcpy(noFolha->pizzas[noFolha->m - 1]->nome, vizinho->pizzas[0]->nome);
						strcpy(noFolha->pizzas[noFolha->m - 1]->categoria, vizinho->pizzas[0]->categoria);
						noFolha->pizzas[noFolha->m - 1]->preco = vizinho->pizzas[0]->preco;
						
						//RETIRA A CHAVE TROCADA DO VIZINHO
						for(int i = 0; i < vizinho->m - 1; i++){
							vizinho->pizzas[i]->cod = vizinho->pizzas[i + 1]->cod;
							strcpy(vizinho->pizzas[i]->nome, vizinho->pizzas[i + 1]->nome);
							strcpy(vizinho->pizzas[i]->categoria, vizinho->pizzas[i + 1]->categoria);
							vizinho->pizzas[i]->preco = vizinho->pizzas[i + 1]->preco;
						}
						
						vizinho->pizzas[vizinho->m] = NULL; 
						vizinho->pizzas[vizinho->m - 1] = NULL; 
						
						//DECRESCE O NUMERO DE CHAVES
						vizinho->m = vizinho->m - 1;
						
						//SALVA AMBAS AS FOLHAS
						fseek(fd, noInterno->p[pos + 1], SEEK_SET);
						salva_no_folha(d, vizinho, fd);
						
						fseek(fd, buscaNo, SEEK_SET);
						salva_no_folha(d, noFolha, fd);
						
						//ATUALIZA CHAVE DO NÓ INTERNO
						noInterno->chaves[pos] = vizinho->pizzas[0]->cod;
					}
					//CASO CONTRÁRIO, PUXA TODO O VIZINHO E ELIMINA O NÓ
					//NESSE CASO SÓ SE SALVA O NÓ FOLHA
					else{
						//REORDENA A FOLHA PARA SUMIR COM A PIZZA QUE POSSUI O COD PASSADO
						for(int i = pos_chave; i < noFolha->m - 1; i++){
							noFolha->pizzas[i]->cod = noFolha->pizzas[i + 1]->cod;
							strcpy(noFolha->pizzas[i]->nome, noFolha->pizzas[i + 1]->nome);
							strcpy(noFolha->pizzas[i]->categoria, noFolha->pizzas[i + 1]->categoria);
							noFolha->pizzas[i]->preco = noFolha->pizzas[i + 1]->preco;
						}
						
						noFolha->pizzas[noFolha->m] = NULL; 
						noFolha->pizzas[noFolha->m - 1] = NULL; 
						
						//DECRESCE O NUMERO DE CHAVES
						noFolha->m = noFolha->m - 1;
						
						//COPIA AS PIZZAS DO VIZINHO PARA O NÓ FOLHA NA POSIÇÃO CORRETA
						for(int i = 0; i < vizinho->m; i++){
							
							TPizza * nova_pizza = pizza(vizinho->pizzas[i]->cod,
														vizinho->pizzas[i]->nome,
														vizinho->pizzas[i]->categoria,
														vizinho->pizzas[i]->preco
														);
							
							noFolha->pizzas[noFolha->m + i] = nova_pizza;
						}
						
						//AUMENTA O M DO NÓ FOLHA
						noFolha->m = noFolha->m + vizinho->m;
						noFolha->pont_prox = vizinho->pont_prox;
						
						//ACERTA OS PONTEIROS DO NÓ INTERNO
						for(int i = pos + 1; i < noInterno->m; i++){
							noInterno->p[i] = noInterno->p[i + 1];
							
							noInterno->p[i + 1] = -1;
						}
						//ACERTA AS CHAVES DO NÓ INTERNO
						for(int i = pos; i < noInterno->m - 1; i++){
							noInterno->chaves[i] = noInterno->chaves[i + 1];
							
							noInterno->chaves[i + 1] = -1;
						}
						
						noInterno->p[noInterno->m] = noInterno->p[noInterno->m + 1];
						noInterno->p[noInterno->m + 1] = -1;
						noInterno->chaves[noInterno->m - 1] = -1;
						noInterno->m = noInterno->m - 1;
						
						//SALVA NO FOLHA
						fseek(fd, buscaNo, SEEK_SET);
						salva_no_folha(d, noFolha, fd);
					}
				}
				//CASO CONTRÁRIO, SEGUE A MESMMA LÓGICA ACIMA, PORÉM PARA A ESQUERDA, OU SEJA, INVERTIDO
				else{
					
					pos = noInterno->m;
					
					//LE O VIZINHO DA ESQUERDA
					fseek(fd, noInterno->p[pos - 1], SEEK_SET);
					vizinho = le_no_folha(d, fd);
					
					//CASO SEJA POSSIVEL PEGAR UMA CHAVE DO VIZINHO SEM DESBALANCEAR A ARVORE
					if(vizinho->m > d){
						
						//CRIA NOVA PIZZA BASEADA NO VALOR MAIS ALTO DO VIZINHO DA ESQUERDA
						TPizza * nova_pizza = pizza(vizinho->pizzas[vizinho->m]->cod,
														vizinho->pizzas[vizinho->m]->nome,
														vizinho->pizzas[vizinho->m]->categoria,
														vizinho->pizzas[vizinho->m]->preco
														);
						
						//MOVE TODAS AS PIZZAS DO NO FOLHA PARA A DIREITA
						for(int i = noFolha->m - 1; i >= 0; i--){
							noFolha->pizzas[i + 1] = noFolha->pizzas[i]; 
						}
						
						//ADICIONA A NOVA PIZZA NO INICIO DA FOLHA
						noFolha->pizzas[0] = nova_pizza;
						noFolha->m = noFolha->m + 1;
						
						//REMOVE A PIZZA RETIRADA DO VIZINHO
						vizinho->pizzas[vizinho->m] = NULL;
						vizinho->m = vizinho->m - 1;
						
						//SALVA AMBAS AS FOLHAS
						fseek(fd, noInterno->p[pos - 1], SEEK_SET);
						salva_no_folha(d, vizinho, fd);
						
						fseek(fd, buscaNo, SEEK_SET);
						salva_no_folha(d, noFolha, fd);
						
						//ATUALIZA CHAVE DO NÓ INTERNO
						noInterno->chaves[pos] = noFolha->pizzas[0]->cod;
					}
					//CASO CONTRÁRIO, DEVE OCORRER A CONCATENAÇÃO
					//NESSE CASO, SÓ O VIZINHO SERÁ SALVO
					else{
						
						//REORDENA A FOLHA PARA SUMIR COM A PIZZA QUE POSSUI O COD PASSADO
						for(int i = pos_chave; i < noFolha->m - 1; i++){
							noFolha->pizzas[i]->cod = noFolha->pizzas[i + 1]->cod;
							strcpy(noFolha->pizzas[i]->nome, noFolha->pizzas[i + 1]->nome);
							strcpy(noFolha->pizzas[i]->categoria, noFolha->pizzas[i + 1]->categoria);
							noFolha->pizzas[i]->preco = noFolha->pizzas[i + 1]->preco;
						}
						
						noFolha->pizzas[noFolha->m] = NULL; 
						noFolha->pizzas[noFolha->m - 1] = NULL; 
						
						//DECRESCE O NUMERO DE CHAVES
						noFolha->m = noFolha->m - 1;
						
						//COPIA AS PIZZAS DO VIZINHO PARA O NÓ FOLHA NA POSIÇÃO CORRETA
						for(int i = 0; i < noFolha->m; i++){
							
							TPizza * nova_pizza = pizza(noFolha->pizzas[i]->cod,
														noFolha->pizzas[i]->nome,
														noFolha->pizzas[i]->categoria,
														noFolha->pizzas[i]->preco
														);
							
							vizinho->pizzas[vizinho->m + i] = nova_pizza;
						}
						
						//AUMENTA O M DO NÓ FOLHA
						vizinho->m = vizinho->m + noFolha->m;
						vizinho->pont_prox = noFolha->pont_prox;
						
						//ACERTA OS PONTEIROS DO NÓ INTERNO
						noInterno->p[noInterno->m] = -1;
						noInterno->chaves[noInterno->m - 1] = -1;
						noInterno->m = noInterno->m - 1;
						
						//CASO O NO INTERNO NÃO POSSUA MAIS CHAVES, DEVE-SE ACERTAR OS METADADOS
						if(noInterno->m = 0){
							noInterno->p[0] = -1;
							metadados->pont_raiz = buscaNo;
							metadados->raiz_folha = 1;
						}
						
						//SALVA VIZINHO
						fseek(fd, noInterno->p[pos - 1], SEEK_SET);
						salva_no_folha(d, vizinho, fd);
					}
				}
				
				int pont_noInterno = noFolha->pont_pai;
					
				if(noInterno->m >= d || noInterno->pont_pai == -1){
					
					//SALVA NÓ INTERNO
					fseek(fi, noFolha->pont_pai, SEEK_SET);
					salva_no_interno(d, noInterno, fi);
					
					free(noFolha);
					free(vizinho);
					free(noInterno);
				}
				else{
				
					int loop = 0; int pos_interno = -1;
					
					TNoInterno * vizinho_interno; TNoInterno * pai_interno;
					
					while(loop == 0){
						
						fseek(fi, noInterno->pont_pai, SEEK_SET);
						pai_interno = le_no_interno(d, fi);
						
						for(int i = 0; i < pai_interno->m; i++){
							if(pai_interno->chaves[i] > noInterno->chaves[0]){
								pos_interno = i;
								break;
							}
						}
						
						//EXISTE NO A DIREITA
						if(pos_interno != -1){
						
							fseek(fi, pai_interno->p[pos_interno + 1], SEEK_SET);
							vizinho_interno = le_no_interno(d, fi);
							
							//NÃO NECESSITA DE CONCATENAÇÃO
							if( (vizinho_interno->m + noInterno->m) > 2*d){
								
								//ACIDIONA O MENOR PONTEIRO DO VIZINHO AO NO INTERNO
								noInterno->p[noInterno->m + 1] = vizinho_interno->p[0];
								
								TNoFolha * aux_folha;
								
								fseek(fd, noInterno->p[noInterno->m + 1], SEEK_SET);
								aux_folha = le_no_folha(d, fd);
								noInterno->chaves[noInterno->m] = aux_folha->pizzas[0]->cod;
								free(aux_folha);
								
								noInterno->m = noInterno->m + 1;
								
								//REMOVE A MENOR CHAVE E PONTEIRO DO VIZINHO
								for(int i = 0; i < vizinho_interno->m - 1; i ++){
									vizinho_interno->chaves[i] = vizinho_interno->chaves[i + 1];
									vizinho_interno->chaves[i + 1] = -1;
								}
								for(int i = 0; i < vizinho_interno->m; i++){
									vizinho_interno->p[i] = vizinho_interno->p[i + 1];
									vizinho_interno->p[i + 1] = -1;
								}
								vizinho_interno->m = vizinho_interno->m - 1;
								
								//ATUALIZAR CHAVE DO PAI
								pai_interno->chaves[pos_interno + 1] = vizinho_interno->chaves[0];
								
								//SALVA OS NOS ATUALIZADOS
								fseek(fi, pai_interno->p[pos_interno + 1], SEEK_SET);
								salva_no_interno(d, vizinho_interno, fi);
								
								fseek(fi, noInterno->pont_pai, SEEK_SET);
								salva_no_interno(d, pai_interno, fi);
								
								fseek(fi, pont_noInterno, SEEK_SET);
								salva_no_interno(d, noInterno, fi);
								
								loop = 1;
							}
							//CONCATENA
							else{
								
								//TRANSFERE OS PONTEIROS DO VIZINHO DA DIREITA PARA O NOINTERNO
								for(int i = 0; i < vizinho_interno->m + 1; i++){
									noInterno->p[noInterno->m + 1 + i] = vizinho_interno->p[i];
								}
								noInterno->m = noInterno->m + vizinho_interno->m + 1;
								
								//ACERTA AS CHAVES
								if(noInterno->aponta_folha == 1){
									
									TNoFolha * aux_folha;
									for(int i = 1; i < noInterno->m + 1; i++){
										fseek(fd, noInterno->p[i], SEEK_SET);
										aux_folha = le_no_folha(d, fd);
										noInterno->chaves[i - 1] = aux_folha->pizzas[0]->cod;
										free(aux_folha);
									}
								}
								else{
									TNoInterno* aux_interno;
									for(int i = 1; i < noInterno->m + 1; i++){
										fseek(fi, noInterno->p[i], SEEK_SET);
										aux_interno = le_no_interno(d, fi);
										noInterno->chaves[i - 1] = aux_interno->chaves[0];
										free(aux_interno);
									}
								}
								//ACERTA AS CHAVES E OS PONTEIROS EXTRAS DO PAI
								for(int i = pos_interno + 1; i < pai_interno->m; i++){
									pai_interno->p[i] = pai_interno->p[i + 1];
									
									pai_interno->p[i + 1] = -1;
								}
								
								for(int i = pos_interno; i < pai_interno->m - 1; i++){
									pai_interno->chaves[i] = pai_interno->chaves[i + 1];
									
									pai_interno->chaves[i + 1] = -1;
								}
								
								pai_interno->p[pai_interno->m] = pai_interno->p[pai_interno->m + 1];
								pai_interno->p[pai_interno->m + 1] = -1;
								pai_interno->chaves[pai_interno->m - 1] = -1;
								pai_interno->m = pai_interno->m - 1;
								
								//SALVA O NOINTERNO
								fseek(fi, pont_noInterno, SEEK_SET);
								salva_no_interno(d, noInterno, fi);
								
								//CASO O NO PAI NÃO TENHA MAIS CHAVES E SEJA RAIZ, ALTERA A RAIZ PARA O NOINTERNO
								if(pai_interno->m == 0 && (metadados->pont_raiz == noInterno->pont_pai)){
									metadados->pont_raiz = pont_noInterno;
									loop = 1;
								}
								//CASO O PAI TENHA MAIS QUE D - 1 CHAVES TERMINA
								else if(pai_interno->m >= d){
									loop = 1;
								}
								else{
									pont_noInterno = noInterno->pont_pai;
									noInterno = pai_interno;
								}
							}
							
						}
						//DEVE SE TRABALHAR COM A ESQUERDA
						else{
							
							pos_interno = pai_interno->m;
							
							fseek(fi, pai_interno->p[pos_interno - 1], SEEK_SET);
							vizinho_interno = le_no_interno(d, fi);
							
							//NÃO NECESSITA DE CONCATENAÇÃO
							if((vizinho_interno->m + noInterno->m) > 2*d){
								
								//ACIDIONA O MAIOR PONTEIRO DO VIZINHO AO NO INTERNO
								for(int i = noInterno->m; i > 0; i--){
									noInterno->chaves[i] = noInterno->chaves[i - 1]; 
								}
								for(int i = noInterno->m + 1; i > 0; i--){
									noInterno->p[i] = noInterno->p[i - 1];
								}
								noInterno->p[0] = vizinho_interno->p[vizinho_interno->m];
								
								TNoFolha * aux_folha;
								
								fseek(fd, noInterno->p[0], SEEK_SET);
								aux_folha = le_no_folha(d, fd);
								noInterno->chaves[0] = aux_folha->pizzas[0]->cod;
								free(aux_folha);
								
								noInterno->m = noInterno->m + 1;
								
								vizinho_interno->p[vizinho_interno->m] = -1;
								vizinho_interno->p[vizinho_interno->m - 1] = -1;
								
								//ATUALIZAR CHAVE DO PAI
								pai_interno->chaves[pos_interno + 1] = noInterno->chaves[0];
								
								//SALVA OS NOS ATUALIZADOS
								fseek(fi, pai_interno->p[pos_interno + 1], SEEK_SET);
								salva_no_interno(d, vizinho_interno, fi);
								
								fseek(fi, noInterno->pont_pai, SEEK_SET);
								salva_no_interno(d, pai_interno, fi);
								
								fseek(fi, pont_noInterno, SEEK_SET);
								salva_no_interno(d, noInterno, fi);
								
								loop = 1;
							
							}
							//CONCATENA
							else{
							
								//TRANSFERE OS PONTEIROS DO NOINTERNO PARA O VIZINHO DA ESQUERDA
								for(int i = 0; i < noInterno->m + 1; i++){
									vizinho_interno->p[vizinho_interno->m + 1 + i] = noInterno->p[i];
								}
								vizinho_interno->m = vizinho_interno->m + noInterno->m + 1;
							
								//ACERTA AS CHAVES
								if(vizinho_interno->aponta_folha == 1){
									
									TNoFolha * aux_folha;
									for(int i = 1; i < vizinho_interno->m + 1; i++){
										fseek(fd, vizinho_interno->p[i], SEEK_SET);
										aux_folha = le_no_folha(d, fd);
										vizinho_interno->chaves[i - 1] = aux_folha->pizzas[0]->cod;
										free(aux_folha);
									}
								}
								else{
									
									TNoInterno* aux_interno;
									for(int i = 1; i < vizinho_interno->m + 1; i++){
										fseek(fi, vizinho_interno->p[i], SEEK_SET);
										aux_interno = le_no_interno(d, fi);
										vizinho_interno->chaves[i - 1] = aux_interno->chaves[0];
										free(aux_interno);
									}
								}
								
								//ACERTA AS CHAVES E OS PONTEIROS EXTRAS DO PAI
								pai_interno->p[pai_interno->m + 1] = -1;
								pai_interno->chaves[pai_interno->m - 1] = -1;
								pai_interno->m = pai_interno->m - 1;
								
								//SALVA O NOINTERNO
								fseek(fi, pont_noInterno, SEEK_SET);
								salva_no_interno(d, noInterno, fi);
								
								//CASO O NO PAI NÃO TENHA MAIS CHAVES E SEJA RAIZ, ALTERA A RAIZ PARA O NOINTERNO
								if(pai_interno->m == 0 && (metadados->pont_raiz == noInterno->pont_pai)){
									metadados->pont_raiz = pont_noInterno;
									loop = 1;
								}
								//CASO O PAI TENHA MAIS QUE D - 1 CHAVES TERMINA
								else if(pai_interno->m >= d){
									loop = 1;
								}
								else{
									pont_noInterno = noInterno->pont_pai;
									noInterno = pai_interno;
								}
							}
						}
					}
				}
			}
			
			//FECHA METADADOS
			salva_arq_metadados(nome_arquivo_metadados, metadados);
			free(metadados);
			
			//FECHA OS ARQUIVOS ABERTOS
			fclose(fd);
			fclose(fi);
			
			//RETORNA O PONTEIRO PARA A FOLHA NA QUAL FOI EXECUTADA A REMOÇÃO
			return buscaNo;
		}
		
	}
	//CASO O COD PASSADO NÃO EXISTA NA ARVORE
	else{
		
		free(noFolha);
		
		//FECHA OS ARQUIVOS ABERTOS
		fclose(fd);
		fclose(fi);
		
		//RETORNA MENSAGEM DE ERRO
		return -1;
	}
	
}

void carrega_dados(int d, char *nome_arquivo_entrada, char *nome_arquivo_metadados, char *nome_arquivo_indice, char *nome_arquivo_dados)
{
    FILE* fentrada = fopen(nome_arquivo_entrada, "rb");
	FILE* findice = fopen(nome_arquivo_indice, "wb");
	FILE* fdados = fopen(nome_arquivo_dados, "wb");
	
	TMetadados *tabMetadados = metadados(d, 0, 1, 0, 1 * tamanho_no_folha(d));
    salva_arq_metadados(nome_arquivo_metadados, tabMetadados);
    
	TNoInterno* noInterno = no_interno_vazio(d);
    salva_no_interno(d, noInterno, findice);
    
	TNoFolha * noFolha = no_folha_vazio(d);
    salva_no_folha(d, noFolha, fdados);
   	
	fclose(findice);
    fclose(fdados);
	
	TPizza* p;
	p = le_pizza(fentrada);

	while (p!=NULL){
		
		p = pizza(p->cod, p->nome, p->categoria, p->preco);
		insere(p->cod, p->nome, p->categoria, p->preco, nome_arquivo_metadados, nome_arquivo_indice, nome_arquivo_dados, d);
    	free(p);
		p = le_pizza(fentrada);
    }
    fclose(fentrada);
}