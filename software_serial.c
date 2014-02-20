#include<avr/io.h>
#include<avr/interrupt.h>
#include "software_serial.h"

// Fires every CYCLES_PER_BIT number of cycles
ISR(TIM1_COMPA_vect)
{

}

int is_transmitting = 0;
int is_receiving = 0;

// Initialize the pins, timer and interrupts.
void init_serial()
{
  cli(); // Turn off interrupts

  DDRB |= (1 << TX); // Set TX pin as output
  DDRB &= ~(1 << RX); // Set RX pin as input

  PORTB |= (1 << TX); // Set the TX pin to idle high
}

// Send a byte through the serial pins.
void send_byte(char byte)
{

}

// Checks if the serial port has something to read.
int can_read()
{
  return !is_receiving;
}

// Checks if the serial port can send data.
int can_write()
{
  return !is_receiving;
}
