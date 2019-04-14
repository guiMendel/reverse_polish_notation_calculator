#ifndef CALC_H
#define CALC_H

// Modos //////////////////////////////////////////////////////////
void m_expression();
void m_calculator();

// Gerais
double char_number(char);
double dump(Pile*);

// Da Expressao ///////////////////////////////////////////////////
int tdigit(char);
int validate(char*);
char* intopost(char*);
double solve(char*);
double texp(int);

// Da Calculadora ////////////////////////////////////////////////
void calc_pdisplay(Pile*);
int execute(char*, double, Pile*);
int process_Celement(char*, double*);

#endif