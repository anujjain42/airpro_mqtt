#include "common.h"
#include "cm.h"
#include <json-c/json.h>
#include "airpro_mgr_msg.h"

enum {
    JSON_TYPE_NULL = 0,
    JSON_TYPE_STRING,
    JSON_TYPE_BOOL,
    JSON_TYPE_INT,
    JSON_TYPE_INT8,
    JSON_TYPE_INT16,
    JSON_TYPE_INT32,
    JSON_TYPE_INT64,
    JSON_TYPE_UINT,
    JSON_TYPE_UINT8,
    JSON_TYPE_UINT16,
    JSON_TYPE_UINT32,
    JSON_TYPE_UINT64,
    JSON_TYPE_DOUBLE,
};

static int map_mac_to_string(char *str, unsigned len, void *mac)
{
    int count = -1;
    uint8_t *m8 = mac;

    if ((str != NULL) && (len != 0) && (mac != NULL)) {
        str[len - 1] = 0;
        count = snprintf(str, (len - 1), "%02x:%02x:%02x:%02x:%02x:%02x",
            (m8[0]), (m8[1]), (m8[2]), (m8[3]), (m8[4]), (m8[5])
        );
    }
    return count;
}

static void app_json_free(void *jsonbuf)
{
    json_object *jobj = (json_object *)jsonbuf;

    if (jobj) {
        json_object_put(jobj);
    }
}

static const char *app_json_stringify(void *jsonbuf)
{
    json_object *jobj = (json_object *)jsonbuf;
    const char  *ret;

    if (jobj) {
        ret = json_object_to_json_string( jobj );
        return( ret );
    }

    return NULL;
}

static int map_value_to_json_field(json_object *jobj, int field_type, const char *field_name, void *value)
{   
    json_object *child = NULL;
    int ret = 0;

    switch( field_type ) { 
    case JSON_TYPE_STRING:
        child = json_object_new_string( (char *)value );
        break;
    case JSON_TYPE_BOOL:
        child = json_object_new_boolean( *((int *)value) );
        break;
    case JSON_TYPE_INT:
        child = json_object_new_int( *((int *)value) );
        break;
    case JSON_TYPE_INT8:
        child = json_object_new_int( *((int8_t *)value) );
        break;
    case JSON_TYPE_INT16:
        child = json_object_new_int( *((int16_t *)value) );
        break;
    case JSON_TYPE_INT32:
        child = json_object_new_int( *((int32_t *)value) );
        break;
    case JSON_TYPE_INT64:
        child = json_object_new_int64( *((int64_t *)value) );
        break;
    case JSON_TYPE_UINT:
        child = json_object_new_int( *((unsigned int *)value) );
        break;
    case JSON_TYPE_UINT8:
        child = json_object_new_int( *((uint8_t *)value) );
        break;
    case JSON_TYPE_UINT16:
        child = json_object_new_int( *((uint16_t *)value) );
        break;
    case JSON_TYPE_UINT32:
        child = json_object_new_int( *((uint32_t *)value) );
        break;
    case JSON_TYPE_UINT64:
        child = json_object_new_int64( *((uint64_t *)value) );
        break;
    case JSON_TYPE_DOUBLE:
        child = json_object_new_double( *((double *)value) );
        break;
    default:
        break;
    }
    if (child == NULL) {
    } else {
        json_object_object_add(jobj, field_name, child);
        ret = 1;
    }
    return ret;
}

void airpro_prepare_get_system_stats_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                                                          struct airpro_system_event_type_stats *msg)
{
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "cpu_usage", &msg->cpu_usage) == 0) {
        goto end;
    }

    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "max_memory", &msg->max_memory) == 0) {
        goto end;
    }

    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "max_memory_used", &msg->max_memory_used) == 0) {
        goto end;
    }

    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "max_disk_space", &msg->max_disk_space) == 0) {
        goto end;
    }

    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "max_disk_space_used", &msg->max_disk_space_used) == 0) {
        goto end;
    }

    if (map_value_to_json_field(jobj, JSON_TYPE_UINT32, "uptime", &msg->uptime) == 0) {
        goto end;
    }

    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }

    return;
}

