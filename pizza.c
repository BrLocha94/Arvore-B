#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <string.h>
#include <stdlib.h>

#include "pizza.h"


void imprime_pizza(TPizza *p)
{
	printf("%d, %s, (%s), R$ %.2f\n", p->cod, p->nome, p->categoria, p->preco);
}

TPizza *pizza(int cod, char *nome, char *categoria, float preco)
{
	TPizza *p = (TPizza *) malloc(sizeof(TPizza));
	if (p) memset(p, 0, sizeof(TPizza));
	p->cod = cod;
	strcpy(p->nome, nome);
	strcpy(p->categoria, categoria);
	p->preco = preco;
	return p;
}

void salva_pizza(TPizza *p, FILE *out)
{
	fwrite(&p->cod, sizeof(int), 1, out);
	fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
	fwrite(p->categoria, sizeof(char), sizeof(p->categoria), out);
	fwrite(&p->preco, sizeof(float), 1, out);
}

TPizza *le_pizza(FILE *in)
{
	printf("LE PIZZA\n");
	TPizza *p = (TPizza *) malloc(sizeof(TPizza));
	printf("LE PIZZA MOD 2\n");
	if (0 >= fread(&p->cod, sizeof(int), 1, in)) {
		printf("LE PIZZA MOD 3\n");
		//printf("\n %i \n", p->cod);
		free(p);
		printf("LE PIZZA MOD 4\n");
		return NULL;
	}
	printf("LE PIZZA MOD 5\n");
	fread(p->nome, sizeof(char), sizeof(p->nome), in);
	printf("LE PIZZA MOD 6\n");
	fread(p->categoria, sizeof(char), sizeof(p->categoria), in);
	printf("LE PIZZA MOD 7\n");
	fread(&p->preco, sizeof(float), 1, in);
	printf("LE PIZZA MOD 8\n");
	imprime_pizza(p);
	printf("\n");
	return p;
}

int cmp_pizza(TPizza *p1, TPizza *p2)
{
	if (p1 == NULL) {
		return (p2 == NULL);
	}
	if (p1->cod != p2->cod) {
		return 0;
	}
	if (strcmp(p1->nome, p2->nome) != 0) {
		return 0;
	}
	if (strcmp(p1->categoria, p2->categoria) != 0) {
		return 0;
	}
	if (p1->preco != p2->preco) {
		return 0;
	}
	return 1;
}

int tamanho_pizza_bytes()
{
	return sizeof(int) + // cod
		sizeof(char) * 50 + // nome
		sizeof(char) * 20 + // categoria
		sizeof(float); // preço
}