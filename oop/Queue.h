/**
* Filename: Queue.h
* Class: Queue
* Authors: Leonardo Cascianelli, Rory Brown, Ewan Skene
* Date: 2018-11-16
* 
* Description: a queue
*/

#ifndef __Queue__
#define __Queue__

#include <stdint.h>

typedef struct _Queue {
	Object super;
	int cap;
	int front;
	int rear;
	int els;
	uint16_t* Q;
} Queue;

extern const void * Queue_Class_Descriptor;

// Public class methods for Queue
// ...

// Public instance methods for Queue
void Queue_Enqueue(Queue* self, uint16_t element);
uint16_t Queue_Dequeue(Queue* self);
uint16_t Queue_Front(Queue* self);
uint16_t Queue_Size(Queue* self);
uint16_t Queue_Is_Empty(Queue* self);

#endif
