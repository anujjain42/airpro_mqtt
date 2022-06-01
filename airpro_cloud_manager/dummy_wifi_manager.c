#include "airpro_mgr_msg.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void airpro_cmd_get_system_stats(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_system_event_type_stats *ss = (struct airpro_system_event_type_stats *)mgr->data;

    AIRPRO_SET_COMPONENT_TYPE_SYSTEM_INFO(hdr);
    mgr->type = AIRPRO_SYSTEM_EVENT_GET_SYSTEM_STATS;

    ss->cpu_usage = 20;
    ss->max_memory = 344;
    ss->max_memory_used = 45;
    ss->max_disk_space = 12;
    ss->max_disk_space_used = 234;
    ss->uptime = 112;

    return;
}

void airpro_get_config_network_mode(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_network_mode *val = (struct airpro_network_mode *)mgr->data;

    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    mgr->type = AIRPRO_NETWORK_EVENT_GET_NETWORK_MODE;

    val->net_mode = 1;
}

void airpro_get_all_vlan(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_get_all_vlan_entry *val = (struct airpro_get_all_vlan_entry *)mgr->data;
    char buff[512], result[100];
    int idx = 0, num_entry = 0;

    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    mgr->type = AIRPRO_NETWORK_EVENT_GET_ALL_VLAN;

    memset(buff, 0, sizeof(buff));
    memset(result, 0, sizeof(result));

    for (idx = 0; idx < 2; idx++) {
        if (idx == 0) {
            strcpy(val->vlan[idx].vlan_name, "vlan1");
            val->vlan[idx].vlan_id = 22;
            val->vlan[idx].ip_source = 1; //static
            strcpy(val->vlan[idx].ip_addr, "192.168.100.10");
            strcpy(val->vlan[idx].ip_netmask, "255.255.255.0");
        } else if (idx == 1) {
            strcpy(val->vlan[idx].vlan_name, "vlan2");
            val->vlan[idx].vlan_id = 42;
            val->vlan[idx].ip_source = 1; //static
            strcpy(val->vlan[idx].ip_addr, "192.168.100.5");
            strcpy(val->vlan[idx].ip_netmask, "255.255.255.0");
        }
    }
    val->num_entry = idx;

    return;
}

void airpro_get_all_static_lease(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_get_all_static_lease_entry *val = (struct airpro_get_all_static_lease_entry *)mgr->data;
    char buff[512], result[100];
    int idx = 0;
    FILE *fp = NULL;

    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    mgr->type = AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_LEASE;

    memset(buff, 0, sizeof(buff));
    memset(result, 0, sizeof(result));

    for (idx = 0; idx < 2; idx++) {
        if (idx == 0) {
            strcpy(val->static_lease[idx].client_name, "client1");
            strcpy(val->static_lease[idx].ip_client, "10.0.0.11");
            strcpy(val->static_lease[idx].ip_netmask, "255.255.255.0");
            strcpy(val->static_lease[idx].client_mac, "b0:10:41:02:37:0d");
            strcpy(val->static_lease[idx].ip_dns, "8.8.8.8");
            strcpy(val->static_lease[idx].ip_gw, "192.168.100.10");
        } else if (idx == 1) {
            strcpy(val->static_lease[idx].client_name, "client2");
            strcpy(val->static_lease[idx].ip_client, "10.0.0.12");
            strcpy(val->static_lease[idx].ip_netmask, "255.255.255.0");
            strcpy(val->static_lease[idx].client_mac, "b0:10:41:02:37:01");
            strcpy(val->static_lease[idx].ip_dns, "8.8.8.8");
            strcpy(val->static_lease[idx].ip_gw, "192.168.100.10");
        }
    }
    val->num_entry = idx;

    return;
}


void airpro_get_nat_mode_config(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_nat_mode_config *val = (struct airpro_nat_mode_config *)mgr->data;
    enum nat_mode_ip_type ip_type;
    char result[100];
    struct nat_mode_static_params *static_var = &val->u.static_mode_params;

    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    mgr->type = AIRPRO_NETWORK_EVENT_GET_NAT_MODE_CONFIG;

    memset(result, 0, sizeof(result));
    ip_type = AIRPRO_NAT_MODE_IP_TYPE_STATIC;
    val->ip_type = ip_type;

    memset(result, 0, sizeof(result));
    strcpy(static_var->ip_addr, "192.168.100.10");

    memset(result, 0, sizeof(result));
    strcpy(static_var->ip_netmask, "255.255.255.0");

    memset(result, 0, sizeof(result));
    strcpy(static_var->def_gw, "192.168.100.1");

    memset(result, 0, sizeof(result));
    strcpy(static_var->dns, "8.8.8.8");
}

