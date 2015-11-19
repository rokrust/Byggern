#include "../Drivers/motor/motor_driver.h"
#include <util/delay.h>

float kp;
float ki;
float kd;

float reference;
float integral;
float prev_error;

//Set regulator paramaters
void pid_init(float _kp, float _ki, float _kd){
	kp = _kp;
	ki = _ki;
	kd = _kd;
	
	reference	= 0;
	integral	= 0;
	prev_error	= 0;
	
}


//Calculate motor speed
float pid_generate(float r, float y, float dt){
	float error = r - y;
	integral += error * dt;
	float derivative = (error - prev_error) / dt;

	prev_error = error;
	return kp*error + ki*integral + kd*derivative;
}

//Initializes the encoder values with 0 in one end
//and a measured value in the other end
uint16_t pid_encoder_max_value(void){
	motor_speed(125);
	uint16_t enc_val = 0, prev_enc_val = -1;
	
	//Go left until two equal values are measured
	while(enc_val != prev_enc_val){
		enc_val = motor_encoder_read();
		_delay_ms(100);
		prev_enc_val = motor_encoder_read();
	}
	
	//Set position as 0 in the encoder
	motor_speed(0);
	motor_encoder_reset();
	_delay_ms(1000);
	
	//Travel to the other end.
	motor_speed(-125);
	enc_val = 0, prev_enc_val = -1;
	while(enc_val != prev_enc_val){
		enc_val = motor_encoder_read();
		_delay_ms(100);
		prev_enc_val = motor_encoder_read();
	}
	motor_speed(0);
	
	//Find max value of encoder.
	return enc_val;
}

uint16_t function_for_fucked_up_motor_box(){
	_delay_ms(3000);
	motor_encoder_reset();
	printf("Zero set\n");
	_delay_ms(3000);
	printf("Max set\n");
	return motor_encoder_read();
}