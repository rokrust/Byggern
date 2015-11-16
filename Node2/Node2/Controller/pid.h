#pragma once
#include <stdint.h>

void pid_init(float _kp, float _ki, float _kd);
float pid_generate(float r, float y, float dt);
uint16_t pid_encoder_max_value(void);