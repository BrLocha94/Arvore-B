#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define NOME_ARQUIVO_METADADOS "metadados.dat"
#define NOME_ARQUIVO_INDICE "indice.dat"
#define NOME_ARQUIVO_DADOS "clientes.dat"
#define NOME_ARQUIVO_INICIAL "dados_iniciais.dat"
#define D 2

#include "arvore_b_mais.h"

void print_menu(){

	printf("---Pizzaria do Makon---\n");
	printf("Digite a opção desejada: \n");
	printf("(1) Entrar como cliente. \n");
	printf("(2) Entrar como garçom. \n");
	printf("(3) Entrar como gerente. \n");
	printf("(0) Sair. \n");
	printf("-------------------------\n");
	printf(">> Digite: ");

}


int main(){

    //printf("entreeeeeeeeeeeiiii\n");
	carrega_dados(D, NOME_ARQUIVO_INICIAL, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS);
    //printf("saíiii");

	printf("        d8b                                   ...\n");
	printf("        Y8P                                 .....\n");
	printf("                                           ......===\n");
	printf("88888b. 8888888888888888888 8888b.        .....=======\n");
	printf("888 888b888   d88P    d88P     888b      .....==========\n");
	printf("888  888888  d88P    d88P  .d888888     ....=============\n");
	printf("888 d88P888 d88P    d88P   888  888    ....================\n");
	printf("88888P 8888888888888888888 Y888888   .....==================\n");
	printf("888                                 .....====================\n");
	printf("888                                 ....=======================\n");
	printf("888                                ....=========================\n");
	printf("                                  ....=============================\n");
	printf("                                  ....==============================\n");
	printf("                                  ....================================\n");
	printf("                                  .....================================\n");
	printf("\n");



	char senhaAdm[7];
	strcpy(senhaAdm, "123456");
	char senha[7];

	int user;
	int continuar;
	int opc;
	int code;
	print_menu();
	scanf("%d", &user);

	while(user){

		switch(user){

		    case 1:
				continuar = 1;
				while(continuar){
					printf (" \n ----------- Bem Vindo a Pizzaria mais tecnologica desse bairro! -----------\n");
					printf("Ver Menu: \n");
					printf("(1) Menu Completo. \n");
					printf("(2) Pizzas Salgadas. \n");
					printf("(3) Pizzas Doces. \n");
					printf("(0) Sair.\n");
					printf(">> Digite: ");
					scanf("%d", &opc);

					switch (opc){

						case 1:

							printf("{ PIZZAS SALGADAS } \n \n");
							busca_por_categoria("Salgada", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
							printf("\n");
							printf("{ PIZZAS DOCES } \n \n");
							busca_por_categoria("Doce", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
						
							break;

						case 2:
							
							printf("{ PIZZAS SALGADAS } \n \n"); 
							busca_por_categoria("Salgada", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);

							break;

						case 3:
							
							printf("{ PIZZAS DOCES } \n \n");
							busca_por_categoria("Doce", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);

							break;
						
						case 0:
							continuar = 0;
							break;

						default:
							printf("Invalid input. Try again. \n");
							scanf("%d", &opc);
							
							break;
					}	
				}

				break;
				
		    	
		    
		    case 2 :
				continuar = 1;
				while(continuar == 1){
					printf ("\n -----------Bem Vindo, Sr. Garcom!-----------\n");
					printf("Ver Menu. \n");
					printf("(1) Buscar Pizza. \n");
					printf("(0) Deslogar. \n");
					printf(">> Digite: ");
					scanf("%d", &opc);
					
					switch(opc){

						case 1:
							
							printf("Digite o codigo da pizza: \n");
							scanf("%d", &code);
						
							if (!(busca_pizza(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D))){
								printf("Pizza não encontrada. \n");
								}
								else{
									TPizza *p = busca_pizza(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
									imprime_pizza(p);
									free (p);
								}
							break;
							
						case 0:
							continuar = 0;
							break;

						default:
							printf("Invalid input. Try again. \n");
							scanf("%d", &opc);							
					}
				}

				break;
				
		    case 3 :

		    	continuar = 1;
		    	printf("Digite a senha: \n");
				scanf(" %s", senha);

			    if (strcmp(senha, senhaAdm) == 0){
					printf("Login efetuado com sucesso!\n");


					while(continuar){
						printf ("-----------Bem Vindo, Sr. Gerente!-----------\n");
						printf("Ver Menu: \n");
						printf("(1) Adicionar Pizza. \n");
						printf("(2) Remover Pizza. \n");
						printf("(3) Alterar Pizza. \n");
						printf("(4) Buscar Pizza. \n");
						printf("(5) Remover Categoria. \n");
						printf("(6) Trocar Senha. \n");
						printf("(0) Deslogar. \n");
						printf(">> Digite: ");	
						scanf("%d", &opc);

						switch(opc){

							case 1: //Adicionar Pizza.
							{
								int tam = 100;
								char nomePizza[tam];
								char pizzaAux[tam]; 
								char categoria [20];
								int categoriaAux = 0;
								float preco = 0.0;
								float precoAux= 0.0;
								int again = 1;

								while(again){
									
									printf("Digite o nome da Pizza: \n");
									printf(">> Digite: ");
									scanf(" %s", pizzaAux);

									if (strlen(pizzaAux) < tam){
										strcpy(nomePizza, pizzaAux);
										again = 0;
									
									} else printf("Número de Caracteres Excedido! Tente Novamente. \n");
								}

								again = 1;
								
								while(again){
										
									printf("Escolha o Tipo da Pizza: \n");
									printf("(1) Salgada. \n");
									printf("(2) Doce. \n");
									printf(">> Digite: ");									
									scanf("%d", &categoriaAux);

									if (categoriaAux == 1){
										strcpy(categoria, "Salgada");
										again = 0;
									} 
									else if (categoriaAux == 2){
											strcpy(categoria, "Doce");
											again = 0;
									} 
									else printf("Opção Inválida! Tente novamente. \n");											
									}

								again = 1;

								while(again){
									
									printf("Digite o preço da pizza: \n");
									printf(">> Digite: ");
									scanf("%f", &precoAux);

									if (precoAux >= 0.0){
										preco = precoAux;	
										again = 0;
									
									} else printf("Preço Inválido! Tente Novamente. \n");
								}
									
								printf("Digite um Código: \n");
								printf(">> Digite: ");
								scanf("%d", &code);


								while(insere(code, nomePizza, categoria, preco, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D) == -1) {
									printf("Digite um Código: \n");
									printf(">> Digite: ");
									scanf("%d", &code);

								}
								
								printf("Pizza Adicionada com Sucesso! \n");
								// continuar = 1;
							} break;
								

							case 2: //Remover Pizza
							{
								printf("Digite um Código: \n");
								printf(">> Digite: ");
								scanf("%d", &code);


								while(exclui(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D) == -1) {
									printf("Digite um Código: \n");
									printf(">> Digite: ");
									scanf("%d", &code);

								}
								
								printf("Pizza removida com sucesso!\n");
								// continuar = 1;
							}
								break;

							case 3: //Faza busca, a busca retorna uma pizza e só altera o necessário.
								{
									int alteraAux;
									int tam = 100;
									char nomePizza[tam];
									char pizzaAux[tam]; 
									char categoria [20];
									int categoriaAux = 0;
									float preco = 0.0;
									float precoAux= 0.0;
									int again = 1;
								
									printf("Digite o codigo da pizza: \n");
									scanf("%d", &code);

									TPizza* p = busca_pizza(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);

									if(p == NULL){
										printf("Pizza não encontrada.\n");
									} 
									else{
											printf("O que deseja alterar?: \n");
											printf("(1) Nome. \n");
											printf("(2) Categoria. \n");
											printf("(3) Preço. \n");
											printf(">> Digite: ");
											scanf (" %d", &alteraAux);

											if (alteraAux ==1){
												while(again){
												
													printf("Digite o nome da Pizza: \n");
													printf(">> Digite: ");
													scanf(" %s", pizzaAux);

													if (strlen(pizzaAux) < tam){
														strcpy(nomePizza, pizzaAux);
														altera_pizza(code, nomePizza, p->categoria, p->preco, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
														printf("Nome alterado com sucesso!\n");
														again = 0;
												
													} else printf("Número de Caracteres Excedido! Tente Novamente. \n");
												}
											}
											
											again = 1;

											if (alteraAux == 2){
												while (again){
													
													printf("Escolha o Tipo da Pizza: \n");
													printf("(1) Salgada. \n");
													printf("(2) Doce. \n");
													printf(">> Digite: ");									
													scanf(" %d", &categoriaAux);
													
													if (categoriaAux == 1){
														strcpy(categoria, "Salgada");
														altera_pizza(code, p->nome, categoria, p->preco, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
														again = 0;
													}
													else if (categoriaAux == 2){
														strcpy(categoria, "Doce");
														altera_pizza(code, p->nome, categoria, p->preco, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
														again = 0;
													}
													else{ 
														printf("Opção Inválida! Tente novamente. \n");
													}
												}
											}
											again = 1;
											if(alteraAux == 3){
												while(again){
									
													printf("Digite o preço da pizza: \n");
													printf(">> Digite: ");
													scanf("%f", &precoAux);

													if (precoAux >= 0.0){
														preco = precoAux;
														altera_pizza(code, p->nome, p->categoria, preco, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);	
														again = 0;
													
													}else{ 
														printf("Preço Inválido! Tente Novamente. \n");
													}
													printf("Preco alterado!\n");
												}
											}
										}
								
								}
								// continuar = 1;
								break;

								case 4: //Busca Pizza
								printf("Digite o codigo da pizza: \n");
								scanf("%d", &code);
								if (!(busca_pizza(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D))){
									printf("Pizza não encontrada. \n");
									}

								else{
									TPizza *p = busca_pizza(code, NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
									imprime_pizza(p);
									free (p);
								}
								break;
								
							case 5:
								{
									int again = 1;
									int categoriaAux = 0;
									while(again){
										
										printf("Escolha o Tipo da Pizza: \n");
										printf("(1) Salgada. \n");
										printf("(2) Doce. \n");
										printf(">> Digite: ");									
										scanf("%d", &categoriaAux);
									if (categoriaAux == 1){
										remove_por_categoria("Salgada", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
										again = 0;
										
									} else if (categoriaAux == 2){
										remove_por_categoria("Doce", NOME_ARQUIVO_METADADOS, NOME_ARQUIVO_INDICE, NOME_ARQUIVO_DADOS, D);
										again = 0;
									} else printf("Opção Inválida! Tente novamente. \n");											
								}
								
								printf("Categoria removida com sucesso!\n");
								continuar = 1;
							}							
								break;

							case 6:
								printf("Digite uma nova senha de 6 dígitos: \n");
								char senhaAux[6];
								scanf("%s", senhaAux);

								if (strlen(senhaAux) == 6){
									strcpy(senhaAdm, senhaAux);
									printf("Senha Trocada com Sucesso.\n");
									printf("Nova Senha: %s \n", senhaAdm);
								} else printf("Senha Inválida.\n");
								continuar = 1;
								break;

							case 0:
								continuar = 0;
								break;

							default:
								printf("Invalid input. Try again. \n");
								scanf("%d", &opc);
							}

  					}

  				} else printf("Senha Inválida. \n");
			}

		print_menu();
		scanf("%d", &user);

	}
	
	return 0;
}