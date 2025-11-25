/**
 *  @file 	rc_servo.c
 *  @author 	sawada souta
 *  @date 	2025/11/11 
 *  @brief	RCサーボモータコントロール関数
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#ifndef DEBUG
#include <pigpiod_if2.h>
#endif

#include "rc_servo.h"

#define SERVO 		14
#define PERIOD		150		//15[ms]	

/**
 *  @brief サーボモータの角度をパルス幅へ変換するための関数
 *  @param[in] x 入力値
 *  @param[in] in_min 入力値の最大
 *  @param[in] in_max 入力値の最小
 *  @param[in] out_min 出力の最小
 *  @param[in] out_max 出力の最大
 *  @return 出力値のスケールに変換した入力値
 */
static short map(short x, short in_min, short in_max, short out_min, short out_max);

/**
 *  @brief サーボモータコントロールスレッド
 *  @param[in] arg スレッドへの引数
 *  @return なし
 */
void *rc_servo_control_thread(void *arg);


/**
 *  @brief サーボモータコントロールスレッドを作成
 *  @param[in] this スレッドへ渡す引数のポインタ
 *  @return なし
 */
void start_rc_servo_control_thread(rc_servo_t *this);

void rc_servo_construct(rc_servo_t *this, int angle, int pigpio_daemon)
{
	static bool is_created_thread = false;

	this->pigpio_daemon = pigpio_daemon;
	this->angle = angle;

#ifndef DEBUG
	set_mode(this->pigpio_daemon, SERVO, PI_OUTPUT);
#endif

	if(!is_created_thread) {
		is_created_thread = true;

		start_rc_servo_control_thread(this);
	}
}

void rc_servo_set_param(rc_servo_t *this, int pigpio_daemon)
{
	this->pigpio_daemon = pigpio_daemon;
}

void rc_servo_set_angle(rc_servo_t *this, unsigned short angle)
{
	this->angle = angle;
}

short rc_servo_get_angle(rc_servo_t *this)
{
	return this->angle;
}


void *rc_servo_control_thread(void *arg)
{
	rc_servo_t *rc_servo = (rc_servo_t *)arg;

	short duty;

	while(1) {
		duty = map(rc_servo->angle, 0, 180, 800, 2300);

#ifndef DEBUG
		set_servo_pulsewidth(rc_servo->pigpio_daemon, SERVO, duty);
#endif

		usleep(1000 * 15);	
	}

}

void start_rc_servo_control_thread(rc_servo_t *this)
{
	pthread_t rc_servo_thread_t;
	int ret;

	RETRY_CREATE_THREAD:

	ret = pthread_create(&rc_servo_thread_t, NULL, rc_servo_control_thread, this);

	if(ret) {
		perror("rc_servo thread_create_error");

		goto RETRY_CREATE_THREAD;
	}

	pthread_detach(rc_servo_thread_t);
}

short map(short x, short in_min, short in_max, short out_min, short out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

