
#include "contiki.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
PROCESS(post_event_process, "post event process");
AUTOSTART_PROCESSES(&post_event_process,&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
	PROCESS_BEGIN();
	while (1) {
		PROCESS_WAIT_EVENT();
		Serial.println("event came");
	}
	PROCESS_END();
}
PROCESS_THREAD(post_event_process, ev, data)
{
	PROCESS_BEGIN();
	static process_event_t event_post;
	event_post = process_alloc_event();
	process_post(&hello_world_process,event_post,NULL);
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
