/**
 *  @file 	dc_motor.h
 *  @author 	sawada souta
 *  @date 	2025/11/12 
 *  @brief	DCモータコントロール関数
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/**
 *  @brief DCモータの設定値を保持する構造体
 */
typedef struct dc_motor_t {
	int 		pigpio_daemon;

	unsigned int 	pwm_frequency;
	unsigned char 	duty;

	unsigned char 	in1_pin;
	unsigned char 	in2_pin;

	unsigned char 	pwm_pin;
} dc_motor_t;

/**
 *  @brief struct dc_motor_t 構造体を初期化
 *  @param[in] this dc_motor_tのポインタ
 *  @param[in] pwm_frequency pwm周波数
 *  @param[in] duty duty比
 *  @param[in] in1_pin in1 pin番号
 *  @param[in] in2_pin in2 pin番号
 *  @param[in] pwm_pin pwm pin番号
 *  @param[in] pigpio_daemon pigpioデーモンへのファイルディスクリプタ
 *  @return なし
 */
void dc_motor_construct
(dc_motor_t *this,
int pigpio_daemon,
unsigned int pwm_frequency,
unsigned char duty,
unsigned char in1_pin,
unsigned char in2_pin,
unsigned char pwm_pin);

/**
 *  @brief DCモータのPWM周波数を指定
 *  @param[in] this dc_motor_tのポインタ
 *  @param[in]  freq 設定するpwm_frequency
 *  @return なし
 */
void dc_motor_set_frequency(dc_motor_t *this, unsigned int freq);

/**
 *  @brief 現在設定されているDCモータのPWM周波数を取得
 *  @param[in] this dc_motor_tのポインタ
 *  @return 現在設定されているDCモータのPWM周波数
 */
unsigned int dc_motor_get_frequency(dc_motor_t *this);

/**
 *  @brief DCモータのduty比を指定
 *  @param[in] this dc_motor_tのポインタ
 *  @param[in] duty 設定するduty比
 *  @return なし
 */
void dc_motor_set_duty(dc_motor_t *this, unsigned char duty);

/**
 *  @brief 現在設定されているDCモータのduty比を取得
 *  @param[in] this dc_motor_tのポインタ
 *  @return 現在設定されているDCモータのduty比
 */
unsigned char dc_motor_get_duty(dc_motor_t *this);

/**
 *  @brief in1の値を設定
 *  @param[in] this dc_motor_tのポインタ
 *  @param[in] var in1に設定するhigh low
 *  @return なし
 */
void dc_motor_set_in1(dc_motor_t *this, unsigned char var);

/**
 *  @brief in2の値を設定
 *  @param[in] this dc_motor_tのポインタ
 *  @param[in] var in2に設定するhigh low
 *  @return なし
 */
void dc_motor_set_in2(dc_motor_t *this, unsigned char var);

#endif

