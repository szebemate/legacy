/* i2c.c */
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


/* rewrite of i2c.c
 * 
 * 
*/

#define SHOW_STATUS(s) DEBUG_S("DEBUG @ "); \
						DEBUG_S(s); \
						DEBUG_S(": TWSR="); \
						DEBUG_B(TWSR & 0xF8); \
						DEBUG_S(": TWCR="); \
						DEBUG_B(TWCR); \
						DEBUG_S(": TWDR="); \
						DEBUG_B(TWDR); \
						DEBUG_S("\r\n");

// #define SHOW_STATUS(s) //

#include "config.h"

 #undef DEBUG

#include "i2c.h"
#include "debug.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

bool			i2c_ack_flag;
i2c_mode_t	i2c_mode;

/****************************************************
 *  i2c_init()
 * **************************************************/
void			i2c_init(void){
	DDRC 	&= ~(_BV(0) | _BV(1)); // clears bit 0&1 and make them inputs
	#ifdef I2C_EXTERNAL_PULLUP
		PORTC 	&= ~(_BV(0) | _BV(1));
	#else
		PORTC 	|=  (_BV(0) | _BV(1));
	#endif
	TWCR &= ~ _BV(TWEN);
	i2c_do_ack();
	i2c_set_speed(I2C_SPEED);
	i2c_mode = I2C_STOPED; 	
}



/****************************************************
 * i2c_set_speed
 * **************************************************/
void			i2c_set_speed(uint8_t twbr, uint8_t twps){
	/* Baudrate
	 * 				CPU Clock freq.
	 *  SCL freq. = ------------------------
	 * 				16 + 2*(TWBR) * 4^(TWPS) 
	 */
	 TWBR = twbr;
	 TWSR |= (twps & 0x3);
}


/****************************************************
 *  i2c_do_ack
 * **************************************************/
void i2c_do_ack(void){
	DEBUG_S("do_ack\r\n");
	i2c_ack_flag = true;
}


/****************************************************
 *  i2c_do_not_ack
 * **************************************************/
void i2c_do_not_ack(void){
	DEBUG_S("do_not_ack\r\n");
	i2c_ack_flag = false;
}


/****************************************************
 * i2c_start()
 * **************************************************/
i2c_status_t		i2c_start (i2c_addr_t addr){
	
	DEBUG_S("start (");
	DEBUG_B(addr);;
	DEBUG_S(")\r\n");
 	SHOW_STATUS("start-0");
	if ( (i2c_mode != I2C_STOPED)){
//		DEBUG_S("going xtra stop loop\r\n");
		while (! (TWCR & _BV(TWINT)))
			;
	}
	
	SHOW_STATUS("start-1");
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	
	while (! (TWCR & _BV(TWINT)))
		;
	SHOW_STATUS("start-2");
	/* place error-handler here */
	TWDR = addr;
	if((addr & 0x1)==I2C_WRITE){
		i2c_mode = I2C_MT;
	} else {
		i2c_mode = I2C_MR;
	}
	TWCR = _BV(TWINT) | _BV(TWEN);
	while (! (TWCR & _BV(TWINT)))
		;
	SHOW_STATUS("start");
	return TWSR; 
}


/****************************************************
 *  i2c_stop()
 * **************************************************/
i2c_status_t 	i2c_stop(void){
	DEBUG_S("stop\r\n");
	while (! (TWCR & _BV(TWINT)))
		;
		
	if (i2c_mode == I2C_MT){
	/*	while (! (TWCR & _BV(TWINT)))
			;*/
	}
	SHOW_STATUS("stop");
	/* place error-handler here */
	TWCR= _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
	SHOW_STATUS("stop-n")
	while (TWCR & _BV(TWSTO)){
		SHOW_STATUS("stop+n");
		;
	}
	SHOW_STATUS("stop+1");
	i2c_mode = I2C_STOPED;
	//TWCR = 0;	
	/* place error-handler here */
	return TWSR;
}


/****************************************************
 * i2c_sendbyte
 * **************************************************/
i2c_status_t		i2c_sendbyte(uint8_t byte){
	DEBUG_S("send\r\n");
/*	while (! (TWCR & _BV(TWINT)))
		;*/
	SHOW_STATUS("send-1");	
	/* place error-handler here */
	TWDR = byte;
	TWCR = _BV(TWINT) | _BV(TWEN);
	while (! (TWCR & _BV(TWINT)))
		;
	SHOW_STATUS("send-2");
	return TWSR;
}


/****************************************************
 * i2c_readbyte
 * **************************************************/
uint8_t			i2c_readbyte(void){
	DEBUG_S("read\r\n");
/*
 * The left debugstatement make this work!!!
 */
	SHOW_STATUS("read-0");
	while (! (TWCR & _BV(TWINT))) // we shouldn't need this loop
		;
	SHOW_STATUS("read-1");
	TWCR = _BV(TWINT) | _BV(TWEN) | (i2c_ack_flag<<TWEA); // commit old action
	
	while (! (TWCR & _BV(TWINT)))
		;
	SHOW_STATUS("read");
	DEBUG_S("read: 0x");
	DEBUG_B(TWDR);
	DEBUG_S("\r\n");	
	/* place error-handler here */
	return TWDR;
}


/*
 * Nice functions
 */
/****************************************************
 * i2c_restart
 * **************************************************/
i2c_status_t i2c_restart (i2c_addr_t devaddr){
	switch (i2c_mode){
		case I2C_MT:
/*			while (! (TWCR & _BV(TWINT)))
				;*/
			SHOW_STATUS("restart");	
			/* place error-handler here */
			break;
		case I2C_MR:
			break;
		default:
			break;	 
	}
	return i2c_start(devaddr);
}


/****************************************************
 * i2c_start_a
 * **************************************************/
i2c_status_t i2c_start_a (i2c_addr_t devaddr){
	return i2c_start(devaddr);
}

/****************************************************
 * i2c_start_a_ack
 * **************************************************/
i2c_status_t i2c_start_a_ack (i2c_addr_t devaddr){
	uint8_t twsr;
	do{
		twsr = i2c_start(devaddr) & 0xF8;
	}while (twsr==TW_MT_SLA_NACK || twsr==TW_MR_SLA_NACK);
	return twsr;
}


/****************************************************
 * i2c_restart_a
 * **************************************************/
i2c_status_t i2c_restart_a(i2c_addr_t devaddr){
	return i2c_restart(devaddr);
}

