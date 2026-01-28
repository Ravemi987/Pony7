/* Garde pour éviter l'inclusion multiple */
#ifndef __QUEUE_H__
#define __QEUE_H__

/* Inclusion des dépendances */
#include <stdbool.h>
#include <stdio.h>

/* Déclaration des types */
typedef struct s_node Node;
typedef struct s_queue Queue;

/* Déclaration des fonctions */
Queue *createQueue();
Queue *queuePush(Queue *q, const void *e);
bool queueEmpty(Queue *q);
Queue *queuePop(Queue *q);
unsigned int queueSize(Queue *q);
const void *queueTop(Queue *q);
const void *queueTail(Queue *q);
void deleteQueue(Queue **q);

#endif
