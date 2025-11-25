/**
 *  @file 	queue.c
 *  @author 	sawada souta
 *  @date 	2025/11/19 
 *  @brief	値 バッファー用のqueue定義
 *  @note	mutex対応
 */

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int init_queue(queue_instance_t *this, int data_length)
{
	if(data_length <= 2) {
		return -1;
	}

	this->data_count = 0;
		
	this->queue_length = data_length;

	this->node = (queue_t*)malloc(sizeof(queue_t) * this->queue_length);
	if(this->node == NULL) {
		return -1;
	}

	this->head = this->node;
	this->tail = &this->node[this->queue_length - 1];

	this->head->next = &this->node[1];
	this->head->prev = this->tail;

	this->tail->next = this->head;
	this->tail->prev = &this->node[this->queue_length - 2];

	this->enqueue_sp = this->head;
	this->dequeue_sp = this->head;

	for(int i = 1; i < this->queue_length - 1; ++i) {
		this->node[i].next = &this->node[i + 1];
		this->node[i].prev = &this->node[i - 1];
	}

	if(pthread_mutex_init(&this->lock, NULL) != 0) {
		free(this->node);

		this->node = NULL;

		return -1;
	}

	return 0;
}

void enqueue(queue_instance_t *this, double data)
{
	if(this == NULL) {
		return;
	}

	if(this->node == NULL || this->enqueue_sp == NULL) {
		return;
	}

	pthread_mutex_lock(&(this->lock));

	this->enqueue_sp->data = data;

	this->enqueue_sp = this->enqueue_sp->next;

	//if(this->data_count < this->queue_length) {
	if(this->queue_length > this->data_count) {
		this->data_count += 1;

	}
	else if(this->data_count == this->queue_length) {
		this->dequeue_sp = this->dequeue_sp->next;
	
	}
	else {
		;
	}

	pthread_mutex_unlock(&(this->lock));
}

double dequeue(queue_instance_t *this)
{
	if(this == NULL) {
		return 0.0;
	}

	if(this->node == NULL || this->enqueue_sp == NULL) {
		return 0.0;
	}

	pthread_mutex_lock(&(this->lock));

	if(this->data_count <= 0) {
		pthread_mutex_unlock(&(this->lock));

		return 0.0;
	}

	double data = this->dequeue_sp->data;

	this->dequeue_sp->data = 0.0;

	this->dequeue_sp = this->dequeue_sp->next;

	if(this->data_count > 0) {
		this->data_count -= 1;
	}
	
	pthread_mutex_unlock(&(this->lock));

	return data;
}

void clear_queue(queue_instance_t *this)
{
	if(this == NULL) {
		return;
	}

	if(this->node == NULL || this->enqueue_sp == NULL) {
		return;
	}

	pthread_mutex_lock(&(this->lock));

	queue_t *sp = this->head;

	for(int i = 0; i < this->queue_length; ++i) {
		sp->data = 0.0;

		sp = sp->next;
	}

	this->data_count = 0;

	pthread_mutex_unlock(&(this->lock));
}

void delete_queue(queue_instance_t *this)
{
	if(this == NULL) {
		return;
	}

	if(this->node == NULL || this->enqueue_sp == NULL) {
		return;
	}

	pthread_mutex_lock(&(this->lock));

	free(this->node);

	this->node = NULL;
	this->head = NULL;
	this->tail = NULL;
	this->enqueue_sp = NULL;
	this->dequeue_sp = NULL;

	this->data_count = 0;

	pthread_mutex_unlock(&(this->lock));
}

double get_average(queue_instance_t *this)
{
	double sum = 0.0;

	if(this == NULL) {
		return 0.0;
	}

	if(this->node == NULL || this->enqueue_sp == NULL) {
		return 0.0;
	}

	pthread_mutex_lock(&(this->lock));

	queue_t *sp = this->dequeue_sp;

	if(this->data_count <= 0) {
		pthread_mutex_unlock(&(this->lock));

		return 0.0;
	}


	for(int i = 0; i < this->data_count; ++i) {
		sum += sp->data;

		sp = sp->next;
	}

	pthread_mutex_unlock(&(this->lock));

	return sum / (double)this->data_count;
}