void airpro_prepare_get_all_vlan_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype, struct airpro_get_all_vlan_entry *msg)
{
    json_object *jobj2 = NULL, *jobj3 = NULL;
    int i;

    jobj2 = json_object_new_array();
    if (!jobj2) {
        goto end;
    }
    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }

    for (i = 0; i < msg->num_entry; i++) {
        jobj3 = json_object_new_object();
        if (!jobj3) {
            goto end;
        }
        json_object *child = NULL;

        child = json_object_new_string(msg->vlan[i].vlan_name);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "vlan_name", child);

        child = json_object_new_int(msg->vlan[i].vlan_id);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "vlan_id", child);

        child = json_object_new_int(msg->vlan[i].ip_source);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_source", child);

        child = json_object_new_string(msg->vlan[i].ip_addr);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_addr", child);

        child = json_object_new_string(msg->vlan[i].ip_netmask);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_netmask", child);

        json_object_array_add(jobj2, jobj3);
    }
    json_object_object_add(jobj, "vlan_list", jobj2);

    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
    if (jobj2) {
        json_object_put(jobj2);
    }
    return;
}

void airpro_prepare_get_all_static_lease_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                                                           struct airpro_get_all_static_lease_entry *msg)
{
    json_object *jobj2 = NULL, *jobj3 = NULL;
    int i;

    jobj2 = json_object_new_array();
    if (!jobj2) {
        goto end;
    }
    for (i = 0; i < msg->num_entry; i++) {
        jobj3 = json_object_new_object();
        if (!jobj3) {
            goto end;
        }
        json_object *child = NULL;

        child = json_object_new_string(msg->static_lease[i].client_name);
        if (child == NULL) {
            goto end;
        }

        json_object_object_add(jobj3, "client_name", child);

        child = json_object_new_string(msg->static_lease[i].ip_client);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_client", child);

        child = json_object_new_string(msg->static_lease[i].ip_netmask);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_netmask", child);

        child = json_object_new_string(msg->static_lease[i].client_mac);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "client_mac", child);

        child = json_object_new_string(msg->static_lease[i].ip_gw);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_gw", child);

        child = json_object_new_string(msg->static_lease[i].ip_dns);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "ip_dns", child);
        json_object_array_add(jobj2, jobj3);
    }
    json_object_object_add(jobj, "static_lease_list", jobj2);
    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
    if (jobj2) {
        json_object_put(jobj2);
    }
}

void *airpro_prepare_get_nat_mode_config_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                                                                      struct airpro_nat_mode_config *msg)
{
    if (msg->ip_type == AIRPRO_NAT_MODE_IP_TYPE_STATIC) {
        struct nat_mode_static_params *static_var = &msg->u.static_mode_params;

        if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "proto", "static") == 0) {
            goto end;
        }
        if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "ip_addr", static_var->ip_addr) == 0) {
            goto end;
        }
        if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "ip_netmask", static_var->ip_netmask) == 0) {
            goto end;
        }
        if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "def_gw", static_var->def_gw) == 0) {
            goto end;
        }
        if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "dns", static_var->dns) == 0) {
            goto end;
        }
    }
    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
}

void *airpro_prepare_get_network_mode_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype, struct airpro_network_mode *msg)
{
    if (map_value_to_json_field(jobj, JSON_TYPE_INT32, "net_mode", &msg->net_mode) == 0) {
        goto end;
    }
    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
    return;
}

