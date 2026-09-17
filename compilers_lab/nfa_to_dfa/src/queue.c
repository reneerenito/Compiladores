#include "queue.h"
#include <stdlib.h>
 
queue queue_create(void) {
  queue q;
  q.front = NULL;
  q.rear = NULL;
  return q;
}
 
void queue_enqueue(queue *q, int state) {
  queue_node *node = malloc(sizeof(queue_node));
  node->state = state;
  node->next = NULL;
 
  if (q->rear == NULL) {
    q->front = node;
    q->rear = node;
  } else {
    q->rear->next = node;
    q->rear = node;
  }
}
 
int queue_dequeue(queue *q) {
  queue_node *node = q->front;
  int state = node->state;
 
  q->front = node->next;
  if (q->front == NULL)
    q->rear = NULL;
 
  free(node);
  return state;
}
 
int queue_is_empty(const queue *q) {
  return q->front == NULL;
}
 
void queue_free(queue *q) {
  while (!queue_is_empty(q))
    queue_dequeue(q);
}
 
