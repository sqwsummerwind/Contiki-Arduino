
#include "contiki.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
#include <stdio.h>
#include "cfs-coffee-arch.h"
/*---------------------------------------------------------------------------*/
PROCESS(cfs_process, "Test CFS process");
AUTOSTART_PROCESSES(&cfs_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(cfs_process, ev, data)
{
	PROCESS_BEGIN();

	{
		int i, j, fd;
		int errors = 0;
		uint16_t filesize = 256*32;
		#define CHUNKSIZE 128

		cfs_remove("hej");
		fd = cfs_open("hej", CFS_WRITE);
		if (fd < 0) {
			Serial.print("could not open file for writing, aborting\n");
			errors++;
		}
		else {
			unsigned char buf[CHUNKSIZE];
			for (i = 0; i < filesize; i += CHUNKSIZE) {
				for (j = 0; j < CHUNKSIZE; ++j) {
					buf[j] = i + j;
				}
				if (cfs_write(fd, buf, CHUNKSIZE) < CHUNKSIZE) {
					Serial.printf("failed to write at offset %d, aborting\n", i);
					errors++;
					break;
				}
			}
			cfs_close(fd);
		}

		fd = cfs_open("hej", CFS_READ);
		if (fd < 0) {
			Serial.print("could not open file for reading, aborting\n");
			errors++;
		}
		else {
			for (i = 0; i < filesize; ++i) {
				unsigned char buf;
				if (cfs_read(fd, &buf, 1) < 1) {
					Serial.printf("failed to read at offset %d, aborting\n", i);
					errors++;
					break;
				}
				if (buf != (i & 0xff)) {
					errors++;
					Serial.printf("error: diff at %d, %d != %d\n", i, i & 0xff, buf);
				}
			}
		}
		Serial.printf("CFS test 1 completed with %d errors\n", errors);
		cfs_remove("hej");
		errors = 0;

		fd = cfs_open("hej", CFS_WRITE);
		if (fd < 0) {
			Serial.print("could not open file for writing, aborting\n");
		}
		else {
			unsigned char buf[CHUNKSIZE];
			for (i = 0; i < filesize; i += CHUNKSIZE) {
				for (j = 0; j < CHUNKSIZE; ++j) {
					buf[j] = i + j + 1;
				}
				if (cfs_write(fd, buf, CHUNKSIZE) < CHUNKSIZE) {
					Serial.printf("failed to write at offset %d, aborting\n", i);
					errors++;
					break;
				}
			}
			cfs_close(fd);
		}

		fd = cfs_open("hej", CFS_READ);
		if (fd < 0) {
			Serial.print("could not open file for reading, aborting\n");
		}
		else {
			for (i = 0; i < filesize; ++i) {
				unsigned char buf;
				if (cfs_read(fd, &buf, 1) < 1) {
					Serial.printf("failed to read at offset %d, aborting\n", i);
					errors++;
					break;
				}
				if (buf != ((i + 1) & 0xff)) {
					errors++;
					Serial.printf("error: diff at %d, %d != %d\n", i, (i + 1) & 0xff, buf);
				}
			}
		}
		Serial.printf("CFS test 2 completed with %d errors\n", errors);
	}

	PROCESS_END();
}
void test() {
	cfs_coffee_format();
	int a = 10;
	COFFEE_WRITE(&a,sizeof(a),0);
	a = 1;
	COFFEE_READ(&a,sizeof(a),0);
	Serial.printf("a=%i\n",a);
	a = 9;
	COFFEE_WRITE(&a, sizeof(a), 0);
	a = 1;
	COFFEE_READ(&a, sizeof(a), 0);
	Serial.printf("a=%i\n", a);
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
	cfs_coffee_format();
	/* Autostart processes */
	autostart_start(autostart_processes);
	//test();
}

void loop()
{
  /* add main program code here */
	process_run();
}


