#include <stdio.h>
#include <stdlib.h>
#include "pile.h"
#include "calc.h"

int main() {
	printf("\nCalculadora Polonesa\nBem vindo!\n");
	int run = 1;
	do{
		printf("\nSelecione um modo:\n	1 - Resolucao de Expressao\n	2 - Calculadora\n	0 - Sair\n-> ");
		int burro = 0;
		char ans[50];
		do{
			if(burro) printf("Digite um valor valido!\n-> ");
			else burro++; // Sistema anti-burragem
			scanf("%[^\n]s", ans);
			getchar();
		}while((ans[0]!='1' && ans[0]!='2' && ans[0]!='0') || ans[1]!='\0');
		switch(ans[0]){
			case '1': m_expression();
			break;
			case '2': m_calculator();
			break;
			case '0': run = 0;
		}
	}while(run);
	return 0;
}