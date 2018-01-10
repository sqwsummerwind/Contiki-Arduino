
#include "contiki.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
static process_event_t event_ready;
PROCESS(print_summer_process, "print summer process");
PROCESS(print_wind_process, "print wind process");
AUTOSTART_PROCESSES(&etimer_process,&print_summer_process,&print_wind_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(print_summer_process, ev, data)
{
	PROCESS_BEGIN();
	Serial.println("summer process start");
	static struct etimer timer;
	etimer_set(&timer,CLOCK_SECOND);
	event_ready = process_alloc_event();
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(ev==PROCESS_EVENT_TIMER);
		Serial.println("summer");
		Serial.println(clock_time());
		process_post(&print_wind_process,event_ready,NULL);
		etimer_reset(&timer);
	}
	PROCESS_END();
}
PROCESS_THREAD(print_wind_process, ev, data)
{
	PROCESS_BEGIN();
	Serial.println("wind process start");
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(ev==event_ready);
		Serial.println("wind");
		Serial.println(clock_time());
	}
	
	PROCESS_END();
}
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	  /* add setup code here */
	  /* Clock */
	clock_init();
	Serial.begin(9600);

	/* Process subsystem */
	process_init();

	/* Register initial processes */
	//procinit_init();

	/* Autostart processes */
	autostart_start(autostart_processes);

}

void loop()
{
  /* add main program code here */
	process_run();
}
