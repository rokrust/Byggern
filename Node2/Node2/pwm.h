#define prescaler 8
#define servo_min 13
#define servo_max 37
#define servo_mid (servo_max+servo_min)/2

void pwm_init(void);
void pwm_set_value(unsigned int val);

void pwm_test(void);
void pwm_set_servo(unsigned int val);