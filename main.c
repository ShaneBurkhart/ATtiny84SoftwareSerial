#define F_CPU 8000000UL

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include "software_serial.h"

int main()
{
  CLKPR = (1 << CLKPCE);
  CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);

  init_serial();

  DDRB |= (1 << PB2);
  PORTB |= (1 << PB2);

  while(1){

    _delay_ms(1000);

    if(can_write())
    {
      send_byte('0');
    }

    if(can_read())
    {
      if(read_byte() == '0')
      {
        PORTB &= ~(1 << PB2);
        _delay_ms(1000);
        PORTB |= (1 << PB2);
      }
    }



  }

  return 0;
}
