//main.c에 함수 정의 작성

#include "main.h"
#include <stdio.h>
#include <stdbool.h>

void power_on(bool* power) {
	*power = true;
}

void move_forward_enable(bool* move_forward) {
	*move_forward = true;
}

void move_forward_disable(bool* move_forward) {
	*move_forward = false;
}

bool front_obstacle() {
	return true;
}

bool left_obstacle() {
	return true;
}

bool right_obstacle() {
	return true;
}

bool dust_existence() {
	return true;
}

void cleaner_on(int* cleaner) {
	*cleaner = 1;
}

void cleaner_off(int* cleaner) {
	*cleaner = 0;
}

void cleaner_up(int* cleaner) {
	*cleaner = 2;
}

bool turn_left(bool* move_forward) {
    stop(move_forward);
    return true;
}

bool turn_right(bool* move_forward) {
    stop(move_forward);
    return true;
}

void stop(bool* move_forward) {
	move_forward_disable(move_forward);
}

