#ifndef PILE_H
#define PILE_H
#define ptop p->top // Facilita o codigo

/*! \brief A struct que corresponde a um elemento de uma pilha.

	Recebe um valor @c @b double e aponta para o proximo elemento na pilha.
*/typedef struct Pelm{
	double value; ///< O valor desse elemento
	struct Pelm *next; ///< Vai apontar para o proximo elemento na pilha
}Pelm;

/*! \brief A struct que forma o corpo principal de um pilha.

	Tem os unicos objetivo de amazenar o endereco do primeiro elemento da pilha e quantidade de elementos nela.
*/typedef struct Pile{
	int qtt; ///< Quantidade de elementos.
	Pelm *top; ///< Armazena o endereco do primeiro elemento.
}Pile;

Pile genPile();
Pelm*  genPelm();

int push(double, Pile*);
int pop(Pile*);
int poust(Pile*);
double pfirst(Pile*);
void pdisplay(Pile*);

#endif