void airpro_prepare_get_channel_scan_results_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                              struct airpro_wifimgr_event_type_get_chan_scan_results *msg)
{
    char tmpbuf[64];
    json_object *jobj2 = NULL, *jobj3 = NULL, *jobj4 = NULL, *jobj5 = NULL;
    int i, j;

    jobj2 = json_object_new_array();
    if (!jobj2) {
        goto end;
    }

    printf("Anjan: nchan=%d\n", msg->nchans);
    for (i = 0; i < msg->nchans; i++) {
        jobj3 = json_object_new_object();
        if (!jobj3) {
            goto end;
        }
        json_object *child = NULL;

        child = json_object_new_int(msg->scan_results[i].chan);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "chan", child);

        child = json_object_new_int(msg->scan_results[i].chan_load);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "chan_load", child);

        child = json_object_new_int(msg->scan_results[i].nbss);
        if (child == NULL) {
            goto end;
        }
        json_object_object_add(jobj3, "nbss", child);

        jobj4 = json_object_new_array();
        if (!jobj4) {
            goto end;
        }
        for (j = 0; j < msg->scan_results[i].nbss; j++) {
            jobj5 = json_object_new_object();
            if (!jobj5) {
                goto end;
            }
            json_object *child = NULL;

            memset(tmpbuf, 0, sizeof(tmpbuf));
            map_mac_to_string(tmpbuf, sizeof(tmpbuf), (msg->scan_results[i].neighbor_list[j].bssid));
            child = json_object_new_string(tmpbuf);
            if (child == NULL) {
                goto end;
            }
            json_object_object_add(jobj5, "bssid", child);

            child = json_object_new_string(msg->scan_results[i].neighbor_list[j].ssid);
            if (child == NULL) {
                goto end;
            }
            json_object_object_add(jobj5, "ssid", child);

            child = json_object_new_int(msg->scan_results[i].neighbor_list[j].rssi);
            if (child == NULL) {
                goto end;
            }
            json_object_object_add(jobj5, "rssi", child);
            json_object_object_add(jobj4, "nbss", jobj5);
        }
        json_object_object_add(jobj3, "channel", jobj4);
    }

    json_object_object_add(jobj, "scan_result", jobj2);
    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
    if (jobj2) {
        json_object_put(jobj2);
    }
}

void *airpro_prepare_get_active_network_config_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                                                                struct airpro_active_network_config *msg)
{
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "network_mode", msg->network_mode) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "wan_interface", msg->wan_interface) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "ip_addr", msg->ip_addr) == 0) {
        goto end;
    }
    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }
    return;
}

void airpro_prepare_get_system_brd_fw_json_buf(json_object *jobj, uint8_t comptype, uint16_t msgtype,
                                                    struct airpro_system_event_type_brd_fw_info *msg)
{
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "board_info", &msg->board_info) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "fw_info", &msg->fw_info) == 0) {
        goto end;
    }

    return;
end:
    if (jobj) {
        json_object_put(jobj);
    }

    return;
}

int airpro_parse_system_config_event_json(json_object *jobj, uint8_t comptype, struct airpro_mgr_data *msg)
{
    const char *buf = NULL;
    void *jsonbuf = NULL;

    switch (msg->type) {
        case AIRPRO_SYSTEM_EVENT_GET_SYSTEM_STATS: {
            airpro_prepare_get_system_stats_json_buf(jobj, comptype, AIRPRO_SYSTEM_EVENT_GET_SYSTEM_STATS, msg->data);
        } break;
        case AIRPRO_SYSTEM_EVENT_GET_BRD_FW_INFO: {
            airpro_prepare_get_system_brd_fw_json_buf(jobj, comptype, AIRPRO_SYSTEM_EVENT_GET_BRD_FW_INFO, msg->data);
        } break;
    }
}

int airpro_mgr_parse_wifimgr_event_json(json_object *jobj, uint8_t comptype, struct airpro_mgr_data *msg)
{
    const char *buf = NULL;
    void *jsonbuf = NULL;

    switch (msg->type) {
        case AIRPRO_WIFI_MGR_EVENT_GET_CHANNEL_SCAN_RESULTS: {
            airpro_prepare_get_channel_scan_results_json_buf(jobj, comptype, AIRPRO_WIFI_MGR_EVENT_GET_CHANNEL_SCAN_RESULTS, msg->data);
        } break;
    }
}
    
