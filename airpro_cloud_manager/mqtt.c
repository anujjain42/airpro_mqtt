#include "common.h"
#include "cm.h"

void airpro_recv_data_handler(struct airpro_cm_handle *cm, char *topic, char *payload, int payloadlen);

void airpro_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    struct airpro_cm_handle *cm = (struct airpro_cm_handle *)obj;
    printf("message_callback\n");
    if (msg->payloadlen) {
        cm->airpro_sub_handler(cm, msg->topic, msg->payload, msg->payloadlen);
    }
    else {
        printf("%s (null)\n", msg->topic);
    }
}

void airpro_connect_callback(struct mosquitto *mosq, void *obj, int rc)
{
    struct airpro_cm_handle *cm = (struct airpro_cm_handle *)obj;
    char subtopic[128];
    int payloadlen = 0;
    printf("airpro_connect_callback\n");
    if (rc) {
        printf("Error with result code: %d\n", rc);
        return;
    }
    
    memset(subtopic, 0, sizeof(subtopic));
    sprintf(subtopic, "airpro/device_%02x:%02x:%02x:%02x:%02x:%02x", cm->dev.macaddr[0], cm->dev.macaddr[1],
          cm->dev.macaddr[2], cm->dev.macaddr[3], cm->dev.macaddr[4], cm->dev.macaddr[5]);
    printf("Anjan: subtopic=%s\n", subtopic);
    mosquitto_subscribe(mosq, NULL, subtopic, 0);
}

int airpro_publish_data(struct airpro_cm_handle *cm, char *buf, int len)
{
    char topic_name[128];

    memset(topic_name, 0, sizeof(topic_name));
    sprintf(topic_name, "airpro/dev/to/cloud");
    printf("publishing\n");
    printf("Client Publishing: topic=%s len=%d msg=%s\n", topic_name, len, buf);
    mosquitto_publish(cm->mosq, NULL, topic_name, len, buf, 0, false);
    //mosquitto_loop_forever(cm->mosq, -1, 1);
}

int airpro_do_mqtt_init(struct airpro_cm_handle *cm)
{
    int rc = -1;

    mosquitto_lib_init();
    cm->airpro_sub_handler = airpro_recv_data_handler;
    cm->mosq = mosquitto_new(NULL, true, cm);
    mosquitto_connect_callback_set(cm->mosq, airpro_connect_callback);
    mosquitto_message_callback_set(cm->mosq, airpro_message_callback);
    while (1) {
        struct timeval tv;
        rc = mosquitto_connect(cm->mosq, cm->broker.ip_addr, cm->broker.port_num, 60);
        
        if (rc == MOSQ_ERR_SUCCESS) {
            printf("Connection Ok\n");
            break;
        }
        printf("COULD NOT CONNECT: %d\n", rc);
        tv.tv_sec = 2;
        tv.tv_usec = 0;
        select(0, NULL, NULL, NULL, &tv);
    }

    rc = mosquitto_loop_start(cm->mosq);

    return 0;
}

