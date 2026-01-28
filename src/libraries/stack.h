/* Garde pour éviter l'inclusion multiple */
#ifndef __STACK_H__
#define __STACK_H__

/* Inclusion des dépendances */
#include <stdbool.h>
#include <stdio.h>

/* Déclarations des types*/
typedef struct s_node Node;
typedef struct s_stack Stack;

/* Déclaration des fonctions */
Stack *createStack();
Stack *stackPush(Stack *s, const void *e);
bool stackEmpty(Stack *s);
int stackTop(Stack *s);
const void *stackGet(Stack *s);
Stack *stackPop(Stack *s);
void deleteStack(Stack **s);

#endif