int airpro_parse_network_config_event_json(json_object *jobj, uint8_t comptype, struct airpro_mgr_data *msg)
{   
    const char *buf = NULL;
    void *jsonbuf = NULL;

    switch (msg->type) {
        case AIRPRO_NETWORK_EVENT_GET_ACTIVE_NETWORK_CONFIG: {
            airpro_prepare_get_active_network_config_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_ACTIVE_NETWORK_CONFIG, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_NETWORK_MODE: {
            airpro_prepare_get_network_mode_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_NETWORK_MODE, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_NAT_MODE_CONFIG: {
            airpro_prepare_get_nat_mode_config_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_NAT_MODE_CONFIG, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_LEASE: {
            airpro_prepare_get_all_static_lease_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_LEASE, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_ALL_VLAN: {
            airpro_prepare_get_all_vlan_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_ALL_VLAN, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_ROUTE: {
            //TODO:
            //airpro_prepare_get_all_static_route_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_ROUTE, msg->data);
        } break;
        case AIRPRO_NETWORK_EVENT_GET_ALL_DHCP_LEASE: {
            //TODO:
            //airpro_prepare_get_all_dhcp_lease_json_buf(jobj, comptype, AIRPRO_NETWORK_EVENT_GET_ALL_DHCP_LEASE, msg->data);
        } break;
    }
}
    
int airpro_lib_mgr_parse_event_json(json_object *jobj, char *data)
{    
    struct airpro_mgr_data *msg = (struct airpro_mgr_data *)data;

    if (msg->hdr.component_type == COMPONENT_TYPE_WIFI_MANAGER) {
       airpro_mgr_parse_wifimgr_event_json(jobj, COMPONENT_TYPE_WIFI_MANAGER, msg);
    } else if (msg->hdr.component_type == COMPONENT_TYPE_SYSTEM_INFO) {
       airpro_parse_system_config_event_json(jobj, COMPONENT_TYPE_SYSTEM_INFO, msg);
    } else if (msg->hdr.component_type == COMPONENT_TYPE_NETWORK_INFO) {
       airpro_parse_network_config_event_json(jobj, COMPONENT_TYPE_NETWORK_INFO, msg);
    }
    
    return 0;
}

void airpro_get_nat_config(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NAT_MODE_CONFIG;

    airpro_lib_send_cmd_network_msg(mgr, sizeof(struct airpro_mgr_data), reply, sizeof(reply_buf));
    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }

    if (mgr) {
        free(mgr);
    }

}

void airpro_get_wifi_chan_scan_res(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data) +
                        sizeof(struct airpro_mgr_iface_idx));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_WIFI_MGR(hdr);
    mgr->len = sizeof(struct airpro_mgr_iface_idx);
    mgr->type = AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_CHANNEL_SCAN_RESULTS;
    struct airpro_mgr_iface_idx *rid = (struct airpro_mgr_iface_idx *)mgr->data;

    rid->radio_id = 0;
    reply = reply_buf;
    airpro_lib_send_wifi_mgr_msg(mgr,
                        (sizeof(struct airpro_mgr_data) + sizeof(struct airpro_mgr_iface_idx)),
                        reply, sizeof(reply_buf));
    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }
    if (mgr) {
        free(mgr);
    }
}

void airpro_get_network_mode(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NETWORK_MODE;

    airpro_lib_send_cmd_network_msg(mgr, sizeof(struct airpro_mgr_data), reply, sizeof(reply_buf));

    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }
    if (mgr) {
        free(mgr);
    }
}

void airpro_get_static_lease(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_STATIC_LEASE;

    airpro_lib_send_cmd_network_msg(mgr, sizeof(struct airpro_mgr_data), reply, sizeof(reply_buf));
    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }
    if (mgr) {
        free(mgr);
    }

}

void airpro_get_active_network_config(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ACTIVE_NETWORK_CONFIG;

    airpro_lib_send_cmd_network_msg(mgr, sizeof(struct airpro_mgr_data), reply, sizeof(reply_buf));

    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }
    if (mgr) {
        free(mgr);
    }

}

void airpro_get_system_stats(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_SYSTEM_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_SYSTEM_STATS;

    airpro_lib_send_cmd_system_msg(mgr, sizeof(struct airpro_mgr_data),
                                                       reply, sizeof(reply_buf));
    airpro_lib_mgr_parse_event_json(jobj, reply);
    if (mgr) {
        free(mgr);
    }
}

void airpro_get_vlan_config(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_VLAN;

    airpro_lib_send_cmd_network_msg(mgr, sizeof(struct airpro_mgr_data), reply, sizeof(reply_buf));
    if (reply) {
        airpro_lib_mgr_parse_event_json(jobj, reply);
    }
    if (mgr) {
        free(mgr);
    }

}

