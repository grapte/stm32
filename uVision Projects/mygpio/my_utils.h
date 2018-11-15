// my_utils.h
#ifndef MY_UTILS_H
# define MY_UTILS_H

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7
#define I 8
#define J 9
#define K 0xa

typedef unsigned int uint;

void activate_peripheral(uint gpio);

/**
 * gpio name
 * pin  pin of gpio
 * mode 0 for read (input), 1 for write (ouput), 2 for alt function, 3 for analog
 */
void set_register_mode(uint gpio, uint pin, uint mode);

void activate_led(uint gpio, uint pin);

#endif
