
#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <malloc.h>
#include "Customer.h"

typedef struct QueueNode
{
	Cust data;
	struct QueueNode * next;
}QueueNode;

typedef struct
{
	QueueNode * front, * rear;
}QueueType;


void init(QueueType * q)
{
	q->front = NULL;
	q->rear = NULL;
}

int is_empty(QueueType * q)
{
	return (q->front == NULL);
}

void enqueue(QueueType * q, Cust data)
{
	QueueNode * temp = (QueueNode *)malloc(sizeof(QueueNode));
	if(temp == NULL)
	{
		printf("memory alloc fail\n");
		return;
	}
	else
	{
		temp->data = data;
		temp->next = NULL;
		if(is_empty(q))
		{
			q->front = temp;
			q->rear = temp;
		}
		else
		{
			q->rear->next = temp;
			q->rear = temp;
		}
	}
	// printf("sucess\n");
}

void firstEnqueue(QueueType * q, Cust data)//우선순위 스케줄링
{
	QueueNode * temp = (QueueNode *)malloc(sizeof(QueueNode));
	if(temp == NULL)
	{
		printf("memory alloc fail\n");
		return;
	}
	else
	{
		temp->data = data;//vip처리
		temp->next = q->front;

		if(is_empty(q))
		{
			q->front = temp;
			q->rear = temp;
		}
		else
		{
			q->front = temp;
		}
	}
	// printf("sucess\n");
}

Cust dequeue(QueueType * q)
{
	QueueNode * temp = q->front;
	Cust data;
	if(is_empty(q))
	{
		//printf("Queue is empty\n");
	}
	else
	{
		data = temp->data;
		q->front = q->front->next;
		if(q->front == NULL)
		{
			q->rear = NULL;
		}
		free(temp);
		temp = NULL;
		return data;
	}
}

Cust peek(QueueType * q)
{
	if(is_empty(q))
	{
		printf("Queue is empty\n");
		Cust temp;
		temp.flag = 0;
		return temp;
	}
	else
	{
		return q->front->data;
	}
}
#endif /* QUEUE_H_ */