void airpro_cmd_get_wifi_chan_scan_res(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct airpro_wifimgr_event_type_get_chan_scan_results *scan_results =
                           (struct airpro_wifimgr_event_type_get_chan_scan_results *)mgr->data;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;

    AIRPRO_SET_COMPONENT_TYPE_WIFI_MGR(hdr);
    mgr->type = AIRPRO_WIFI_MGR_EVENT_GET_CHANNEL_SCAN_RESULTS;

    scan_results->nchans = 1;
    scan_results->scan_results[0].chan_load = 20;
    scan_results->scan_results[0].chan = 6;
    scan_results->scan_results[0].chan_load = 20;
    scan_results->scan_results[0].nbss = 1;
    scan_results->scan_results[0].neighbor_list[0].phymode = 10;
    scan_results->scan_results[0].neighbor_list[0].rssi = 20;
    scan_results->scan_results[0].neighbor_list[0].bssid[0] = 0x01;
    scan_results->scan_results[0].neighbor_list[0].bssid[1] = 0x02;
    scan_results->scan_results[0].neighbor_list[0].bssid[2] = 0x03;
    scan_results->scan_results[0].neighbor_list[0].bssid[3] = 0x04;
    scan_results->scan_results[0].neighbor_list[0].bssid[4] = 0x05;
    scan_results->scan_results[0].neighbor_list[0].bssid[5] = 0x06;
    scan_results->scan_results[0].neighbor_list[0].ssid_len= 6;
    strcpy(scan_results->scan_results[0].neighbor_list[0].ssid, "airpro");
}

void airpro_cmd_get_active_network_config(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_active_network_config *ev = (struct airpro_active_network_config *)mgr->data;
    char value[32];

    AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(hdr);
    mgr->type = AIRPRO_NETWORK_EVENT_GET_ACTIVE_NETWORK_CONFIG;

    memset(ev, 0, sizeof(struct airpro_active_network_config));
    strcpy(ev->network_mode, "NAT");
    strcpy(ev->wan_interface, "STATIC");
    strcpy(ev->ip_addr, "192.168.100.10");

    return 0;
}

void airpro_cmd_get_brd_fw_info(char *reply, int reply_buf_len)
{
    struct airpro_mgr_data *mgr = (struct airpro_mgr_data *)reply;
    struct AIRPRO_MGR_MSG_HDR *hdr = &mgr->hdr;
    struct airpro_system_event_type_brd_fw_info *brd_fw = (struct airpro_system_event_type_brd_fw_info *)mgr->data;
    FILE *fp = NULL;
    char line[256], count = 0;

    AIRPRO_SET_COMPONENT_TYPE_SYSTEM_INFO(hdr);
    mgr->type = AIRPRO_SYSTEM_EVENT_GET_BRD_FW_INFO;

    fp = fopen("version", "r");
    if (!fp) {
        return;
    }

    memset(line, 0, sizeof(line));
    while (fgets(line, sizeof(line), fp)) {
        if (count == 0) {
            strcpy(brd_fw->fw_info, line);
        } else if (count == 1) {
            strcpy(brd_fw->board_info, line);
        }
        memset(line, 0, sizeof(line));
        count++;
    }

    return;
}


int airpro_lib_send_cmd_system_msg(struct airpro_mgr_data *msg, int len,
                                       char *reply, int reply_buf_len)
{
    switch (msg->type) {
        case AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_SYSTEM_STATS: {
            airpro_cmd_get_system_stats(reply, reply_buf_len);
        } break;
        case AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_BRD_FW_INFO: {
            airpro_cmd_get_brd_fw_info(reply, reply_buf_len);
        } break;
    }
}

int airpro_lib_send_cmd_network_msg(struct airpro_mgr_data *msg, int len,
                                       char *reply, int reply_buf_len)
{
    switch (msg->type) {
        case AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ACTIVE_NETWORK_CONFIG: {
            airpro_cmd_get_active_network_config(reply, reply_buf_len);
        } break;
        case AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NETWORK_MODE: {
            airpro_get_config_network_mode(reply, reply_buf_len);
        } break;
        case AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NAT_MODE_CONFIG: {
            airpro_get_nat_mode_config(reply, reply_buf_len);
        } break;
        case AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_STATIC_LEASE: {
            airpro_get_all_static_lease(reply, reply_buf_len);
        } break;
        case AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_VLAN: {
            airpro_get_all_vlan(reply, reply_buf_len);
        } break;
    }
}

int airpro_lib_send_wifi_mgr_msg(struct airpro_mgr_data *msg, int len,
                                       char *reply, int reply_buf_len)
{
    switch (msg->type) {
        case AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_CHANNEL_SCAN_RESULTS: {
            airpro_cmd_get_wifi_chan_scan_res(reply, reply_buf_len);
        } break;
    }
}

