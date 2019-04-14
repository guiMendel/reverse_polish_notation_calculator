#include <stdio.h>
#include <stdlib.h>
#include "pile.h"
#include "printfcolors.h"

#define unless(x) if(!(x))

/// @file pile.h
/// @brief  Possui funcoes e structs relacionadas ao uso de uma pilha para valores double.
/// @author Guilherme Mendel de A. Nascimento
/// @date 18 Maio 2018

/// Aloca memoria dinamicamente para o elemento e faz seu membro @c @b next apontar para @c @b NULL.
/// @brief Gera um novo elemento @c Pelm.
/// @return O endereco do elemento gerado.
Pelm*  genPelm(){
	Pelm *e;
	e = (Pelm *)malloc(sizeof(Pelm));
	unless(e) return NULL;
	e->next = NULL;
	return e;
}
/// @brief Gera uma nova pilha @c Pile
/// 
/// O elemento @c @b qtt e inicializado com @c @b 0 e o @c @b top e inicializado com @c @b NULL
/// @return A pilha gerada
/// @warning E fundamental lembrar de sempre inicializar novas pilhas com essa funcao!
Pile genPile(){
	Pile p;
	p.qtt = 0;
	p.top = NULL;
	return p;
}
/// @brief Coloca um elemento novo na pilha.
/// @param x - o valor do novo elemento.
/// @param p - um ponteiro para @c Pile.
/// @return @c @b 1 caso a funcao seja executada com sucesso, @c @b 0 caso haja algum erro.
int push(double x, Pile *p){

	// Coloca um elem vazio no topo
	if(ptop){
		Pelm *aux = ptop;
		ptop = genPelm();
		ptop->next = aux;
	}
	else ptop = genPelm();

	// Define o valor do elem para x
	ptop->value = x;

	p->qtt++;

	return 1;
}
/// @brief Busca o valor do primeiro elemento da pilha.
/// @param p - um ponteiro para @c Pile.
/// @return O valor do primeiro elemento da pilha, @c @b 0 se ela estiver vazia.
double pfirst(Pile *p) {
	unless(ptop) {
		printf(YELLOW"--(pile.c.pfirst)--> A pilha esta vazia!\n"NORMAL);
		return 0;
	}
	return ptop->value;
}
/// @brief Imprime na tela todos os elementos da pilha.
/// @param p - um ponteiro para Pile.
/// @return @c @b Void.
void pdisplay(Pile *p){

	unless(ptop){
		printf(YELLOW"--(pile.c.pdisplay)--> A pilha esta vazia!\n"NORMAL);
		return;
	}

	printf(YELLOW"--(pile.c.pdisplay)--> Atual configuracao da pilha:\n"NORMAL);
	Pelm *e = ptop;
	int i=1;
	while(e){
		printf("	%d: %.2lf\n", i++, e->value );
		e = e->next;
	}
}
/// @brief Tira o primeiro elemento da pilha.
/// @param p - um ponteiro para Pile.
/// @return @c @b 1 caso a funcao seja executada com sucesso, @c @b 0 caso haja algum erro.
int pop(Pile *p){

	unless(ptop){
		printf(YELLOW"--(pile.c.pop)--> A pilha esta vazia!\n"NORMAL);
		return 0;
	}

	Pelm *e = ptop;
	ptop = ptop->next;
	free(e);

	p->qtt--;

	return 1;
}
/// @brief Tira todos os elementos da pilha.
/// @param p - um ponteiro para Pile.
/// @return @c @b 1 caso a funcao seja executada com sucesso, @c @b 0 caso haja algum erro.
int poust(Pile *p){

	unless(ptop){
		printf(YELLOW"--(pile.c.poust)--> A pilha ja esta vazia!\n"NORMAL);
		return 0;
	}

	// Vamos liberar o espaco elemento por elemento
	Pelm *a, *e = ptop;
	while(e){
		a = e->next;
		free(e);
		e = a;
	}

	// Para indicar que a pilha esta agora vazia
	ptop = NULL;
	p->qtt = 0;

	return 1;
}