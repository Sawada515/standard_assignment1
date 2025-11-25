/**
 *  @file 	queue.h
 *  @author 	sawada souta
 *  @date 	2025/11/11 
 *  @brief	値 バッファー用のqueue定義
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <pthread.h>

/**
 *  @brief 自作queue構造体定義 双方向連結リスト
 */
typedef struct queue_t {
	struct queue_t *next;
	struct queue_t *prev;

	double data;
} queue_t;

/**
 * @brief queueインスタンス
 */
typedef struct queue_instance_t {
	queue_t *node;
	queue_t *head;
	queue_t *tail;
	queue_t *enqueue_sp;
	queue_t *dequeue_sp;

	int queue_length;

	int data_count;

	pthread_mutex_t lock;
} queue_instance_t;

/**
 *  @brief queueの作成
 *  @param[in] data_length 作成するqueueの長さ
 *  @return -1 : 値が不正など，0 : 成功
 */
int init_queue(queue_instance_t *this, int data_length);

/**
 *  @brief queueにデータを追加
 *  @note 古い値を上書きしていきます
 *  @param[in] this queueインスタンスへのポインタ
 *  @param[in] data queueへ追加するデータ
 *  @return なし
 */
void enqueue(queue_instance_t *this, double data);

/**
 *  @brief queueから値の取り出し
 *  @param[in] this queueインスタンスへのポインタ
 *  @return queueから取り出した値
 */
double dequeue(queue_instance_t *this);

/**
 *  @brief queueのクリア
 *  @param[in] this queueインスタンスへのポインタ
 *  @return なし
 */
void clear_queue(queue_instance_t *this);

/**
 *  @brief queueの削除
 *  @return なし
 */
void delete_queue(queue_instance_t *this);

/**
 *  @brief queueに入っているデータから平均を計算
 *  @param[in] this queueインスタンスへのポインタ
 *  @return queueに入っているデータの平均
 */
double get_average(queue_instance_t *this);

#endif

