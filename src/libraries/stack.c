#include "stack.h"
#include <stdlib.h>
#include <assert.h>


struct s_node {
    const void *value;
    struct s_node *next;
};

struct s_stack {
    struct s_node *top;
    int size;
};


Stack *createStack() {
    Stack *s = malloc(sizeof(Stack));
    if (s == NULL) return NULL;
    s->top = NULL;
    s->size = 0;
    return s;
}

Stack *stackPush(Stack *s, const void *e) {
    Node *new = malloc(sizeof(Node));
    new->value = e;
    new->next = s->top;
    s->top = new;
    ++(s->size);
    return s;
}

bool stackEmpty(Stack *s) {
    return s->size == 0;
}

int stackTop(Stack *s) {
    assert(!stackEmpty(s));
    return (s->size) - 1;
}

const void *stackGet(Stack *s) {
    assert(!stackEmpty(s));
    return s->top->value;
}

Stack *stackPop(Stack *s) {
    assert(!stackEmpty(s));
    Node *toDelete = s->top;
    s->top = s->top->next;
    free(toDelete);
    toDelete = NULL;
    --(s->size);
    return s;
}

void deleteStack(Stack **s) {
    if (*s != NULL) {
        Node *toDelete;
        Node *current = (*s)->top;

        while (current != NULL) {
            toDelete = current;
            current = current ->next;
            free(toDelete);
            toDelete = NULL;
        }

        free(*s);
        *s = NULL;
    }
}
