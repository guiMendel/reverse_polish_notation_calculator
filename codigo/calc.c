#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pile.h"
#include "calc.h"
#include "printfcolors.h"

#define unless(x) if(!(x))
#define OPERAND 1
#define OPERATOR 2
#define PARENTHESES 0


/// @file calc.h
/// @brief  Define as funcoes do programa de Calculadora
/// @author Guilherme Mendel de A. Nascimento
/// @date 18 Maio 2018


/// @brief Indica qual o tipo do algorismo recebido - se e um operando, um operador ou um parentese
/// @param c - o algarismo em questao
/// @return @c @b 0 se for um parentese, @c @b 1 se for parte de um operando, @c @b 2 se for operador e @c @b -1 se nao for nenhum destes
int tdigit(char c) {
	if(c=='(' || c==')' || c=='[' || c==']' || c=='{' || c=='}') return 0;
	if(((int)c>=48 && (int)c<=57) || c=='.' || c==',') return 1;
	if(c=='+' || c=='-' || c=='/' || c=='*') return 2;
	return -1;
}

/// Pede ao usuario uma expressao matematica valida na forma infixa, faz um teste de validacao da expressao, a passa para o formato posfixo e por fim imprime seu resultado na tela.
/// @brief Inicia o modo de resolucao de expressao do programa
/// @see m_calcuator()
/// @return @c @b Void.
void m_expression() {
	char exp[100], *post;
	printf("\nModo Resolucao de Expressao\n");
	do{
		printf("\nDigite uma expressao valida, ou "BOLD"q"NORMAL" para voltar:\n-> ");
		scanf("%[^\n]s", exp);
		getchar();
	}while(!validate(exp));
	// Caaso o usuario tenha digitado apenas 'q', ele optou por sair do modo
	if(exp[0]=='q') return;		

	post = intopost(exp);
	printf("A expressao no formato posfixo e: "BOLD"%s\n"NORMAL, post);
	printf("Seu resultado e "BOLD"%.2lf\n"NORMAL, solve(post));

	free(post);
}

