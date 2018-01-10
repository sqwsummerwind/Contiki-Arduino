
#include "contiki.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
PROCESS(print_process, "print process");
AUTOSTART_PROCESSES(&hello_world_process,&print_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
	PROCESS_BEGIN();
	while (1) {
		delay(1000);
		Serial.println("hello");
		digitalWrite(LED_BUILTIN, LOW);
		Serial.println("world");
	}
	PROCESS_END();
}
PROCESS_THREAD(print_process, ev, data)
{
	PROCESS_BEGIN();
	while (1) {
		delay(1000);
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
