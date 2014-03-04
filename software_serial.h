#ifndef SOFTWARE_SERIAL_H
#define SOFTWARE_SERIAL_H

#define F_CPU 8000000UL
#define BAUD 9600
#define PRESCALAR 8
#define CYCLES_PER_BIT F_CPU / PRESCALAR / BAUD
#define HALF_CYCLE CYCLES_PER_BIT / 2
#define TIMER_OFFSET 65536UL - CYCLES_PER_BIT
#define TIMER_OFFSET_SM 255 - CYCLES_PER_BIT

#define TX PB0
#define RX PB1

#define CLK_SELECT 0x02

extern uint8_t is_transmitting; // Is in the process of sending
extern uint8_t is_receiving; // Is in the process of reading
extern uint8_t current_tx_bit; // Position of tx_data_byte
extern uint8_t current_rx_bit; // Position of rx_data_byte
extern uint8_t tx_data_byte; // Byte to send
extern uint8_t rx_data_byte; // Byte to read to
extern uint8_t rx_data_ready; // Flag if buffer hasn't been read
extern uint8_t rx_buffer_byte; // Stores retrieved value

// Initialize the pins, timer and interrupts.
void init_serial();

// Send a byte through the serial pins.
void send_byte(uint8_t byte);

// Checks if the serial port has something to read.
uint8_t can_read();

// Reads data if available.
uint8_t read_byte();

// Checks if the serial port can send data.
uint8_t can_write();

#endif
