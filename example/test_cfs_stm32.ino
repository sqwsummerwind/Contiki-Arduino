
#include "contiki.h"
#include "cfs/cfs.h"
#include "cfs/cfs-coffee.h"
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
		uint16_t filesize = 256 * 32;
#define CHUNKSIZE 128

		cfs_remove("hej");
		fd = cfs_open("hej", CFS_WRITE);
		if (fd < 0) {
			Serial.println("could not open file for writing, aborting\n");
			errors++;
		}
		else {
			unsigned char buf[CHUNKSIZE];
			for (i = 0; i < filesize; i += CHUNKSIZE) {
				for (j = 0; j < CHUNKSIZE; ++j) {
					buf[j] = i + j;
				}
				if (cfs_write(fd, buf, CHUNKSIZE) < CHUNKSIZE) {
					Serial.println("failed to write at offset ");
					Serial.print(i);
					Serial.print(", aborting\n");
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
					Serial.print("failed to read at offset ");
					Serial.print(i);
					Serial.print(", aborting\n");
					errors++;
					break;
				}
				if (buf != (i & 0xff)) {
					errors++;
					Serial.print("error: diff at ");
					Serial.print(i);
					Serial.print(", ");
					Serial.print(i & 0xff);
					Serial.print(" != ");
					Serial.print(buf);
					Serial.print("\n");
				}
			}
		}
		Serial.print("CFS test 1 completed with ");
		Serial.print(errors);
		Serial.print(" errors\n");

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
					Serial.println("failed to write at offset ");
					Serial.print(i);
					Serial.print(", aborting\n");
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
					Serial.println("failed to write at offset ");
					Serial.print(i);
					Serial.print(", aborting\n");
					errors++;
					break;
				}
				if (buf != ((i + 1) & 0xff)) {
					errors++;
					Serial.print("error: diff at ");
					Serial.print(i);
					Serial.print(", ");
					Serial.print(i & 0xff);
					Serial.print(" != ");
					Serial.print(buf);
					Serial.print("\n");
				}
			}
		}
		Serial.print("CFS test 2 completed with ");
		Serial.print(errors);
		Serial.print(" errors\n");
	}

	PROCESS_END();
}
void test() {
	cfs_coffee_format();
	int a = 10;
	COFFEE_WRITE(&a, sizeof(a), 4);
	a = 1;
	COFFEE_READ(&a, sizeof(a), 4);
	Serial.print("a = ");
	Serial.println(a);
	Serial.println("done!");
	
}
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	/* add setup code here */
	Serial.begin(9600);

	/* Clock */
	clock_init();

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

