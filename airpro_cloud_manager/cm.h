#ifndef __AIRPRO_CM_H__
#define __AIRPRO_CM_H_

#include <mosquitto.h>

#define AIRPRO_CM_SM_STATE_DEVICE_DISCOVERY 1
#define AIRPRO_CM_SM_STATE_DEVICE_MANAGEMENT 2

#define AIRPRO_CM_SM_WAIT_TIME_SECS_DEFAULT 5

struct mosq_config {
    char *id;
    char *id_prefix;
    int protocol_version;
    int keepalive;
    char *host;
    int port;
    int qos;
    bool retain;
    int pub_mode; /* pub */
    char *file_input; /* pub */
    long msglen; /* pub */
    char *topic; /* pub */
    char *bind_address;
};

struct airpro_sm {
    int state;
    pthread_mutex_t lock;
    pthread_cond_t cond;
};

struct http_device_discovery {
    char http_msg[1024];
    int http_msg_len;
};

struct device_info {
    unsigned char macaddr[6];
    char ip_addr[64];
    char serial_num[64];
};

struct airpro_broker {
    int port_num;
    char ip_addr[64];
};

struct airpro_cm_handle {
    struct mosq_config cfg;
    struct airpro_sm sm;
    struct http_device_discovery hdd;
    struct airpro_broker broker;
    struct device_info dev;
    struct mosquitto *mosq;
    void (*airpro_sub_handler)(struct airpro_cm_handle *cm, char *topic, char *payload, int payloadlen);
};

#endif
