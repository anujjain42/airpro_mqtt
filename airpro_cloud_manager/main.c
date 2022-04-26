#include "common.h"
#include "cm.h"

struct airpro_cm_handle cm;
int do_exit;

int airpro_send_recv_device_discovery(struct airpro_cm_handle *cm);
int airpro_publish_data(struct airpro_cm_handle *cm, char *buf, int len);

void airpro_recv_data_handler(struct airpro_cm_handle *cm, char *topic, char *payload, int payloadlen)
{
    printf("recv: topic=%s payloadlen=%d\n", topic, payloadlen);
}

void airpro_fetch_broker_details(struct airpro_cm_handle *cm)
{
    strcpy(cm->broker.ip_addr, "127.0.0.1");
    // cm->broker.port_num = 8520;
    cm->broker.port_num = 1883;
}

int main()
{
    struct airpro_sm *sm = &cm.sm;
    int delay_time = AIRPRO_CM_SM_WAIT_TIME_SECS_DEFAULT;
    int res = -1;

    pthread_mutex_init(&sm->lock, NULL); //header pthread for multithreading
    pthread_cond_init(&sm->cond, NULL);  //header pthread for multithreading

    airpro_fetch_broker_details(&cm);
    sm->state |= AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY;

    while (!do_exit) {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts); // header time or sys/time
        ts.tv_sec += delay_time;
        ts.tv_nsec = 0;
        pthread_mutex_lock(&sm->lock); //header pthread for multithreading
        pthread_cond_timedwait(&sm->cond, &sm->lock, &ts); //header pthread for multithreading
        pthread_mutex_unlock(&sm->lock); //header pthread for multithreading

        switch (sm->state) {
            case AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY: {
                res = airpro_send_recv_device_discovery(&cm);
                if (res != 0) {
                    sm->state |= AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY;
                    continue;
                }
                res = airpro_do_mqtt_init(&cm);
                if (res != 0) {
                    sm->state |= AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY;
                    continue;
                }
                sm->state |= AIRPRO_CM_SM_STATE_DEVICE_MANAGEMENT;
            } break;
            case AIRPRO_CM_SM_STATE_DEVICE_MANAGEMENT: {
                char buf[1024];
                int len = 0;

                airpro_publish_data(&cm, buf, len);
            } break;
        }
    }

    mosquitto_destroy(cm.mosq);
    mosquitto_lib_cleanup();
}
