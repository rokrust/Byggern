#include <stdint.h>
#define k_p 0.5
#define k_i 0.5
#define OVERFLOW_FIX 64

float p_controller(float reference_position, float max_encoder_value);
uint16_t pi_controller(uint16_t reference_position, uint16_t *error, uint16_t max_encoder_value);
uint16_t controller_init(void);