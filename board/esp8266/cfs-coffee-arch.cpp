/*
 * Copyright (c) 2009, Swedish Institute of Computer Science
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
 *  Coffee architecture-dependent functionality for the AVR-Raven 1284p platform.
 * \author
 *  Nicolas Tsiftes <nvt@sics.se>
 *  Frederic Thepaut <frederic.thepaut@inooi.com>
 *  David Kopf <dak664@embarqmail.com>
 */



#include "cfs-coffee-arch.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"

#include "ESP.h"

void esp8266_flash_write(CFS_CONF_OFFSET_TYPE address, uint8_t* data, CFS_CONF_OFFSET_TYPE length) {

	const uint32_t end = address + length;
	uint32_t i;
	uint32_t next_sector_addr, curr_sector_addr;
	uint32_t offset;
	static uint8_t flash_write_buf[COFFEE_SECTOR_SIZE];

	for (i = address; i < end;)
	{
		next_sector_addr = (i | (COFFEE_SECTOR_SIZE - 1)) + 1;
		curr_sector_addr = i &~(COFFEE_SECTOR_SIZE - 1);
		offset = i - curr_sector_addr;
		if (next_sector_addr > end)
		{
			next_sector_addr = end;
		}
		//1.Read a page from flash and put it into a mirror buffer.
		esp8266_flash_read(curr_sector_addr, flash_write_buf, COFFEE_SECTOR_SIZE);
		//2.Update flash mirror data with new data.
		memcpy(flash_write_buf + offset, data, next_sector_addr - i);
		/**********3. Erase flash page. ***************/
		//esp8266_flash_erase(curr_sector_addr/ COFFEE_SECTOR_SIZE);
		//uint32_t sector = curr_sector_addr / COFFEE_SECTOR_SIZE;
		ESP.flashEraseSector(curr_sector_addr / COFFEE_SECTOR_SIZE);
		/*****4. Write modified data form mirror buffer into the flash.********/
		ESP.flashWrite(curr_sector_addr, (uint32_t*)flash_write_buf, COFFEE_SECTOR_SIZE);

		data = (u8_t*)data + next_sector_addr - i;
		i = next_sector_addr;
	}
}

void esp8266_flash_read(CFS_CONF_OFFSET_TYPE offset, uint8_t* buf, CFS_CONF_OFFSET_TYPE size) {
	ESP.flashRead(offset, (uint32_t*)buf, size);
}

void esp8266_flash_erase(coffee_page_t sector) {
	ESP.flashEraseSector(sector);
	char bufzero[COFFEE_PAGE_SIZE];
	memset(bufzero,0,COFFEE_PAGE_SIZE);

	CFS_CONF_OFFSET_TYPE offset = sector*COFFEE_SECTOR_SIZE;
	int page_per_sector = COFFEE_SECTOR_SIZE / COFFEE_PAGE_SIZE;

	for (int i = 0;i < page_per_sector;i++) {
		ESP.flashWrite(offset, (uint32_t*)bufzero, COFFEE_PAGE_SIZE);
		offset += COFFEE_PAGE_SIZE;
	}
	
}
