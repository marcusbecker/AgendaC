/*
 * main.c
 *
 *  Created on: 31/12/2014
 *      Author: mbecker
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <errno.h>

#include "agenda.h"
//#include <encrypt.h>

contato *primeiro = NULL;
contato *ultimo = NULL;

enum OPCOES {
	ADD, LST, GRA, CAR, BSC, LIM, SAIR
};

void erro(char *msg) {
	fprintf(stderr, "Erro: %s", msg);
}

void print_(int a, ...) {
	va_list ap;
	va_start(ap, a);

	int i;
	for (i = 0; i < a; i++) {
		printf("Arg: %i\n", va_arg(ap, int));
	}

	va_end(ap);
}

void split_by(char sep, char * str, char * strEsq, char * strDir) {
	int i;
	int x;

	for (i = 0, x = 0; i < strlen(str); ++i) {
		if (i == x && str[i] != sep) {
			strEsq[x++] = str[i];

		} else if (str[i] == sep) {
			strEsq[i] = '\0';
			x = 0;

		} else if (x < 2 && str[i] != '\n') {
			strDir[x++] = str[i];
		}
	}

	strDir[x] = '\0';
}

int main(int argc, char **argv) {

	short opt;

	char nome[80];
	int idade;

	//print_(3, 31, 42, 26);

	do {
		puts(
				"1 Adicionar | 2 Listar | 3 Gravar | 4 Carregar | 5 Buscas | 6 Limpar | 7 Sair");
		scanf("%1hu", &opt);
		opt--;

		//Pegando ENTER
		char e[2];
		fgets(e, 2, stdin);

		switch (opt) {
		case ADD:
			puts("Digite o nome do contato:");
			//fgets(nome, 79, stdin);
			scanf("%79[^\n]", nome);

			puts("Digite a idade do contato:");
			scanf("%i", &idade);

			add(nome, idade);
			break;

		case LST:
			lst();
			break;

		case GRA:
			gravar();
			break;

		case CAR:
			carregar();
			break;

		case BSC:
			puts("1 Mais Velho | 2 Mais Novo");
			scanf("%1hu", &opt);

			if (opt == 1) {
				buscar(busca_mais_velho);
			} else {
				buscar(busca_mais_novo);
			}

			break;
		case LIM:
			liberar(primeiro);
			primeiro = NULL;
			ultimo = NULL;
			break;

		/*default:
			if (execlp("C:\\Program Files (x86)\\Notepad++\\notepad++.exe",
					"C:\\Program Files (x86)\\Notepad++\\notepad++.exe", NULL)
					== -1) {
				erro(strerror(errno));
			}*/
		}

		puts("\n--------------------------------------");

	} while (opt < SAIR);

	liberar(primeiro);

	return 0;

}

void add(char*n, int i) {

	contato *c = malloc(sizeof(contato));
	c->nome = strdup(n);
	c->idade = i;
	c->prx = NULL;

	if (primeiro == NULL) {
		primeiro = c;

	} else if (ultimo == NULL) {
		//printf("P %s %i", primeiro->nome, primeiro->idade);
		ultimo = c;
		primeiro->prx = ultimo;

	} else {
		ultimo->prx = c;
		ultimo = c;
	}
}

void lst() {
	contato *t = primeiro;
	while (t != NULL) {
		printf("Nome: %s. Idade: %i.\n", t->nome, t->idade);

		t = t->prx;
	}
}

void liberar(contato *inicio) {
	if (inicio == NULL)
		return;

	if (inicio->prx != NULL)
		liberar(inicio->prx);

	free(inicio);
}

void carregar() {
	char linha[83];
	FILE *f = fopen("agenda.txt", "r");

	if (!f)
		return;

	char nome[80];
	char idade[3];

	while (fscanf(f, "%79[^\n]\n", linha) == 1) {
		printf("Carregando: %s\n", linha);

		split_by(';', linha, nome, idade);

		printf("Debug %s | %i \n", nome, atoi(&idade));

		//char *n = strdup(nome);
		//encrypt(n);
		//printf("Debug %s\n", n);

		add(nome, atoi(idade));
	}

	fclose(f);

	//mook
	//char s[] = "Becker";
	//add(s, 20);
}

void gravar() {
	FILE *arq = fopen("agenda.txt", "w");
	contato *t = primeiro;

	while (t) {
		fprintf(arq, "%s;%i\n", t->nome, t->idade);
		t = t->prx;
	}

	fclose(arq);
}

int ordernar(const void* a, const void* b) {
	char** sa = (char**) a;
	char** sb = (char**) b;

	return strcmp(*sa, *sb);
}

void buscar(contato* (*bsc)(contato*)) {

	if (primeiro == NULL) {
		return;
	}

	puts("Busca...\n");

	contato *res = bsc(primeiro);
	printf("Resultado da busca:\n %s, %i", res->nome, res->idade);

	puts("\n\nFim busca...");
}

contato* busca_mais_velho(contato *c) {
	contato *t = c;
	contato *r = NULL;

	while (t != NULL) {
		if (r == NULL) {
			r = t;
		} else if (t->idade > r->idade) {
			r = t;
		}

		t = t->prx;
	}

	//printf("Mais velho %s\n", r->nome);

	return r;
}

contato* busca_mais_novo(contato *c) {
	contato *t = c;
	contato *r = NULL;

	while (t != NULL) {
		//printf("Comparando %s\n", t->nome);

		if (r == NULL || t->idade < r->idade) {
			r = t;
		}

		t = t->prx;
	}
	//printf("Resultado %s\n", r->nome);
	return r;
}
