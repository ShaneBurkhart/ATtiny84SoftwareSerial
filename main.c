#define F_CPU 1000000UL // Set clock to calibrate delay
#define BAUD 9600 // Set the target baud

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "software_serial.h"

int main()
{
  init_serial();

  return 0;
}
