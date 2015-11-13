
#define MOTOR_DIR	3
#define MOTOR_EN	4
#define MOTOR_SEL	5
#define MOTOR_RST	6
#define MOTOR_OE	7
#define max_speed 255

#include <stdint.h>

typedef enum motorDirection motorDirection;
enum motorDirection {right, left};

void motor_init(void);

void motor_speed(int16_t speed);
void motor_direction(motorDirection dir);
int motor_encoder_read(void);
void motor_encoder_reset();
void motor_test(void);
uint8_t reverse(uint8_t x);