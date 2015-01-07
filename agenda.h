/*
 * agenda.h
 *
 *  Created on: 31/12/2014
 *      Author: mbecker
 */


typedef struct contato {
	char *nome;
	int idade;
	struct contato *prx;
} contato;

void lst();
void gravar();
void carregar();
void liberar(contato *inicio);
void add(char*nome, int idade);

void buscar(contato* (*bsc)(contato*));

contato* busca_mais_velho(contato *c);
contato* busca_mais_novo(contato *c);

