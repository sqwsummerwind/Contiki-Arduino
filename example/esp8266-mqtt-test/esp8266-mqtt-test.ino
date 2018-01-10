#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "apps/mqtt.h"
#include <ESP8266WiFi.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_mqtt_process, "Coffee example");
AUTOSTART_PROCESSES(&example_mqtt_process);
/*---------------------------------------------------------------------------*/
#define KEEP_ALIVE_INTERVAL 30	//keep alvie timer in 15 seconds
/*---------------------------------------------------------------------------*/
const char* ssid = "gangoffour";
const char* password = "joker123";
char* mqtt_server = "www.sumaxi.com";
uint16_t server_port = 1883;
char *username = "sumas";
char *user_password = "summerwind";
char *pub_topic = "hello";
char *pub_payload = "suqiangwei";
char *sub_topic = "sumas";
String clientId;
/*---------------------------------------------------------------------------*/
static struct mqtt_connection conn;
static WiFiClient espClient;
/*---------------------------------------------------------------------------*/
void setup_wifi() {

	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	//randomSeed(micros());

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}
/*---------------------------------------------------------------------------*/
static void mqtt_event(struct mqtt_connection *conn, mqtt_event_t event, void *data) {
	DBG("MQTT - Call mqtt_event\n");
	struct mqtt_message *pub_message = NULL;
	switch (event) {
	case MQTT_EVENT_PUBLISH:
		pub_message = (struct mqtt_message *)data;
		DBG("Pub Handler: topic='%s' (len=%u), chunk_len=%u\n", 
			pub_message->topic, strlen(pub_message->topic),pub_message->payload_length);
		DBG("MQTT - Received message: Topic:%s, message:%s\n",pub_message->topic,pub_message->payload_chunk);
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
	clientId = "ESP8266Client-";
	clientId += String(random(0xffff), HEX);
	Serial.printf("clientId:%s\n",clientId.c_str());

	mqtt_register(&conn,&example_mqtt_process,const_cast<char *>(clientId.c_str()), mqtt_event,&espClient);
	mqtt_set_username_password(&conn,username, user_password);

	mqtt_connect(&conn, mqtt_server, server_port, KEEP_ALIVE_INTERVAL,true);
	while (1) {
		PROCESS_WAIT_EVENT_UNTIL(ev== MQTT_EVENT_CONNECTED);
		/*test level 0 and without mid*/
		//mqtt_publish(&conn, NULL, pub_topic, (uint8_t*)pub_payload, strlen(pub_payload), MQTT_QOS_LEVEL_0, MQTT_RETAIN_OFF);
		mqtt_subscribe(&conn,NULL,sub_topic, MQTT_QOS_LEVEL_0);
	}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void test() {
	
}
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	  /* add setup code here */
	
	/* Clock */
	clock_init();
	Serial.begin(9600);
	/*setup wifi*/
	setup_wifi();

	/* Process subsystem */
	process_init();

	/* Register initial processes */
	//procinit_init();

	/*start etimer and ctimer*/
	process_start(&etimer_process, NULL);
	ctimer_init();

	/* Autostart processes */
	autostart_start(autostart_processes);
	
	//test();
}

void loop()
{
  /* add main program code here */
	process_run();
}


