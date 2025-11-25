/**
 *  @file 	motor_control.h
 *  @author 	F班
 *  @date 	2025/11/21
 *  @brief	ロボット操作関数群
 */

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

#include <stdint.h>
#include "../dc_motor/dc_motor.h"

/**
 *  @brief ロボットを前進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_forward(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを後進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_back(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを右回りさせる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_right_turn(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを左回りさせる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_left_turn(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを右斜めに前進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_right_front(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを左斜めに前進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_left_front(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを右斜めに後進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_right_rear(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを左斜めに後進させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @param[in] r_duty 右モータのduty
 *  @param[in] l_duty 左モータのduty
 *  @return なし
 */
void move_left_rear(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty);

/**
 *  @brief ロボットを停止させる
 *  @param[in] right dc_motor_t型のポインタ
 *  @param[in] left dc_motor_t型のポインタ
 *  @return なし
 */
void move_brake(dc_motor_t *right, dc_motor_t *left);

#endif

