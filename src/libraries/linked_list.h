/* Garde pour éviter l'inclusion multiple */
#ifndef __LIST_H__
#define __LIST_H__

/* Inclusion des dépendances */
#include <stdbool.h>

/* Déclaration des synonymes de types */
typedef struct s_node Node;
typedef struct s_list List;

/* Déclaration des fonctions */
List *list();
List *push_front(List *l, int v);
List *push_back(List *l, int v);
List *insert_at(List *l, int i, int v);
bool is_empty(List *l);
List *pop_front(List *l);
List *pop_back(List *l);
List *remove_at(List *l, int i);
int front(List *l);
int back(List *l);
int at(List *l, int i);
void freeList(List **l);

#endif
