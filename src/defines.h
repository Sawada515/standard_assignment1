/**
 *  @file 	defines.h
 *  @author 	sawada souta
 *  @date 	2025/11/24
 *  @brief	ピン定義など
 */

#ifndef DEFINES_H_
#define DEFINES_H_

#include <stdint.h>

#define M1_IN1		26
#define M1_IN2		6

#define M1_OUT1		5
#define M1_OUT2		0

#define M1_PWM		19

#define M2_IN1		7
#define M2_IN2		16

#define M2_OUT1		20
#define M2_OUT2		21

#define M2_PWM		12

#define set_pwm(pwm)	(pwm)	//!基本的に使わない
#define set_duty(duty)	(duty)	//!基本的に使わない

#define SHM_NAME	"/gpio_control_shm"
#define SHM_SIZE	48

/**
 * @brief ロボット方向
 * @note python側でも同様の定義を作成する
 */
typedef enum direction {
	STOP,
	FORWARD,
	BACK,
	RIGHT_TURN,
	LEFT_TURN,
	RIGHT_FRONT,
	LEFT_FRONT,
	RIGHT_REAR,
	LEFT_REAR
} direction_t;	

/**
 *@brief python <=> c 通信用構造体
 */
typedef struct data_packet_t {
	uint8_t r_duty;
	uint8_t l_duty;
	uint8_t direction;

	uint8_t angle;
	
	double distance_sensor1;
	double distance_sensor2;
	double distance_sensor3;
	double distance_sensor4;
} __attribute__((packed)) data_packet_t;

/**
 * @brief 共有メモリにデータを書き込み
 * @param [in] shm_addr 共有メモリの先頭アドレスi
 * @param [in] r_duty
 * @param [in] l_duty
 * @param [in] direction
 * @param [in] angle
 * @param [in] distance_sensor1
 * @param [in] distance_sensor2
 * @param [in] distance_sensor3
 * @param [in] distance_sensor4
 * @return なし
 */
void 
shm_write(void *shm_addr,
	  uint8_t r_duty,
	  uint8_t l_duty,
	  uint8_t direction,
	  uint8_t angle,
	  double distance_sensor1,
	  double distance_sensor2,
	  double distance_sensor3,
	  double distance_sensor4);

void 
shm_read(void *shm_addr,
	 uint8_t *r_duty,
	 uint8_t *l_duty,
	 uint8_t *direction,
	 uint8_t *angle,
	 double *distance_sensor1,
	 double *distance_sensor2,
	 double *distance_sensor3,
	 double *distance_sensor4);

void
shm_write_sensor_value(void *shm_addr,
          double distance_sensor1,
          double distance_sensor2,
          double distance_sensor3,
          double distance_sensor4);

#endif

