#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "limits.h"

#include "skiplist.h"
#include "rng.h"

#define FIRST_LEVEL 0


typedef struct s_Node {
	int value; /* Clé ou valeur */
	int level; /* Niveau du noeud */
	struct s_Node **next; /* Tableau pour les noeuds suivants */
	struct s_Node **previous; /* Tableau pour les noeuds précédents */
} Node;


struct s_SkipList {
	int maxLevel; /* Nombre de niveaux dans la SkipList */
	Node *sentinel;
	RNG seed; /* Graine de génération aléatoire */
	int size;
};


struct s_SkipListIterator {
	SkipList collection; /* Une SkipList */
	Node *current; /* Le noeud pointé par l'itérateur */
	Node *begin; /* Ici la sentinelle */
	Node *(*next)(Node *); /* Fonction permettant de parcourir la collection */
};


/*  Alloue et initialise une SkipList */
SkipList skiplist_create(int nbLevels) {
	SkipList d = (SkipList)malloc(sizeof(struct s_SkipList)); /* Allocation SkipList */
	if (d == NULL) return NULL;

	d->sentinel = (Node*)malloc(sizeof(Node)); /* Allocation sentinelle */
	if (d->sentinel == NULL) return NULL;

	d->seed = rng_initialize(0); /* Initialisation de la graine de génération à 0 */
	d->size = 0;
	d->maxLevel = d->sentinel->level = nbLevels; /* Le niveau de la sentinelle est celui de la liste */
	d->sentinel->value = INT_MAX; /* La sentinelle a pour valeur 2147483647 (voir rapport) */

	/* Initialisation de la sentinelle */
	d->sentinel->next = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs next */
	if (d->sentinel->next == NULL) return NULL;

	d->sentinel->previous = (Node**)malloc(nbLevels*sizeof(Node*)); /* Allocation du tableau de pointeurs previous */
	if (d->sentinel->previous == NULL) return NULL;

	/* Initialisation des tableaux de pointeurs */
	for (int i = 0; i < nbLevels; ++i) {
		d->sentinel->next[i] = d->sentinel->previous[i] = d->sentinel;
	}

	return d;
}


/* Libère les tableaux de pointeurs d'un noeud ainsi que ce noeud en positionnant sa valeur à NULL */
static void skiplist_free_node(Node **n) {
	if (*n != NULL) {
		free((*n)->next);
		free((*n)->previous);
		free(*n);
		*n = NULL;
	}
}


/* Libère la mémoire utilisée par une SkipList */
void skiplist_delete(SkipList d) {
	Node *toDelete;
	Node *current = d->sentinel->next[FIRST_LEVEL];

	while (current != d->sentinel) {
		toDelete = current;
		current = current->next[FIRST_LEVEL];
		skiplist_free_node(&toDelete); /* Libération du noeud */
	}

	skiplist_free_node(&current); /* Libération de la sentinelle */
	free(d); /* Libération de la skiplist */
}


/* Renvoie le nombre de noeuds dans une SkipList */
unsigned int skiplist_size(SkipList d) {
	return d->size;
}


/* Retourne la valeur dans la SkipList 'd' située à l'index 'i' */
int skiplist_ith(SkipList d, unsigned int i) {
	Node *node = d->sentinel->next[FIRST_LEVEL];
	while (i--) node = node->next[FIRST_LEVEL];
	return node->value;
}


/* Permet d'appliquer un même traitement à tous les noeuds d'une SkipList */
void skiplist_map(SkipList d, ScanOperator f, void *user_data) {
	for (Node *node = d->sentinel->next[FIRST_LEVEL]; node != d->sentinel; node = node->next[FIRST_LEVEL])
		f(node->value, user_data);
}


/* Alloue et initialise un noeud */
Node *makeNode(int level, int value) {
	Node* n = (Node*)malloc(sizeof(Node)); /* Allocation du noeud */
	if (n == NULL) return NULL;

	n->next = (Node**)malloc(level*sizeof(Node*)); /* Allocation du tableau de pointeurs next */
	if (n->next == NULL) return NULL;

	n->previous = (Node**)malloc(level*sizeof(Node*)); /* Allocation du tableau de pointeurs previous */
	if (n->previous == NULL) return NULL;

	n->level = level; /* Nombre de niveux du noeud */
	n->value = value; /* Valeur du noeud */
	
	return n;
}


