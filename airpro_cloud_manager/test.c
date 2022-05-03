#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mosquitto.h>

void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    printf("ID: %d\n", * (int *) obj);
    if (rc) {
        printf("Error with result code: %d\n", rc);
        exit(-1);
    }
    mosquitto_subscribe(mosq, NULL, "airpro/cloud_to_dev", 0);
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    printf("New message with topic %s: %s\n", msg->topic, (char *) msg->payload);
}

void airpro_sub_thread(void *arg)
{
    struct mosquitto *r_mosq;
    int r_rc, id = 12;

    r_mosq = mosquitto_new("airpro-sub", true, &id);
    mosquitto_connect_callback_set(r_mosq, on_connect);
    mosquitto_message_callback_set(r_mosq, on_message);
    r_rc = mosquitto_connect(r_mosq, "127.0.0.1", 1883, 10);
    if (r_rc) {
        printf("Could not connect to Broker with return code %d\n", r_rc);
        return;
    }
    mosquitto_loop_start(r_mosq);

}

int main()
{
    int s_rc;
    struct mosquitto *s_mosq;
    pthread_t sub_threadid;
    mosquitto_lib_init();

    s_mosq = mosquitto_new("airpro-pub", true, NULL);
    s_rc = mosquitto_connect(s_mosq, "127.0.0.1", 1883, 60);
    if (s_rc != 0) {
        printf("Client could not connect to broker! Error Code: %d\n", s_rc);
        mosquitto_destroy(s_mosq);
        return -1;
    }
    pthread_create(&sub_threadid, NULL, airpro_sub_thread, NULL);
    while (1) {
        printf("Anjan: send data..\n");
        mosquitto_publish(s_mosq, NULL, "airpro/dev/to/cloud", 6, "Hello", 0, false);
        sleep(5);
    }

    //mosquitto_disconnect(mosq);
    //mosquitto_destroy(mosq);
    //mosquitto_lib_cleanup();
    return 0;
}