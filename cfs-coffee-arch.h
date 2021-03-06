/*
 * Copyright (c) 2008, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *  Coffee architecture-dependent header for the AVR-Raven 1284p platform.
 *  The 1284p has 4KB of onboard EEPROM and 128KB of program flash.
 * \author
 *  Frederic Thepaut <frederic.thepaut@inooi.com>
 *  David Kopf <dak664@embarqmail.com>
 */

#ifndef CFS_COFFEE_ARCH_H
#define CFS_COFFEE_ARCH_H

#include "contiki-conf.h"


/* Byte page size, starting address on page boundary, and size of the file system */
#define COFFEE_PAGE_SIZE          256
#ifndef COFFEE_ADDRESS            //Make can pass starting address with COFFEE_ADDRESS=0xnnnnnnnn, default is 64KB for webserver
#define COFFEE_ADDRESS            (FLASH_BASE+FLASH_PAGE_SIZE*64)
#endif
#define COFFEE_PAGES              (((FLASH_BANK1_END)-(COFFEE_ADDRESS))/COFFEE_PAGE_SIZE)	//511
#define COFFEE_START              (COFFEE_ADDRESS & ~(COFFEE_PAGE_SIZE-1))
#define COFFEE_SIZE               (COFFEE_PAGES*COFFEE_PAGE_SIZE)

/* These must agree with the parameters passed to makefsdata */
#define COFFEE_SECTOR_SIZE        FLASH_PAGE_SIZE	//1KB
#define COFFEE_NAME_LENGTH        16

/* These are used internally by the AVR flash read routines */
/* Word reads are faster but take 130 bytes more PROGMEM */
#define FLASH_WORD_READS          1
/* 1=Slower reads, but no page writes after erase and 18 bytes less PROGMEM. Best for dynamic file system */
#define FLASH_COMPLEMENT_DATA     0 

/* These are used internally by the coffee file system */
/* Micro logs are not needed for single page sectors */
#define COFFEE_MAX_OPEN_FILES     8
#define COFFEE_FD_SET_SIZE        8
#define COFFEE_LOG_TABLE_LIMIT    16
#define COFFEE_DYN_SIZE           (COFFEE_PAGE_SIZE*1)
#define COFFEE_MICRO_LOGS         1
#define COFFEE_LOG_SIZE           128

//#define ESP8266_BOARD


/* coffee_page_t is used for page and sector numbering
 * uint8_t can handle 511 pages.
 * cfs_offset_t is used for full byte addresses
 * If CFS_CONF_OFFSET_TYPE is not defined it defaults to int.
 * uint16_t can handle up to a 65535 byte file system.
 */
#define coffee_page_t uint32_t
#define CFS_CONF_OFFSET_TYPE uint32_t

//#define COFFEE_PREV_SECTOR_SIZE 256		//(COFFEE_ADDRESS)/(4*1024)

#define COFFEE_WRITE(buf, size, offset) \
        stm32_flash_write((COFFEE_START)+offset, (uint8_t *) buf, size)

#define COFFEE_READ(buf, size, offset) \
        stm32_flash_read((COFFEE_START)+offset, (uint8_t *) buf, size)

#define COFFEE_ERASE(sector) stm32_flash_erase(sector)
		
void stm32_flash_write(CFS_CONF_OFFSET_TYPE offset, uint8_t* buf, CFS_CONF_OFFSET_TYPE size);

void stm32_flash_read(CFS_CONF_OFFSET_TYPE offset, uint8_t* buf, CFS_CONF_OFFSET_TYPE size);

void stm32_flash_erase(coffee_page_t sector);


#endif /* !COFFEE_ARCH_H */