/* Ajoute une valeur à la SkipList */
SkipList skiplist_insert(SkipList d, int value) {
	Node *update[d->maxLevel]; /* Tableau pour mémoriser les liens à modifier */
	int start_level = (d->maxLevel)-1; /* Parcours (level-1)..0 */
	Node *node = d->sentinel; /* Pointeur vers le noeud courant */

	/* Exploration de tous les niveaux */
	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (node->next[i]->value < value)
			node = node->next[i];
		update[i] = node;
	}
	/* 'value' est supérieure ou égale à la valeur du noeud courant */
	
	node = node->next[FIRST_LEVEL];

	/* Si la valeur n'est pas déjà présente dans la SkipList, création d'un nouveau noeud */
	if (node->value != value) {
		int level = (int)rng_get_value(&(d->seed), (d->maxLevel)-1); /* Génération de 0 à level-1*/
		Node *newNode = makeNode(level+1, value);
		/* Mise à jour des liens pour chaque niveau */
		for (int i = 0; i <= level; ++i) {
			newNode->next[i] = update[i]->next[i];
			newNode->previous[i] = update[i]->next[i]->previous[i];
			update[i]->next[i]->previous[i] = newNode;
			update[i]->next[i] = newNode;
		}
		++(d->size); /* Augmentation de la taille de la SkipList */
	}

	return d;
}


/* Supprime une valeur de la SkipList */
SkipList skiplist_remove(SkipList d, int value) {
	int start_level = (d->maxLevel)-1; /* Parcours (level-1)..0 */
	Node *node = d->sentinel; /* Pointeur vers le noeud courant */

	/* Exploration de tous les niveaux */
	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (node->next[i]->value < value)
			node = node->next[i];
	}
	/* 'value' est supérieure ou égale à la valeur du noeud courant */

	node = node->next[FIRST_LEVEL];

	/* Si le noeud est trouvé, mise à jour des liens sur tous les niveux */
	if (node->value == value) {
		for (int i = 0; i < node->level; ++i) {
			node->next[i]->previous[i] = node->previous[i];
			node->previous[i]->next[i] = node->next[i];
		}
		skiplist_free_node(&node); /* Libération du noeud*/
		--(d->size); /* Diminution de la taille de la liste */

		/* On a pu supprimer des niveaux */
		while (d->maxLevel > 1 && d->sentinel->next[(d->maxLevel)-1] == d->sentinel)
			--(d->maxLevel);
	}

	return d;
}


/* Recherche une valeur dans la SkipList en mémorisant le nombre de noeuds visités */
bool skiplist_search(SkipList d, int value, unsigned int *nb_operations) {
	int start_level = (d->maxLevel)-1; /* Parcours (level-1)..0 */
	Node *current_node = d->sentinel; /* Pointeur vers le noeud courant */
	*nb_operations = 1; /* Nombre d'opérations minimum (exploration de la sentinelle) */

	for (int i = start_level; i >= FIRST_LEVEL; i--) {
		while (current_node->next[i]->value < value) {
			current_node = current_node->next[i];
			++(*nb_operations); /* Un nouveau noeud a été exploré */
		}
		/* Si la valeur a été trouvée, inutile de continuer */
		if (current_node->next[i]->value == value)
			return true;
	}
	return false;
}


/* Renvoie le noeud suivant le noeud 'n' sur le premier niveau */
static Node *goto_next(Node *n) {
	return n->next[FIRST_LEVEL];
}


/* Renvoie le noeud précédant le noeud 'n' sur le premier niveau */
static Node *goto_previous(Node *n) {
	return n->previous[FIRST_LEVEL];
}


/* Alloue et initialise un itérateur sur une SkipList */
SkipListIterator skiplist_iterator_create(SkipList d, unsigned char w) {
	SkipListIterator it = malloc(sizeof(struct s_SkipListIterator));
	if (it == NULL) return NULL;

	it->collection = d;

	/* La SkipList se parcourra de gauche à droite ou de droite à gauche */
	if (w == FORWARD_ITERATOR) {
		it->begin = d->sentinel->next[FIRST_LEVEL];
		it->next = goto_next;
	} else {
		it->begin = d->sentinel->previous[FIRST_LEVEL];
		it->next = goto_previous;
	}

	/* Initialisation du pointeur courant au début de la liste (tête ou queue) */
	it->current = it->begin;

	return it;
}


/* Supprime l'itérateur */
void skiplist_iterator_delete(SkipListIterator it) {
	if (it) free(it);
}


/* Place le pointeur courant au début de l'itérateur */
SkipListIterator skiplist_iterator_begin(SkipListIterator it) {
	it->current = it->begin;
	return it;
}


/* Place le pointeur courant à la fin de l'itérateur */
bool skiplist_iterator_end(SkipListIterator it) {
	return it->current == it->collection->sentinel;
}


/* Déplace le pointeur courant suivant le sens de parcours (FORWARD ou BACKWARD) */
SkipListIterator skiplist_iterator_next(SkipListIterator it) {
	it->current = it->next(it->current);
	return it;
}


/* Renvoie la valeur de l'élément courant */
int skiplist_iterator_value(SkipListIterator it) {
	return it->current->value;
}
