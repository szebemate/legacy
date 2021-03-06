/* config.h */
/*
 *   This file is part of AnonAccess, an access system which can be used
 *    to open door or doing other things with an anonymity featured
 *    account managment.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef CONFIG_H__
#define CONFIG_H__

#include <avr/io.h>

#define F_CPU 16000000         // Oszillator-Frequenz in Hz

#define DEBUG uart

//#define ATMEGA128
//#define ATMEGA644
//#define ATMEGA32

//c uart.[ch] defines
#define UART_HOOK
#define UART_INTERRUPT 1
#define UART_BAUD_RATE 9600
#define UART_RXBUFSIZE 128
#define UART_TXBUFSIZE 128
#define UART_LINE_BUFFER_SIZE 40

#define UART_XON_XOFF
#define UART_XON_XOFF_THRESHOLD_1 (UART_RXBUFSIZE - 24)
#define UART_XON_XOFF_THRESHOLD_2 (UART_RXBUFSIZE - 30)

#undef UART_LEDS

/* i2c defines */
//#define I2C_EXTERNAL_PULLUP

#define BOOTSTRAP_ACCOUNTS 200 /* accounts given "for free" */
#define NO_ANON_ADMINS 1

#define EEPROM_SEC_INIT_RWS 2 /*129*/

/* tamperdetection defines */
//#define TAMPER_DETECTION
#define TAMPER_SECT 1
#define TAMPER_PORT PORTB
#define TAMPER_DDR DDRB
#define TAMPER_PIN 2
#define TAMPER_PCMSK PCMSK1
#define TAMPER_PCINT PCINT1_vect

#define HMAC_SHORTONLY

#define LITTLE_ENDIAN

#define KEYMIG_OFFSET 0
#define KEYMIG_SIZE (9*32)

#define TICKETDB_OFFSET (KEYMIG_OFFSET + KEYMIG_SIZE)
#define TICKETDB_SIZE 4096

#define FLMDB_OFFSET (TICKETDB_OFFSET + TICKETDB_SIZE)
#define FLMDB_SIZE 4096

#define TICKET_TIMEOUT (1000LL*60*60*24*30*3) /* timeout value in ms */ /* 90 days */

#define PRINTER_ADDR 0x00

#define QPTINY_AUTOKEYING
//#define QPTINY_CRYPTO_OFF
//#define QPTINY_STREAMAUTH 1

#endif

