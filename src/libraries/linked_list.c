#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct s_node {
	int value;
	struct s_node *next;
	struct s_node *previous;
};

struct s_list {
	struct s_node *sentinel;
	int size;
};

List *list() {
	List *l = malloc(sizeof(List));
	if (l == NULL) return NULL;
	l->sentinel = malloc(sizeof(Node));
	if(l->sentinel == NULL) return NULL;
	l->sentinel->next = l->sentinel->previous = l->sentinel;
	l->size = 0;
	return l; 
}

List *push_front(List *l, int v) {
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->previous = l->sentinel;
	new->next = l->sentinel->next;
	new->next->previous = new;
	l->sentinel->next = new;
	++(l->size);
	return l;
}

List *push_back(List *l, int v) {
	Node *new = malloc(sizeof(Node));
	new->value = v;
	new->previous = l->sentinel->previous;
	new->next = l->sentinel;
	new->previous->next = new;
	l->sentinel->previous = new;
	++(l->size);
	return l;
}

List *insert_at(List *l, int i, int v) {
	assert(0 <= i && i <= l->size);
	if (i == 0) {
		return push_front(l, v);
	} else if (i == l->size) {
		return push_back(l, v);
	} else {
		Node *current = l->sentinel->next;
		while (i--) current = current->next;
		Node *new = malloc(sizeof(Node));
		new->value = v;
		new->next = current;
		new->previous = current->previous;
		current->previous->next = new;
		current->previous = new;
		++(l->size);
		return l;
	}

}

bool is_empty(List *l) {
	return l->size == 0;
}

List *pop_front(List *l) {
	assert(!is_empty(l));
	Node *toDelete = l->sentinel->next;
	toDelete->next->previous = l->sentinel;
	l->sentinel->next = toDelete->next;
	free(toDelete);
	toDelete = NULL;
	--(l->size);
	return l;
}

List *pop_back(List *l) {
	assert(!is_empty(l));
	Node *toDelete = l->sentinel->previous;
	toDelete->previous->next = l->sentinel,
	l->sentinel->previous = toDelete->previous;
	free(toDelete);
	toDelete = NULL;
	--(l->size);
	return l;
}

List *remove_at(List *l, int i) {
	assert(!is_empty(l) && 0 <= i && i <= l->size);
	if (i == 0) {
		return pop_front(l);
	} else if (i == l->size) {
		return pop_back(l);
	} else {
		Node *current = l->sentinel->next;
		while (i--) current = current->next;
		Node *toDelete = current;
		current->next->previous = current->previous;
		current->previous->next = current->next;
		free(toDelete);
		toDelete = NULL;
		--(l->size);
		return l;
	}
}

int front(List *l) {
	assert(!is_empty(l));
	return l->sentinel->next->value;
}

int back(List *l) {
	assert(!is_empty(l));
	return l->sentinel->previous->value;
}

int at(List *l, int i) {
	assert(!is_empty(l) && 0 <= i && i <= l->size);
	Node *current = l->sentinel->next;
	while (i--) current = current->next;
	return current->value;
}

void freeList(List **l) {
	if (*l != NULL) {
		Node *toDelete;
		Node *current = (*l)->sentinel->next;
		
		while (current != (*l)->sentinel) {
			toDelete = current;
			current = current->next;
			free(toDelete);
			toDelete = NULL;
		}

		free((*l)->sentinel);
		(*l)->sentinel = NULL;
		free(*l);
		*l = NULL;
	}
}
