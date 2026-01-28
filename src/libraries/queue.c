#include "queue.h"
#include <stdlib.h>
#include <assert.h>


struct s_node {
    const void *value;
    struct s_node *next;
};

struct s_queue {
    struct s_node *head;
    struct s_node *tail;
    unsigned int size;
};


Queue *createQueue() {
    Queue *q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

void deleteQueue(Queue **q) {
    if (*q != NULL) {
        Node *toDelete;
        Node *current = (*q)->head;

        while (current != NULL) {
            toDelete = current;
            current = current->next;
            free(toDelete);
            toDelete = NULL;
        }

        free(*q);
        *q = NULL;
    }
}

Queue *queuePush(Queue *q, const void *e) {
    Node **insert_at = (q->size ? &(q->tail->next) : &(q->head));
    Node *new = malloc(sizeof(Node));
    new->value = e;
    new->next = NULL;
    *insert_at = new;
    q->tail = new;
    ++(q->size);
    return q;
}

Queue *queuePop(Queue *q) {
    assert(!queueEmpty(q));
    Node *toDelete = q->head;
    q->head = q->head->next;
    free(toDelete);
    toDelete = NULL;
    --(q->size);
    return q;
}

const void *queueTop(Queue *q) {
    assert(!queueEmpty(q));
    return q->head->value;
}

bool queueEmpty(Queue *q) {
    return q->size == 0;
}

unsigned int getSize(Queue *q) {
    return q->size;
}


const void *queueTail(Queue *q) {
    assert(!queueEmpty(q));
    return q->tail->value;
}