/// Verifica se a expressao possui algum caracter invalido e se ela possui a quantidade correta de parenteses de abertura e fechamento.
/// @brief Verifica se a expressao parametro e valida.
/// @param exp - expressao em string.
/// @return @c @b 1 se a expressao e valida, @c @b 0 se e invalida.
int validate(char *exp) {

	int exp_size = strlen(exp);
	// Valida os algarismos
	int first_digit_operand = 0;
	for(int i=0; i<exp_size; i++){
		int current_digit = tdigit(exp[i]);
		if(current_digit==OPERAND) {
			// Se o primeiro algarismo nao for um numero deu errado
			unless(first_digit_operand) first_digit_operand++;
			// Checamos se o algarismo depois do operando e outro operando, uma operacao, um fechamento de parentese ou um espaco e o ultimo algarismo
			int j=0;
			while((i+j!=exp_size-1) && (!j || exp[i+j]==' ')) j++;
			if(j) {
				int tdigitr = tdigit(exp[i+j]);
				unless(tdigitr==2 || tdigitr==1 || exp[i+j]==')' || exp[i+j]==']' || exp[i+j]=='}' || exp[i+j]==' ') {
					printf("Erro de operacao com o algarismo na casa %d\n", i);
					return 0;
				}
			}
			j=0;
			// point vai indicar se ha algum ponto (ou virgula). Claro que nao pode ter mais do que um.
			int point=0;
			while((i+j!=exp_size-1) && (!j || tdigit(exp[i+j])==OPERAND)) {
				if(point) return 0;
				else point = 1;
				j++;
			}
		}
		else if(current_digit==OPERATOR) {
			unless(first_digit_operand) {
				printf("A expressao deve comecar com um operando\n");
				return 0;
			}
			// Checamos se a frente tem um operando ou parentese
			int j=0;
			while((i+j!=exp_size-1) && (!j || exp[i+j]==' ')) j++;
			if(j) {
				int tdigitr = tdigit(exp[i+j]);
				unless(tdigitr==1 || exp[i+j]=='(' || exp[i+j]=='[' || exp[i+j]=='{') {
					printf("Erro de operacao com o algarismo na casa %d\n", i);
					return 0;
				}
			}
			// O operador nao pode ser o ultimo
			else {
				printf("Erro de operacao com o algarismo na casa %d\n", i);
				return 0;
			}
		}
		else if(exp[i]=='(' || exp[i]=='[' || exp[i]=='{') {
			int j=0;
			while((i+j!=exp_size-1) && (!j || exp[i+j]==' ')) j++;
			if(j) {
				int tdigitr = tdigit(exp[i+j]);
				unless(tdigitr==1 || tdigitr==0) {
					printf("Erro de operacao com o algarismo na casa %d\n", i);
					return 0;
				}
			}
		}
		else if(exp[i]==')' || exp[i]==']' || exp[i]=='}') {
			int j=0;
			while((i+j!=exp_size-1) && (!j || exp[i+j]==' ')) j++;
			if(j) {
				int tdigitr = tdigit(exp[i+j]);
				unless(tdigitr==2 || tdigitr==0 || exp[i+j]==' ') {
					printf("Erro de operacao com o algarismo na casa %d\n", i);
					return 0;
				}
			}
		}
		else if( exp[i]!=' '){
			// se o usuario digitar apenas 'q' nao sera necessario o repreender
			if(exp[0]!='q' || exp[1]!='\0')
				printf("O algarismo '%c' e invalido.\n-> ", exp[i]);
			else return 1;
			return 0;
		}
	}

	// Valida os parenteses
	Pile p = genPile();
	for(int i=0; i<exp_size; i++){
		// Se ela ler '(', o elemento lido e empilhado
		if(exp[i]=='(') push(1, &p);
		else if(exp[i]=='[') push(2, &p);
		else if(exp[i]=='{') push(3, &p);
		// Se ela ler ')', vai checar se o ultimo elemento empilhado era '('
		else if(exp[i]==')'){ 
			if(pfirst(&p)==1) pop(&p);
			// Se nao for, houve um erro
			else {
				printf("A expressao fornecida possui erro(s) na sintaxe dos '('\n");
				return 0;
			}
		}
		else if(exp[i]==']'){ 
			if(pfirst(&p)==2) pop(&p);
			// Se nao for, houve um erro
			else {
				printf("A expressao fornecida possui erro(s) na sintaxe dos '['\n");
				return 0;
			}
		}
		else if(exp[i]=='}'){ 
			if(pfirst(&p)==3) pop(&p);
			// Se nao for, houve um erro
			else {
				printf("A expressao fornecida possui erro(s) na sintaxe dos '{'\n");
				return 0;
			}
		}
	}
	// Se ao final do loop a pilha nao estiver vazia, houve um erro
	if(p.top) {
		printf("A expressao fornecida possui erro(s) na sintaxe dos ')' ou ']' ou '}'\n");
		return 0;
	}

	printf("A expressao fornecida e valida.\n");

	return 1;
}
/// @brief Transforma a expressao infixa para o formato posfixo
/// @param exp - expressao em string.
/// @return O endereco da nova expressao, alocado dinamicamente.
char* intopost(char* exp) {
	char* r = (char*) malloc(2*(strlen(exp))+1);
	int i, j;
	Pile p = genPile();

	// Na pilha, parenteses terao prioridade 0, + prioridade 1, - prioridade 1.5, * prioridade 2 e / prioridade 2.5
	for(i=0, j=0; i<strlen(exp); i++) {
		// Caso o algarismo seja um numero, ponto ou virgula
		if(((int)exp[i]>47 && (int)exp[i]<58) || (exp[i]=='.') || (exp[i]==',')) {
			r[j++] = exp[i];
		}
		else if(exp[i]=='+' || exp[i]=='-') {
			unless(r[j-1]==' ') r[j++] = ' ';
			double subj;
			if(exp[i]=='+') subj = 1;
			else subj = 1.5;
			if(pfirst(&p)) {
				if(pfirst(&p)==1) r[j++] = '+';
				else if(pfirst(&p)==1.5) r[j++] = '-';
				else if(pfirst(&p)==2) r[j++] = '*';
				else r[j++] = '/';
				r[j++] = ' ';
				pop(&p);
				// Se sobrou coisa na pilha, e importante desempilhar ja agora
				if(pfirst(&p)) {
					if(pfirst(&p)==1) r[j++] = '+';
					else r[j++] = '-';
					r[j++] = ' ';
					pop(&p);
				}
			}

			push(subj, &p);
		}
		else if(exp[i]=='*' || exp[i]=='/') {
			unless(r[j-1]==' ') r[j++] = ' ';
			double subj;
			if(exp[i]=='*') subj = 2;
			else subj = 2.5;
			if(pfirst(&p)>1.5) {
				if(pfirst(&p)==2) r[j++] = '*';
				else r[j++] = '/';
				r[j++] = ' ';
				pop(&p);
				// Se sobrou coisa na pilha, e importante desempilhar ja agora
				if(pfirst(&p)) {
					if(pfirst(&p)==1) r[j++] = '+';
					else r[j++] = '-';
					r[j++] = ' ';
					pop(&p);
				}
			}

			push(subj, &p);
		}
		else if(exp[i]=='(' || exp[i]=='[' || exp[i]=='{') push(0, &p);
		else if(exp[i]==')' || exp[i]==']' || exp[i]=='}') {
			while(pfirst(&p)) {
				unless(r[j-1]==' ') r[j++] = ' ';
				if(pfirst(&p)==1) r[j++] = '+';
				else if(pfirst(&p)==1.5) r[j++] = '-';
				else if(pfirst(&p)==2) r[j++] = '*';
				else r[j++] = '/';
				pop(&p);
			}
			pop(&p);
		}
	}
	while(p.top) {
		unless(r[j-1]==' ') r[j++] = ' ';
		if(pfirst(&p)==1) r[j++] = '+';
		else if(pfirst(&p)==1.5) r[j++] = '-';
		else if(pfirst(&p)==2) r[j++] = '*';
		else r[j++] = '/';
		pop(&p);
	}
	r[j] = '\0';

	return r;
}
/// @brief Resolve uma expressao na notacao posfixa
/// @param exp - expressao a ser resolvida
/// @return O resultado da operacao
double solve(char* exp) {
	int exp_size = strlen(exp);
	Pile p = genPile(), n = genPile(); // p sera a pilha da expressao, n sera usada para montar os operandos
	for(int i=0; i<exp_size; i++) {
		int current_digit = tdigit(exp[i]);
		if(current_digit==OPERAND) push(char_number(exp[i]), &n);
		else if(current_digit==-1) {
			if(n.top) push(dump(&n), &p);
		}
		else {
			if((char)exp[i]=='+') {
				double r = p.top->next->value + p.top->value;
				pop(&p);
				pop(&p);
				push(r, &p);
			}
			else if((char)exp[i]=='-') {
				double r = p.top->next->value - p.top->value;
				pop(&p);
				pop(&p);
				push(r, &p);
			}
			else if((char)exp[i]=='*') {
				double r = p.top->next->value * p.top->value;
				pop(&p);
				pop(&p);
				push(r, &p);
			}
			else {
				unless(p.top->value) {
					printf(RED BOLD"A expressao contem uma divisao por 0!\n"NORMAL);
					return 0;
				}
				double r = p.top->next->value / p.top->value;
				pop(&p);
				pop(&p);
				push(r, &p);
			}
		}
	}
	if(n.top) push(dump(&n), &p);
	return pfirst(&p);
}
/// @brief Faz a conversao de um numero em @c @b char para @c @b int
/// @param c - o @c @b char a ser convertido
/// @return @c @b -1 se o char nao for de um numero, ou o proprio numero se o for
double char_number(char c) {
	if(c=='.' || c==',') return (double)'.';
	unless(((int)c>=48 && (int)c<=57)) {
		printf(YELLOW"--(calc.c.char_number)--> O char '%c' nao e um numero!\n"NORMAL, c);
		return -1;
	}
	return (double)c-48;
}
/// @brief Transforma os numeros dispostos na pilha @c @b n em um @c @b double
/// @param *n - pilha que contem o numero dispoto digito por digito
/// @return O valor convertido
double dump(Pile *n) {
	// Caso a pilha de numeros esteja vazia, ja retorna
	unless(n->top) return 0;
	// A principio, o ponto esta depois do ultimo numero. Caso contrario, vamos descobrir jaja
	int point=0, i=0;
	double r=0;
	Pelm *e = n->top;
	do {
		if((char)e->value=='.') {
			point = i;
			break;
		}
		i++;
		e = e->next;
	}while(e);
	// Coloca os numeros em potencia negativa de 10 (que vem depois da virgula)
	i = 0;
	while(i<point) {
		r += n->top->value*texp((i++)-point);
		pop(n);
	}
	// Para retirar o ponto da pilha
	if((char)pfirst(n)=='.' || (char)pfirst(n)==',') pop(n);
	// Coloca os numeros em potencia positiva de 10 (antes da virgula)
	i = 0;
	while(n->top) {
		r += n->top->value*texp(i++);
		pop(n);
	}
	return r;
}
/// @brief Funcao para achar a potencia @c x de @c 10
/// @param x e a potencia q qual o 10 sera elevado
/// @return @c 10 elevado a @c x
double texp(int x) {
	unless(x) return 1;
	double r=1;
	if(x>0) {
		for(int i=0; i<x; i++) r*=10;
		return r;
	}
	for(int i=0; i>x; i--) r/=10;
	return r;
}
/// @brief Inicia o modo de calculadora do programa
/// @see m_expression()
/// @return @c @b Void.
void m_calculator() {
	Pile p = genPile();
	int r = 1, w=0; // r > return, w > warning
	char c[50]; // c > command
	double num; // num > number (se o comando for um umero, num vai receber seu valor)
	do {
		system("clear");
		printf("Modo Calculadora\n");
		// Imprime avisos relativos as execucoes passadas
		if(w) printf(RED"A pilha necessita ter no minimo 2 elementos para realizar essa funcao!\n"NORMAL);
		calc_pdisplay(&p);
		num = 0;
		// Recebe um valor do teclado
		do {
			printf("\nDigite um valor valido, ou 'q' para voltar\n->");
			scanf("%[^\n]s", c);
			getchar();
		}while( /* PRECISAMOS FAZER UMA FUNCAO QUE CHECA SE O COMANDO INSERIDO POSSUI UM NUMERO OU UMA OPERCAO OU Q OU C, LEMBRANDO QUE O NUMERO PODE UMA SERIE DE CASAS */ !process_Celement(c, &num) );
		unless(c[0]=='q') w = execute(c, num, &p);
		else r=0;
	}while(r);
}
/// @brief Imprime na tela todos os elementos da pilha na calculadora.
/// @param p - um ponteiro para Pile.
/// @return @c @b Void.
void calc_pdisplay(Pile *p) {

	unless(ptop){
		printf("\nPilha vazia\n");
		return;
	}

	printf("\nAtual configuracao da pilha:\n");
	Pelm *e = ptop;
	int i=1;
	while(e){
		printf("	%d: %.2lf\n", i++, e->value );
		e = e->next;
	}
}
/// @brief Executa o comando referenciado em uma pilha de numeros
/// @param *c - um ponteiro para o algarismo que corresponde a um comando
/// @param num - se o comando for para empilhar um numero, vai carregar o valor desse numero
/// @param *p - um ponteiro para @c @b Pile.
/// @return 0 se executada com sucesso, @c @b 1 do contrario.
int execute(char *c, double num, Pile *p) {
	if(num) push(num, p);
	else if(c[0]=='c') {
		if(p->qtt<2) return 1;
		int n = (int)pfirst(p);
		double k = ptop->next->value;
		pop(p);
		pop(p);
		while(n--) push(k, p);
	}
	else if(c[0]=='+') {
		if(p->qtt<2) return 1;
		double r=0;
		if(c[1]) {
			do {
				r += pfirst(p);
				pop(p);
			}while(ptop);
			if(r) push(r, p);
		}
		else {
			r = pfirst(p);
			pop(p);
			if(pfirst(p)+r) ptop->value += r;
			else pop(p);
		}
	}
	else if(c[0]=='-') {
		if(p->qtt<2) return 1;
		double r=pfirst(p);
		pop(p);
		if(c[1]) {
			do {
				r -= pfirst(p);
				pop(p);
			}while(ptop);
			if(r) push(r, p);
		}
		else {
			if(pfirst(p)-r) {
				ptop->value *= -1;
				ptop->value += r;
			}
			else pop(p);
		}
	}
	else if(c[0]=='*') {
		if(p->qtt<2) return 1;
		double r=pfirst(p);
		pop(p);
		if(c[1]) {
			do {
				r *= pfirst(p);
				pop(p);
			}while(ptop);
			push(r, p);
		}
		else {
			ptop->value *= r;
		}
	}
	else { /* c[0]=='/' */
		if(p->qtt<2) return 1;
		double r=pfirst(p);
		pop(p);
		if(c[1]) {
			do {
				r /= pfirst(p);
				pop(p);
			}while(ptop);
			push(r, p);
		}
		else {
			r /= pfirst(p);
			pop(p);
			push(r, p);
		}
	}
	return 0;
}
/// @brief Checa uma string quanto a sua validez para ser empilhada na calculadora e eventualmente armazena em @c @b num um numero
/// 
/// Essa funcao nao so faz a validacao de uma @c @b string, ela tambem inicializa a variavel @c @b num com o valor da @c @b string, caso ela seja um numero
/// @param c - a string que vai ser analisada
/// @see validate()
/// @return @c 0 se a string nao e valida, @c 1 caso contrario
int process_Celement(char *c, double *num) {
	Pile p = genPile();
	// Se o primeiro caracter for um numero, todos os outros tambem devem o ser
	if(((int)c[0]>=48 && (int)c[0]<=57) || c[0]=='.' || c[0]==',') {
		push(char_number(c[0]), &p);
		// Essa flag recebe 1 quando algum numero for diferente de 0. Se todos os numeros forem 0, a string nao sera valida.
		int diff0, point=0;
		if(c[0]=='.' || c[0]==',') {
			point = 1;
			diff0 = 0;
		}
		else {
			if(char_number(c[0])) diff0 = 1;
			else diff0 = 0;
		}
		for(int i=1; i<strlen(c); i++) {
			if(c[i]=='.' || c[i]==',') {
				// Se ja houver algum ponto no numero, tem erro
				if(point) return 0;
				else point = 1;
			}
			else {
				unless((int)c[i]>=48 && (int)c[i]<=57) return 0;
				if(char_number(c[i])) diff0 = 1;
			}
			push(char_number(c[i]), &p);
		}
		unless(diff0) return 0;
		*num = dump(&p);
		return 1;
	}

	if( (c[1]=='\0' && (c[0]=='c' || c[0]=='q' || c[0]=='+' || c[0]=='-' || c[0]=='*' || c[0]=='/')) || (c[1]=='!' && (c[0]=='+' || c[0]=='-' || c[0]=='*' || c[0]=='/')) ) return 1;
	return 0;
}
/// @brief Converte uma @c @b string formada por numeros em @c @b double
/// @param c - a @c @b string a ser convertida
/// @see char_number()
/// @return o valor convertido em @c @b double