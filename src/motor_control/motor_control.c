/**
 *  @file 	motor_control.c
 *  @author 	F班
 *  @date 	2025/11/21
 *  @brief	ロボット操作関数群
 */

#include "motor_control.h"

#include "../dc_motor/dc_motor.h"

#ifdef DEBUG
#include <stdio.h>

#define debug_print()		printf("%s\n", __func__)
#else
#define debug_print()
#endif

void move_forward(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

	dc_motor_set_duty(right, r_duty);
	dc_motor_set_duty(left, l_duty);

	dc_motor_set_in1(right, 1);
	dc_motor_set_in2(right, 0);

	dc_motor_set_in1(left, 1);
	dc_motor_set_in2(left, 0);
}

void move_back(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

	dc_motor_set_in1(right, 0);
	dc_motor_set_in2(right, 1);

	dc_motor_set_in1(left, 0);
	dc_motor_set_in2(left, 1);
}

void move_right_turn(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

	dc_motor_set_in1(right, 0);
	dc_motor_set_in2(right, 1);

	dc_motor_set_in1(left, 1);
	dc_motor_set_in2(left, 0);
}

void move_left_turn(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

	dc_motor_set_in1(right, 1);
	dc_motor_set_in2(right, 0);

	dc_motor_set_in1(left, 0);
	dc_motor_set_in2(left, 1);
}

void move_right_front(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

        dc_motor_set_in1(right, 1);
        dc_motor_set_in2(right, 0);

        dc_motor_set_in1(left, 1);
        dc_motor_set_in2(left, 0);
}

void move_left_front(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

        dc_motor_set_in1(right, 1);
        dc_motor_set_in2(right, 0);

        dc_motor_set_in1(left, 1);
        dc_motor_set_in2(left, 0);
}

void move_right_rear(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

        dc_motor_set_in1(right, 0);
        dc_motor_set_in2(right, 1);

        dc_motor_set_in1(left, 0);
        dc_motor_set_in2(left, 1);
}

void move_left_rear(dc_motor_t *right, dc_motor_t *left, uint8_t r_duty, uint8_t l_duty)
{
	debug_print();

        dc_motor_set_duty(right, r_duty);
        dc_motor_set_duty(left, l_duty);

        dc_motor_set_in1(right, 0);
        dc_motor_set_in2(right, 1);

        dc_motor_set_in1(left, 0);
        dc_motor_set_in2(left, 1);
}

void move_brake(dc_motor_t *right, dc_motor_t *left)
{
	debug_print();

        dc_motor_set_duty(right, 100);
        dc_motor_set_duty(left, 100);

        dc_motor_set_in1(right, 1);
        dc_motor_set_in2(right, 1);

        dc_motor_set_in1(left, 1);
        dc_motor_set_in2(left, 1);
}
