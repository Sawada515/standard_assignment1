/**
 *  @file 	distance_sensor.h
 *  @author 	sawada souta
 *  @date 	2025/11/12 
 *  @brief	ads1015用
 */

#ifndef DISTANCE_SENSOR_H_
#define DISTANCE_SENSOR_H_

#include "queue/queue.h"

/**
 * @brief センサの値保持用 queue
 */
typedef struct sensor_queue_t {
	queue_instance_t ch0;
	queue_instance_t ch1;
	queue_instance_t ch2;
	queue_instance_t ch3;
} sensor_queue_t;

/**
 *  @brief 測距センサクラス構造体
 */
typedef struct distance_sensor_t {
	int 			pigpio_daemon;
	int			i2c_handler; 

	sensor_queue_t		distance_sensor; 
} distance_sensor_t;

/**
 *  @brief  distance_sensor_t 構造体を初期化
 *  @param[in] this distance_sensor_tのポインタ
 *  @param[in] pigpio_daemon pigpioデーモンへのファイルディスクリプタ
 *  @return なし
 */
void distance_sensor_construct(distance_sensor_t *this, int pigpio_daemon);

/**
 *  @brief  指定したチャンネルからみた距離を返す
 *  @param[in] this distance_sensor_tのポインタ
 *  @param[in] ch chを指定 0 ~ 3
 *  @return 指定したチャンネルの値
 */
double distance_sensor_get_value(distance_sensor_t *this, int ch);

/**
 *  @brief  distance_sensor_t queueを削除
 *  @param[in] this distance_sensor_tのポインタ
 *  @return なし
 */
void distance_sensor_deconstruct(distance_sensor_t *this);

#endif

