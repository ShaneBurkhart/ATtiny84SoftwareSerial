#include<avr/io.h>
#include<avr/interrupt.h>
#include "software_serial.h"

#include<util/delay.h>

// Fires every CYCLES_PER_BIT number of cycles
ISR(TIM1_OVF_vect) // Transmit timer interrupt
{
  TCNT1 = TIMER_OFFSET; // Set timer to 0

  if(is_transmitting)
  {
    if(current_tx_bit == 0) // Send start bit
    {
      PORTB &= ~(1 << TX);
    }else if(current_tx_bit > 0 && current_tx_bit < 9) // Send appropriate data
    {
      PORTB = (PORTB & ~(1 << TX)) | (((tx_data_byte >> (8 - current_tx_bit)) & 1) << TX);
    }else // Send stop bit
    {
      PORTB |= (1 << TX);
      is_transmitting = 0;
    }

    current_tx_bit ++;
  }else
  {
    current_tx_bit = 0;
    PORTB |= (1 << TX);
  }

}

ISR(TIM0_OVF_vect)
{
  TCNT0 = TIMER_OFFSET_SM;

  if(is_receiving)
  {
    if(current_rx_bit == 0)
    {

      if((PINB >> RX) & 1) // If not start bit then stop
      {
        is_receiving = 0;
        TIMSK0 &= ~(1 << TOIE0); // Turn off interrupt
      }
    }else if(current_rx_bit > 0 && current_rx_bit < 9)
    {
      rx_data_byte |= (((PINB >> RX) & 1) << (8 - current_rx_bit));
    }else
    {
      TIMSK0 &= ~(1 << TOIE0); // Turn off interrupt
      is_receiving = 0;

      if((PINB >> RX) & 1) // If stop bit then we good
      {
        rx_buffer_byte = rx_data_byte;
        rx_data_ready = 1;
      }
    }

    current_rx_bit ++;
  }

}

ISR(PCINT1_vect) // Used to start read phase
{
  if(is_receiving == 0)
  {
    TCNT0 = TIMER_OFFSET_SM + HALF_CYCLE; // First time is half.

    // Set RX timer interrupt
    TIMSK0 |= (1 << TOIE0); // Set overflow interrupt for Timer 0

    is_receiving = 1; // Set reading

    current_rx_bit = 0; // Ensure the counter is correct.

  }
}

uint8_t is_transmitting = 0;
uint8_t is_receiving = 0;
uint8_t current_tx_bit = 0;
uint8_t current_rx_bit = 0;
uint8_t tx_data_byte = 0x00;
uint8_t rx_data_byte = 0x00;
uint8_t rx_data_ready = 0;
uint8_t rx_buffer_byte = 0;

// Initialize the pins, timer and interrupts.
void init_serial()
{
  cli(); // Turn off interrupts

  DDRB |= (1 << TX); // Set TX pin as output
  DDRB &= ~(1 << RX); // Set RX pin as input

  PORTB |= (1 << TX); // Set the TX pin to idle high
  PORTB |= (1 << RX); // Set the RX pull-up

  // Set pin change for RX
  GIMSK |= (1 << PCIE1); // Enable PC interrrupt
  PCMSK1 |= (1 << PCINT9); // Enable RX PC interrupt
  MCUCR |= (0x02 << ISC00); // Enable falling edge PC interrupt

  // Set TX timer interrupt
  TCCR1B |= (CLK_SELECT << CS10); // Set timer clock
  TIMSK1 |= (1 << TOIE1); // Set overflow interrupt for Timer 1
  TCNT1 = TIMER_OFFSET; // Set timer to Intial

  // Turn on clk 0
  TCCR0B |= (CLK_SELECT << CS00); // Set timer clock

  sei(); // Enable interrupts
}

// Send a byte through the serial pins.
void send_byte(uint8_t byte)
{
  tx_data_byte = byte;
  current_tx_bit = 0;
  is_transmitting = 1;
}

// Checks if the serial port has something to read.
uint8_t can_read()
{
  return rx_data_ready;
}

// Reads data if available.
uint8_t read_byte()
{
  rx_data_ready = 0;
  return rx_buffer_byte;
}

// Checks if the serial port can send data.
uint8_t can_write()
{
  return is_transmitting == 0;
}