void airpro_get_brd_fw_version(json_object *jobj)
{
    struct airpro_mgr_data *mgr = malloc(sizeof(struct airpro_mgr_data));
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    char reply_buf[2048];
    char *reply = NULL;

    memset(mgr, 0, sizeof(struct airpro_mgr_data));
    AIRPRO_SET_COMPONENT_TYPE_SYSTEM_INFO(hdr);
    reply = reply_buf;
    mgr->type = AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_BRD_FW_INFO;

    airpro_lib_send_cmd_system_msg(mgr, sizeof(struct airpro_mgr_data),
                                                       reply, sizeof(reply_buf));
    airpro_lib_mgr_parse_event_json(jobj, reply);
    if (mgr) {
        free(mgr);
    }
}

static void *map_wifi_info_to_json(struct airpro_cm_handle *cm)
{
    json_object *jobj  = NULL;
    json_object *jobj2 = NULL;
    json_object *jobj3 = NULL;
    char tmpbuf[64];

    jobj = json_object_new_object();
    if (!jobj) {
        goto end;
    }

    jobj2 = json_object_new_object();
    if (!jobj2) {
        goto end;
    }
    int msg_type = 1003;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "type", &msg_type) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "flag", "REQ") == 0) {
        goto end;
    }
    double ver = 2.5;
    if (map_value_to_json_field(jobj, JSON_TYPE_DOUBLE, "ver", &ver) == 0) {
        goto end;
    }
    int ts = 12456662;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "timestamp", &ts) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "uuid", "3b85d2aa-1dd2-11b2-9874-99a311b3456") == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "token", "99a311b34hdt5666dddd56") == 0) {
        goto end;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    map_mac_to_string(tmpbuf, sizeof(tmpbuf), (cm->dev.macaddr));
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "macaddr", tmpbuf) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "serial_num", cm->dev.serial_num) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "device_id", &cm->dev.device_id) == 0) {
        goto end;
    }
    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }

    //airpro_get_wifi_chan_scan_res(jobj3);
    json_object_object_add(jobj2, "channel_scan_res", jobj3);

    json_object_object_add(jobj, "data", jobj2);

    return (void *)jobj;
end:
    if (jobj2) {
        json_object_put(jobj2);
    }
    if (jobj) {
        json_object_put(jobj);
    }
    return NULL;
}

static void *map_network_info_to_json(struct airpro_cm_handle *cm)
{
    json_object *jobj  = NULL;
    json_object *jobj2 = NULL;
    json_object *jobj3 = NULL;
    char tmpbuf[64];

    jobj = json_object_new_object();
    if (!jobj) {
        goto end;
    }

    jobj2 = json_object_new_object();
    if (!jobj2) {
        goto end;
    }

    int msg_type = 1001;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "type", &msg_type) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "flag", "REQ") == 0) {
        goto end;
    }
    double ver = 2.5;
    if (map_value_to_json_field(jobj, JSON_TYPE_DOUBLE, "ver", &ver) == 0) {
        goto end;
    }
    int ts = 12456662;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "timestamp", &ts) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "uuid", "3b85d2aa-1dd2-11b2-9874-99a311b3456") == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "token", "99a311b34hdt5666dddd56") == 0) {
        goto end;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    map_mac_to_string(tmpbuf, sizeof(tmpbuf), (cm->dev.macaddr));
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "macaddr", tmpbuf) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "serial_num", cm->dev.serial_num) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "device_id", &cm->dev.device_id) == 0) {
        goto end;
    }
    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_network_mode(jobj3);
    json_object_object_add(jobj2, "network_mode", jobj3);

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_nat_config(jobj3);
    json_object_object_add(jobj2, "nat_config", jobj3);

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_active_network_config(jobj3);
    json_object_object_add(jobj2, "active_network_config", jobj3);

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_static_lease(jobj3);
    json_object_object_add(jobj2, "static_lease", jobj3);

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_vlan_config(jobj3);
    json_object_object_add(jobj2, "vlan", jobj3);

    json_object_object_add(jobj, "data", jobj2);

    return (void *)jobj;
