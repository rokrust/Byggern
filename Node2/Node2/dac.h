#include <stdint.h>

void dac_initialize(uint8_t MAX520_addr);
void dac_write(uint8_t channel, uint8_t val);