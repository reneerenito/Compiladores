
/* Cola sencilla de enteros (FIFO), usada por Subconjuntos para llevar el
   control de los estados del DFA que todavia faltan por procesar. */
 
#ifndef QUEUE_H
#define QUEUE_H
 
typedef struct queue_node {
  int state;
  struct queue_node *next;
} queue_node;
 
typedef struct {
  queue_node *front;
  queue_node *rear;
} queue;
 
queue queue_create(void);
void queue_enqueue(queue *q, int state);
int queue_dequeue(queue *q);
int queue_is_empty(const queue *q);
void queue_free(queue *q);
 
#endif
