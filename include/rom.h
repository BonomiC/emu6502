#ifndef ROM_H_
#define ROM_H_

#include <stdint.h>
#include <stdio.h>

#define MAX_ROM_SIZE 24 * 1024

extern uint8_t romBuffer[MAX_ROM_SIZE];

uint16_t read_rom(const char *filename);

#endif /* ROM_H_ */
