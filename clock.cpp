
#include "Arduino.h"
#include "clock.h"
#include "clock-avr.h"
#include "etimer.h"

//#include <avr/io.h>
//#include <avr/interrupt.h>

#define RATE 1    		// ms


static volatile clock_time_t count, scount;
static volatile unsigned long seconds;

/*---------------------------------------------------------------------------*/
/*void systick_handler() 
{
	count++;
	if (++scount == CLOCK_SECOND) 
	{
		scount = 0;
		seconds++;
	}
	if (etimer_pending()&&etimer_next_expiration_time() <= count) {
		etimer_request_poll();
	}
}*/

void HAL_SYSTICK_Callback(void) {
	count++;
	if (++scount == CLOCK_SECOND)
	{
		scount = 0;
		seconds++;
	}
	if (etimer_pending() && etimer_next_expiration_time() <= count) {
		etimer_request_poll();
	}
}
//SIGNAL(SIG_OUTPUT_COMPARE0)
/*
ISR(AVR_OUTPUT_COMPARE_INT)
{
  count++;
  if(++scount == CLOCK_SECOND) {
    scount = 0;
    seconds++;
  }
  if(etimer_pending()) {
    etimer_request_poll();
  }
}
*/
/* External clock source does not work ? */

#if 0
void
clock_init(void)
{
  cli ();
  TIMSK &= ((unsigned char)~(1 << (TOIE0)));
  TIMSK &= ((unsigned char)~(1 << (OCIE0)));
  /* Disable TC0 interrupt */

  /** 
   *  set Timer/Counter0 to be asynchronous 
   *  from the CPU clock with a second external 
   *  clock(32,768kHz)driving it
   */
  ASSR |= (1 << (AS0));
  TCCR0 = _BV (CS02) | _BV (CS01) | _BV (WGM1);

  TCNT0 = 0;
  OCR0 = 128;
  
  TIMSK |= (1 << (OCIE0));
  TIMSK |= (1 << (TOIE0));
  sei ();
}
#endif

/*---------------------------------------------------------------------------*/
/*void
clock_init(void)
{
  cli ();


  OCRSetup();

  /*
   * Counts the number of ticks. Since clock_time_t is an unsigned
   * 16 bit data type, time intervals of up to 524 seconds can be
   * measured.
   */
  //scount = count = 0;

  //sei ();
//}
void clock_init(void) 
{
	/*systick_disable();
	systick_attach_callback(systick_handler);
	systick_init(RATE);*/

	/*Configure the SysTick to have interrupt in 1ms time basis*/
	HAL_SYSTICK_Config(SystemCoreClock / 1000U);

	/*Configure the SysTick IRQ priority */
	//HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority, 0U);
}
/*---------------------------------------------------------------------------*/
clock_time_t
clock_time(void)
{
  clock_time_t tmp;
  do {
    tmp = count;
  } while(tmp != count);
  return tmp;
}
/*---------------------------------------------------------------------------*/
/**
 * Delay the CPU for a multiple of TODO
 */
void
clock_delay(unsigned int i)
{
  for (; i > 0; i--) {		/* Needs fixing XXX */
    unsigned j;
	for (j = 50; j > 0; j--) {}
      //asm volatile("nop");
  }
}

/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of 1 / 125 sec = 0.008 ms.
 *
 */
void
clock_wait(int i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
void
clock_set_seconds(unsigned long sec)
{
    // TODO
}

unsigned long
clock_seconds(void)
{
  unsigned long tmp;
  do {
    tmp = seconds;
  } while(tmp != seconds);
  return tmp;
}