end:
    if (jobj2) {
        json_object_put(jobj2);
    }
    if (jobj) {
        json_object_put(jobj);
    }
    return NULL;
}

static void *map_system_info_to_json(struct airpro_cm_handle *cm)
{
    json_object *jobj  = NULL;
    json_object *jobj2 = NULL;
    json_object *jobj3 = NULL;
    char tmpbuf[64];

    jobj = json_object_new_object();
    if (!jobj) {
        goto end;
    }

    jobj2 = json_object_new_object();
    if (!jobj2) {
        goto end;
    }

    int msg_type = 1000;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "type", &msg_type) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "flag", "REQ") == 0) {
        goto end;
    }
    double ver = 2.5;
    if (map_value_to_json_field(jobj, JSON_TYPE_DOUBLE, "ver", &ver) == 0) {
        goto end;
    }
    int ts = 12456662;
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "timestamp", &ts) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "uuid", "3b85d2aa-1dd2-11b2-9874-99a311b3456") == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "token", "99a311b34hdt5666dddd56") == 0) {
        goto end;
    }
    memset(tmpbuf, 0, sizeof(tmpbuf));
    map_mac_to_string(tmpbuf, sizeof(tmpbuf), (cm->dev.macaddr));
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "macaddr", tmpbuf) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_STRING, "serial_num", cm->dev.serial_num) == 0) {
        goto end;
    }
    if (map_value_to_json_field(jobj, JSON_TYPE_UINT, "device_id", &cm->dev.device_id) == 0) {
        goto end;
    }

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_system_stats(jobj3);
    json_object_object_add(jobj2, "system_info", jobj3);

    jobj3 = json_object_new_object();
    if (!jobj3) {
        goto end;
    }
    airpro_get_brd_fw_version(jobj3);
    json_object_object_add(jobj2, "firmware_info", jobj3);

    json_object_object_add(jobj, "data", jobj2);

    return (void *)jobj;
end:
    if (jobj2) {
        json_object_put(jobj2);
    }
    if (jobj) {
        json_object_put(jobj);
    }
    return NULL;
}

int airpro_fetch_system_info(struct airpro_cm_handle *cm, char *data_buf, int *len)
{
    const char *buf = NULL;
    void *jsonbuf = NULL;

    jsonbuf = map_system_info_to_json(cm);
    if (jsonbuf) {
        buf = app_json_stringify(jsonbuf);
        if (buf) {
            *len = strlen(buf);
            strcpy(data_buf, buf);
        } else {
        }
        app_json_free(jsonbuf);
    }

    return 0;
}

int airpro_fetch_network_info(struct airpro_cm_handle *cm, char *data_buf, int *len)
{
    const char *buf = NULL;
    void *jsonbuf = NULL;

    jsonbuf = map_network_info_to_json(cm);
    if (jsonbuf) {
        buf = app_json_stringify(jsonbuf);
        if (buf) { 
            *len = strlen(buf);
            strcpy(data_buf, buf);
        } else {
        }
        app_json_free(jsonbuf);
    }
}

int airpro_fetch_wifi_info(struct airpro_cm_handle *cm, char *data_buf, int *len)
{
    const char *buf = NULL;
    void *jsonbuf = NULL;

    jsonbuf = map_wifi_info_to_json(cm);
    if (jsonbuf) {
        buf = app_json_stringify(jsonbuf);
        if (buf) { 
            *len = strlen(buf);
            strcpy(data_buf, buf);
        } else {
        }
        app_json_free(jsonbuf);
    }
}

int airpro_fetch_device_info_type(struct airpro_cm_handle *cm, enum airpro_cmd_type cmd_type, char *data_buf, int *len)
{
    int res = -1;

    switch (cmd_type) {
        case AIRPRO_DEV_CMD_TYPE_SYSTEM: {
            res = airpro_fetch_system_info(cm, data_buf, len);
        } break;
        case AIRPRO_DEV_CMD_TYPE_NETWORK: {
            res = airpro_fetch_network_info(cm, data_buf, len);
        } break;
        case AIRPRO_DEV_CMD_TYPE_WIFI: {
            res = airpro_fetch_wifi_info(cm, data_buf, len);
        } break;
        default: {
            return -1;
        }
    }

    return res;
}
