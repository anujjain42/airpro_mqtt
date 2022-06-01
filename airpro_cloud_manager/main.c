#include "common.h"
#include "cm.h"

struct airpro_cm_handle cm;
int do_exit;

int airpro_send_recv_device_discovery(struct airpro_cm_handle *cm);
int airpro_publish_data(struct airpro_cm_handle *cm, char *buf, int len);
int airpro_do_mqtt_init(struct airpro_cm_handle *cm);
int airpro_fetch_device_info_type(struct airpro_cm_handle *cm, enum airpro_cmd_type cmd_type, char *data_buf, int *len);

void airpro_recv_data_handler(struct airpro_cm_handle *cm, char *topic, char *payload, int payloadlen)
{
    printf("recv: topic=%s payloadlen=%d\n", topic, payloadlen);
}

void airpro_fetch_broker_details(struct airpro_cm_handle *cm)
{
    strcpy(cm->broker.ip_addr, AIRPRO_MQTT_BROKER_IP);
    cm->broker.port_num = AIRPRO_MQTT_BROKER_PORT;
}

void airpro_fetch_serial_num(struct airpro_cm_handle *cm)
{
    strcpy(cm->dev.serial_num, AIRPRO_DUMMY_SERIAL_NUM);
    strcpy(cm->dev.ip_addr, AIRPRO_DUMMY_IP_ADDR);
    cm->dev.device_id = AIRPRO_DUMMY_DEVICE_ID;
}

int airpro_fetch_device_info(struct airpro_cm_handle *cm)
{
    char *ifname = AIRPRO_DEV_MAC_IFACE;
    struct ifreq ifr;
    int fd;
    
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, ifname, IFNAMSIZ-1);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
        close(fd);
        return -1;
    } else {
        memcpy(cm->dev.macaddr, ifr.ifr_hwaddr.sa_data, 6);
    }
    close(fd);
    airpro_fetch_serial_num(cm);

    return 0;
} 

int main()
{
    struct airpro_sm *sm = &cm.sm;
    int delay_time = AIRPRO_CM_SM_WAIT_TIME_SECS_DEFAULT;
    int res = -1;

    pthread_mutex_init(&sm->lock, NULL);
    pthread_cond_init(&sm->cond, NULL);

    airpro_fetch_broker_details(&cm);
    if (airpro_fetch_device_info(&cm) < 0) {
        printf("Fail to fetch device mac addr..\n");
        return -1;
    }
    sm->state |= AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY;

    while (!do_exit) {
        struct timespec ts;

        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += delay_time;
        ts.tv_nsec = 0;
        pthread_mutex_lock(&sm->lock);
        pthread_cond_timedwait(&sm->cond, &sm->lock, &ts);
        pthread_mutex_unlock(&sm->lock);

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
                char buf[4096];
                int len = 0;

                if (!airpro_fetch_device_info_type(&cm, AIRPRO_DEV_CMD_TYPE_SYSTEM, buf, &len)) {
                    airpro_publish_data(&cm, buf, len);
                }
                if (!airpro_fetch_device_info_type(&cm, AIRPRO_DEV_CMD_TYPE_NETWORK, buf, &len)) {
                    airpro_publish_data(&cm, buf, len);
                }
                if (!airpro_fetch_device_info_type(&cm, AIRPRO_DEV_CMD_TYPE_WIFI, buf, &len)) {
                    airpro_publish_data(&cm, buf, len);
                }
            } break;
        }
    }

    mosquitto_destroy(cm.mosq);
    mosquitto_lib_cleanup();
}
