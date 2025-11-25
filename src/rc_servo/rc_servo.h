/**
 *  @file 	rc_servo.h
 *  @author 	sawada souta
 *  @date 	2025/11/11 
 *  @brief	RCサーボコントロール関数
 */

#ifndef RC_SERVO_H_
#define RC_SERVO_H_

#define DEFAULT_RC_SERVO_ANGLE	90

/**
 *  @brief RCサーボの設定値を保持する構造体
 */
typedef struct rc_servo_t {
	int 		pigpio_daemon;
	unsigned short 	angle;
} rc_servo_t;

/**
 *  @brief struct rc_servo_t 構造体を初期化
 *  @param[in] this rc_servo_tのポインタ
 *  @param[in] angle RCサーボモータの初期角度
 *  @param[in] pigpio_daemon pigpioデーモンへのファイルディスクリプタ
 *  @return なし
 */
void rc_servo_construct(rc_servo_t *this, int angle, int pigpio_daemon);

/**
 *  @brief pigpioデーモンへのファイルディスクリプタを渡す
 *  @param[in] this rc_servo_tのポインタ
 *  @param[in] pigpio_daemon pigpioデーモンへのファイルディスクリプタ
 *  @note 使わない
 *  @return なし
 */
void rc_servo_set_param(rc_servo_t *this, int pigpio_daemon);

/**
 *  @brief RCサーボモータの角度を指定
 *  @param[in] this rc_servo_tのポインタ
 *  @param[in] angle RCサーボモータの設定角度
 *  @return なし
 */
void rc_servo_set_angle(rc_servo_t *this, unsigned short angle);

/**
 *  @brief 現在設定されているRCサーボモータの角度を取得
 *  @param[in] this rc_servo_tのポインタ
 *  @return 現在設定されているRCサーボモータの角度
 */
short rc_servo_get_angle(rc_servo_t *this);

#endif

