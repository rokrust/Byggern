

float kp;
float ki;
float kd;

float reference;
float integral;
float prev_error;

void pid_init(float _kp, float _ki, float _kd){
	kp = _kp;
	ki = _ki;
	kd = _kd;
	
	reference	= 0;
	integral	= 0;
	prev_error	= 0;
	
}


float pid_generate(float r, float y, float dt){
	
	float error = r - y;
	integral += error * dt;
	float derivative = (error - prev_error) / dt;
	
	prev_error = error;
	
	return kp*error + ki*integral + kd*derivative;
}