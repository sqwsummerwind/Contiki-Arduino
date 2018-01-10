#include <string.h>
#include "contiki.h"
#include "apps/stm32/mqtt.h"
#include "board/stm32/ethernet-drv.h"
// inslude the SPI library:
#include <SPI.h>
//#include <ESP8266WiFi.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_mqtt_process, "stm32 mqtt example");
AUTOSTART_PROCESSES(&example_mqtt_process);
/*---------------------------------------------------------------------------*/
PROCINIT(&etimer_process, &tcpip_process, &resolv_process, &ethernet_process);
/*---------------------------------------------------------------------------*/
#define KEEP_ALIVE_INTERVAL 30	//keep alvie timer in 30 seconds
/*---------------------------------------------------------------------------*/
uip_ipaddr_t ipaddr;
//const char* password = "joker123";
char* mqtt_server = "119.29.10.36";
uint16_t server_port = 1883;
char *username = "sumas";
char *user_password = "summerwind";
char *pub_topic = "hello";
char *pub_payload = "suqiangwei";
char *sub_topic = "sumas";
String clientId;
/*---------------------------------------------------------------------------*/
static struct mqtt_connection conn;
//static WiFiClient espClient;
/*---------------------------------------------------------------------------*/
/* Maximum TCP segment size for outgoing segments of our socket */
#define MAX_TCP_SEGMENT_SIZE    32
/*---------------------------------------------------------------------------*/
void setup_uip() {

	delay(10);
	// We start by connecting to a ethernet network
	Serial.println();
	Serial.println("192.168.1.115");
	//set host ip address
	uip_ipaddr(&ipaddr, 192, 168, 1, 115);
	uip_sethostaddr(&ipaddr);

	/* set default router ip address */
	uip_ipaddr(&ipaddr, 192, 168, 1, 1);
	uip_setdraddr(&ipaddr);

	/* set network mask */
	uip_ipaddr(&ipaddr, 255, 255, 255, 0);
	uip_setnetmask(&ipaddr);

	Serial.println("");
	Serial.println("set uip successfully");
	Serial.println("ipaddr:192.168.1.115");
}
/*---------------------------------------------------------------------------*/
static void mqtt_event(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
	MDBG("MQTT - Call mqtt_event\n");
	struct mqtt_message *pub_message = NULL;
	switch (event) {
	case MQTT_EVENT_PUBLISH:
		pub_message = (struct mqtt_message *)data;
		MDBG("Pub Handler: topic= ");
		MDBG(" len= ");
		MDBG(strlen(pub_message->topic));
		MDBG(" chunk_len= ");
		MDBG(pub_message->payload_length);
		MDBG("\nMQTT - Received message: Topic: ");
		MDBG(pub_message->topic);
		MDBG(" Message: ");
		MDBG((char*)pub_message->payload_chunk);
		MDBG("\n");
		//mqtt_unsubscribe(conn,NULL,sub_topic);
		break;
	case MQTT_EVENT_UNSUBACK:
		//mqtt_disconnect(conn);
		break;
	}
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_mqtt_process, ev, data)
{
	PROCESS_BEGIN();
	// Create a random client ID
	clientId = "STM32Client-";
	clientId += String(random(0xffff), HEX);
	Serial.print("clientId:");
	Serial.println(clientId.c_str());

	mqtt_register(&conn, &example_mqtt_process, const_cast<char *>(clientId.c_str()), mqtt_event, MAX_TCP_SEGMENT_SIZE);
	mqtt_set_username_password(&conn, username, user_password);

	mqtt_connect(&conn, mqtt_server, server_port, KEEP_ALIVE_INTERVAL);
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(ev == MQTT_EVENT_CONNECTED);
		/*test level 0 and without mid*/
		//mqtt_publish(&conn, NULL, pub_topic, (uint8_t*)pub_payload, strlen(pub_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
		mqtt_subscribe(&conn, NULL, sub_topic, MQTT_QOS_LEVEL_0);
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PC12,OUTPUT);
	/* add setup code here */
	/* Clock */
	clock_init();
	Serial.begin(9600);
	SPI.begin();
	SPI.setDataMode(CS_PIN_CONTROLLED_BY_USER, SPI_MODE_0);
	/*setup wifi*/
	setup_uip();

	/* Process subsystem */
	process_init();

	/* Register initial processes */
	procinit_init();

	/*start etimer and ctimer*/
	ctimer_init();

	/* Autostart processes */
	autostart_start(autostart_processes);
}

void loop()
{
	/* add main program code here */
	process_run();
}
