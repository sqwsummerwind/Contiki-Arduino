/*
 * Copyright (c) 2007, Swedish Institute of Computer Science
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
 * Author: Oliver Schmidt <ol.sc@web.de>
 *
 */
#include "ethernet.h"  
#include "../../net/ip/uip.h"  
#include "../../net/ipv4/uip_arp.h"  
#include "enc28j60.h"
#include "Arduino.h"

 // MAC地址  
struct uip_eth_addr uip_mac;
static unsigned char ethernet_mac[6] = { 0x54,0xA0,0x50,0x7C,0x8A ,0x97 };

void ethernet_init() {

	enc28j60Init(ethernet_mac); /*初始化enc28j60 赋值MAC地址*/
	for (uint8_t i = 0; i < 6; i++)
	{
		uip_mac.addr[i] = ethernet_mac[i];
	}
	uip_setethaddr(uip_mac); /* 设定uip mac地址*/

}

void ethernet_send() {
	Serial.print("SEND DATA uip_len:");
	Serial.println(uip_len);
	enc28j60PacketSend(uip_len, uip_buf);
}

uint16_t ethernet_poll() {
	return enc28j60PacketReceive(1500, uip_buf);
}

