#ifndef __AIRPRO_MGR_MSG_H__
#define __AIRPRO_MGR_MSG_H__

#include <pthread.h>
#include <stdint.h>

#define MAX_MAC_ADDR_LEN         6
#define MAX_NWID_LEN             32
#define AIRPRO_MAX_STA           64
#define MAX_NEIGHBOUR_COUNT      32
#define MAX_SCAN_CHAN            27
#define MAX_DNS_LENGTH           256
#define MAX_NUM_DNS              10
#define MAX_NUM_RADIO            3
#define MAX_DHCP_ENTRY           8
#define MAX_STATIC_ROUTE_ENTRY   16
#define MAX_VLAN_ENTRY           16
#define MAX_STATIC_LEASE_ENTRY   17

enum supported_modes {
    MODE_11A = 1,
    MODE_11B = 2,
    MODE_11G = 3,
    MODE_11N = 4,
    MODE_11NA = 5,
    MODE_11BGN = 6,
    MODE_11AC = 7,
    MODE_11AX = 8,
    MODE_11NA_11AC_11AX = 9,
    MODE_11BGN_11AX = 10,
    MAX_SUPPORTED_MODES,
};

enum api_status {
    API_STATUS_SUCCESS = 0,
    API_STATUS_FAILURE,
};

struct AIRPRO_MGR_MSG_HDR {
    int cookie;
    char component_type;
};

enum COMPONENT_TYPE {
    COMPONENT_TYPE_WIFI_MANAGER = 1,
    COMPONENT_TYPE_SYSTEM_INFO = 2,
    COMPONENT_TYPE_NETWORK_INFO = 3,
};

enum airpro_bw_mapping {
    AIRPRO_BW_20MHZ = 20,
    AIRPRO_BW_40MHZ = 40,
    AIRPRO_BW_80MHZ = 80,
    AIRPRO_BW_160MHZ = 160,
    AIRPRO_BW_NONE = 0,
};

enum airpro_radio_mapping {
    AIRPRO_RADIO_24 = 0,
    AIRPRO_RADIO_5 = 1,
    AIRPRO_RADIO_6 = 2,
};

struct airpro_mgr_data {
    struct AIRPRO_MGR_MSG_HDR hdr;
    char type;  //Type of cmd
    int len;
    char data[0]; //COMPONENT specific data
};

/* wifi manager */
struct airpro_mgr_iface_idx {
    int radio_id;
    int vap_id;
};

struct airpro_mgr_macaddr {
    uint8_t macaddr[6];
};

struct airpro_mgr_wlan_radio_params {
    int radio_index;
    int channel;
    int mode;
    int disabled;
    int txpower;
    int country;
    int rf_bw;
    int max_sta;
    char radio_type[10]; //2.4Ghz, 5Ghz, 6Ghz
    char hwmode[32];
    char htmode[32];
};

struct airpro_mgr_wlan_vap_params {
    int radio_index;
    int vap_index;
    int wds;
    int hide_ssid;
    int isolate;
    char ssid[32];
    char opmode[10];
    char key[32];
    int encrypt_type;
    char server_name[32];
    char server_ip[64];
    int auth_port;
    int acct_port;
    char wifi_device[10]; //wifi0, wifi1, wifi2
    char network[10];
};

struct airpro_wifimgr_event_type_get_radio_stats {
    int8_t      radio_id;
    uint16_t    num_iface;
    int8_t      cu;
    int8_t      cu_wifi;
    int8_t      cu_wifi_obss;
    int8_t      cu_wifi_self;
    uint8_t     chan;
    uint8_t     phymode;
    uint8_t     bw;
    char        radio_type[10];
};

struct airpro_wifimgr_event_type_get_all_client_count {
    int total_client;
    int per_radio_client[MAX_NUM_RADIO];
};

struct airpro_wifimgr_event_type_get_all_client_rssi {
    int total_client;
    int num_client_weak;
    int num_client_medium;
    int num_client_strong;
};

struct airpro_wifimgr_event_type_get_client_rate_limit {
    uint8_t macaddr[6];
    uint32_t dl_bytes_per_sec;
    uint32_t ul_bytes_per_sec;
};

struct airpro_wifimgr_event_type_get_whitelist_clients {
    uint16_t num_clients;
    uint8_t macaddr[256][MAX_MAC_ADDR_LEN];
};

struct airpro_wifimgr_event_type_get_blacklist_clients {
    uint16_t num_clients;
    uint8_t macaddr[256][MAX_MAC_ADDR_LEN];
};

struct airpro_wifimgr_event_type_get_ap_stats {
    uint8_t     radio_id; //incase multiple ssid with same name on diff band
    uint8_t     vap_id;
    uint8_t     macaddr[MAX_MAC_ADDR_LEN];
    uint8_t     bssid[MAX_MAC_ADDR_LEN];
    uint8_t     ssid[MAX_NWID_LEN+1];
    uint8_t     ssid_len;
    uint8_t     num_sta;
    uint64_t    tot_tx_in_mb;
    uint64_t    tot_rx_in_mb;
    uint64_t    tot_tx_pkts;
    uint64_t    tot_rx_pkts;
    uint64_t    tot_usage_in_mb;
};

struct airpro_wifimgr_event_type_get_sta_stats {
    uint8_t            macaddr[MAX_MAC_ADDR_LEN];
    uint8_t            bssid[MAX_MAC_ADDR_LEN];
    uint8_t            domain_name_count;
    uint8_t            txpower;
    int8_t             rssi;
    uint8_t            channel;
    uint8_t            bw;
    uint32_t           tx_kbps;
    uint32_t           rx_kbps;
    uint32_t           tx_mb;
    uint32_t           rx_mb;
    uint32_t           tot_tx_rx_mb;
    uint8_t            device_type[64];
    uint8_t            os_type[64];
    uint8_t            domain_name[MAX_NUM_DNS][MAX_DNS_LENGTH];
};

struct airpro_neighbor_list {
    uint32_t   phymode;
    int        rssi;
    uint8_t    bssid[MAX_MAC_ADDR_LEN];
    uint8_t    ssid_len;
    uint8_t    ssid[MAX_NWID_LEN+1];
};

struct airpro_chan_scan_results {
    uint8_t         chan_load;
    uint8_t         chan;
    uint8_t         nbss;       /*number of bss per channel */
    struct airpro_neighbor_list neighbor_list[MAX_NEIGHBOUR_COUNT];
};

struct airpro_wifimgr_event_type_get_chan_scan_results {
    uint8_t nchans;
    struct airpro_chan_scan_results scan_results[MAX_SCAN_CHAN];
};

struct airpro_wifimgr_event_type_get_supported_modes {
    int num_modes;
    int max_supported_modes[MAX_SUPPORTED_MODES];
};

struct airpro_wifimgr_event_wlan_radio_params {
    int channel;
    int disabled;
    int txpower;
    int mode;
    int country;
    int rf_bw;
    int max_sta;
    char radio_type[10];
    char hwmode[32];
    char htmode[32];
};

struct airpro_wifimgr_event_wlan_vap_params {
    int wds;
    int hide_ssid;
    char ssid[32];
    char opmode[10];
    char ifname[10];
    char network[10];
    char key[32];
    int encrypt_type;
    char server_name[32];
    char server_ip[64];
    int auth_port;
    int acct_port;
    int isolate;
};

struct airpro_wifimgr_assoc_sta_summary {
    uint8_t     ssid[MAX_NWID_LEN+1];
    uint8_t     ssid_len;
    uint8_t     mac[MAX_MAC_ADDR_LEN];
    char        ip[20];
    char        hostname[17];
    int8_t      rssi;
    uint8_t     channel;
    uint32_t    txratekbps;
    uint32_t    rxratekbps;
};

struct airpro_wifimgr_event_get_all_assoc_sta_summary {
    int num_sta;
    struct airpro_wifimgr_assoc_sta_summary sta_summary[AIRPRO_MAX_STA];
};

struct airpro_wifimgr_event_get_assoc_sta_summary {
    uint8_t radio_id;
    uint8_t vap_id;
    int num_sta;
    struct airpro_wifimgr_assoc_sta_summary sta_summary[AIRPRO_MAX_STA];
};

struct airpro_system_event_type_isp_info {
    int test_in_progress;
    char isp_name[64];
    char isp_external_ip[64];
    double isp_download_speed;
    double isp_upload_speed;
    double latitude;
    double longitude;
};

struct airpro_system_event_type_brd_fw_info {
    char board_info[256];
    char fw_info[256];
};

struct airpro_wifimgr_event_type_int {
    int val;
};

struct airpro_system_event_type_stats {
    int cpu_usage;
    int max_memory; 
    int max_memory_used;
    int max_disk_space;
    int max_disk_space_used;
    int uptime;
};

enum nat_mode_ip_type {
    AIRPRO_NAT_MODE_IP_TYPE_STATIC = 1,
    AIRPRO_NAT_MODE_IP_TYPE_DHCP = 2,
    AIRPRO_NAT_MODE_IP_TYPE_PPPOE = 3,
};

enum bridge_mode_ip_type {
    AIRPRO_BRIDGE_MODE_IP_TYPE_STATIC = 1,
    AIRPRO_BRIDGE_MODE_IP_TYPE_DHCP = 2,
};

struct nat_mode_static_params {
    char ip_addr[64];
    char ip_netmask[64];
    char def_gw[64];
    char dns[64];
};

struct bridge_mode_static_params {
    char ip_addr[64];
    char ip_netmask[64];
    char def_gw[64]; 
    char dns[64]; 
};

struct nat_mode_pppoe_params {
    char username[64];
    char password[64];
};

struct nat_mode_dhcp_params {
    char def_gw[64];
};

struct bridge_mode_dhcp_params {
    char def_gw[64];
};

enum airpro_net_mode {
    AIRPRO_NETWORK_MODE_NAT = 1,
    AIRPRO_NETWORK_MODE_BRIDGE = 2,
};

struct airpro_network_mode {
    int net_mode;
};

struct airpro_system_mode {
    int system_mode;
}; 

struct airpro_speedtest_interval {
    int interval;
};

struct airpro_del_dhcp_lease {
    char pool_name[32];
};

struct airpro_del_static_route {
    char route_name[32];
};

struct airpro_del_vlan_entry {
    char vlan_name[32];
};

struct airpro_del_static_lease_entry {
    char static_lease_name[32];
};

struct airpro_dhcp_lease_entry {
    char pool_name[32];
    char ip_addr[64];
    char ip_netmask[64];
    int start_addr;
    int end_addr;
    char def_gw[64];
    int lease_time;
    char pri_dns[64];
    char sec_dns[64];
};

struct airpro_static_route_entry {
    char route_name[32];
    char iface[32];
    char ip_target[64];
    char ip_netmask[64];
    char ip_gw[64];
};

struct airpro_vlan_entry {
    char vlan_name[32];
    int vlan_id;
    int tag;
    int ip_source;
    char ip_addr[64];
    char ip_netmask[64];
};

struct airpro_static_lease_entry {
    char client_name[32];
    char ip_client[64];
    char ip_netmask[64];
    char client_mac[64];
    char ip_gw[64];
    char ip_dns[64];
};

struct airpro_nat_mode_config {
    enum nat_mode_ip_type ip_type;
    union nat_mode_params {
        struct nat_mode_static_params static_mode_params;
        struct nat_mode_dhcp_params dhcp_mode_params;
        struct nat_mode_pppoe_params pppoe_mode_params;
    } u;
};

struct airpro_get_all_dhcp_lease_entry {
    int num_entry;
    struct airpro_dhcp_lease_entry dhcp[MAX_DHCP_ENTRY];
};

struct airpro_get_all_static_route_entry {
    int num_entry; 
    struct airpro_static_route_entry route[MAX_STATIC_ROUTE_ENTRY];
};

struct airpro_get_all_vlan_entry {
    int num_entry; 
    struct airpro_vlan_entry vlan[MAX_VLAN_ENTRY];
};

struct airpro_get_all_static_lease_entry {
    int num_entry; 
    struct airpro_static_lease_entry static_lease[MAX_STATIC_LEASE_ENTRY];
};

struct airpro_bridge_mode_config {
    int vlan_id;
    enum bridge_mode_ip_type ip_type;
    struct bridge_mode_static_params static_mode_params;
    struct bridge_mode_dhcp_params dhcp_mode_params;
};

struct airpro_active_network_config {
    char network_mode[32];
    char wan_interface[32];
    char ip_addr[32];
}; 

enum acl_rule_type {
    ACL_RULE_NONE  = 0,
    ACL_RULE_IP    = 1,
    ACL_RULE_MAC   = 2,
    ACL_RULE_PROTO = 3,
};

enum acl_action_type {
    ACL_ACTION_DENY   = 0,
    ACL_ACTION_PERMIT = 1,
};

struct airpro_acl_macaddr {
    uint8_t mac[6];
};

struct airpro_acl_ipaddr {
    char ip[64];
};

struct airpro_acl_config {
    enum acl_rule_type rule;
    union airpro_acl_param {
        struct airpro_acl_macaddr macaddr;
        struct airpro_acl_ipaddr ipaddr;
        int proto;
    }u;
    enum acl_action_type action;
};

struct airpro_rate_limit_config {
    uint8_t macaddr[6];
    uint32_t dl_bytes_per_sec;
    uint32_t ul_bytes_per_sec;
};


#define AIRPRO_SET_COMPONENT_TYPE_WIFI_MGR(_hdr) (_hdr)->component_type = COMPONENT_TYPE_WIFI_MANAGER
#define AIRPRO_SET_COMPONENT_TYPE_SYSTEM_INFO(_hdr) (_hdr)->component_type = COMPONENT_TYPE_SYSTEM_INFO
#define AIRPRO_SET_COMPONENT_TYPE_NETWORK_INFO(_hdr) (_hdr)->component_type = COMPONENT_TYPE_NETWORK_INFO

/* NETWORK CMD MSGTYPE */
#define AIRPRO_NETWORK_DATA_TYPE_CMD_SET_NAT_MODE                   1
#define AIRPRO_NETWORK_DATA_TYPE_CMD_SET_BRIDGE_MODE                2
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NAT_MODE_CONFIG            3
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_BRIDGE_MODE_CONFIG         4
#define AIRPRO_NETWORK_DATA_TYPE_CMD_SET_NETWORK_MODE               5
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_NETWORK_MODE               6
#define AIRPRO_NETWORK_DATA_TYPE_CMD_ADD_DHCP_LEASE                 7
#define AIRPRO_NETWORK_DATA_TYPE_CMD_DEL_DHCP_LEASE                 8
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_DHCP_LEASE             9
#define AIRPRO_NETWORK_DATA_TYPE_CMD_ADD_STATIC_ROUTE               10
#define AIRPRO_NETWORK_DATA_TYPE_CMD_DEL_STATIC_ROUTE               11
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_STATIC_ROUTE           12
#define AIRPRO_NETWORK_DATA_TYPE_CMD_ADD_VLAN                       13
#define AIRPRO_NETWORK_DATA_TYPE_CMD_DEL_VLAN                       14
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_VLAN                   15
#define AIRPRO_NETWORK_DATA_TYPE_CMD_ADD_STATIC_LEASE               16
#define AIRPRO_NETWORK_DATA_TYPE_CMD_DEL_STATIC_LEASE               17
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ALL_STATIC_LEASE           18
#define AIRPRO_NETWORK_DATA_TYPE_CMD_NETWORK_COMMIT                 19
#define AIRPRO_NETWORK_DATA_TYPE_CMD_GET_ACTIVE_NETWORK_CONFIG      20

/* NETWORK EVENT MSGTYPE */
#define AIRPRO_NETWORK_EVENT_SET_NAT_MODE                           1
#define AIRPRO_NETWORK_EVENT_SET_BRIDGE_MODE                        2
#define AIRPRO_NETWORK_EVENT_GET_NAT_MODE_CONFIG                    3
#define AIRPRO_NETWORK_EVENT_GET_BRIDGE_MODE_CONFIG                 4
#define AIRPRO_NETWORK_EVENT_SET_NETWORK_MODE                       5
#define AIRPRO_NETWORK_EVENT_GET_NETWORK_MODE                       6
#define AIRPRO_NETWORK_EVENT_ADD_DHCP_LEASE                         7
#define AIRPRO_NETWORK_EVENT_DEL_DHCP_LEASE                         8
#define AIRPRO_NETWORK_EVENT_GET_ALL_DHCP_LEASE                     9
#define AIRPRO_NETWORK_EVENT_ADD_STATIC_ROUTE                       10
#define AIRPRO_NETWORK_EVENT_DEL_STATIC_ROUTE                       11
#define AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_ROUTE                   12
#define AIRPRO_NETWORK_EVENT_ADD_VLAN                               13
#define AIRPRO_NETWORK_EVENT_DEL_VLAN                               14
#define AIRPRO_NETWORK_EVENT_GET_ALL_VLAN                           15
#define AIRPRO_NETWORK_EVENT_ADD_STATIC_LEASE                       16
#define AIRPRO_NETWORK_EVENT_DEL_STATIC_LEASE                       17
#define AIRPRO_NETWORK_EVENT_GET_ALL_STATIC_LEASE                   18
#define AIRPRO_NETWORK_EVENT_NETWORK_COMMIT                         19
#define AIRPRO_NETWORK_EVENT_GET_ACTIVE_NETWORK_CONFIG              20

/*SYSTEM INFO MSGTYPE */
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_SYSTEM_STATS                1
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_SPEEDTEST_RESULT            2
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_BRD_FW_INFO                 3
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_SET_SYSTEM_MODE                 4
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_GET_SYSTEM_MODE                 5
#define AIRPRO_SYSTEM_DATA_TYPE_CMD_SET_SPEEDTEST_INTERVAL          6

/*SYSTEM EVENT MSGTYPE */
#define AIRPRO_SYSTEM_EVENT_GET_SYSTEM_STATS                        1
#define AIRPRO_SYSTEM_EVENT_GET_SPEEDTEST_RESULT                    2
#define AIRPRO_SYSTEM_EVENT_GET_BRD_FW_INFO                         3
#define AIRPRO_SYSTEM_EVENT_SET_SYSTEM_MODE                         4
#define AIRPRO_SYSTEM_EVENT_GET_SYSTEM_MODE                         5
#define AIRPRO_SYSTEM_EVENT_SET_SPEEDTEST_INTERVAL                  6

/*WIFIMGR MSGTYPE */
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_RADIO_STATS               1
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_SET_WIRELESS_RADIO        2
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_GET_WIRELESS_RADIO        3
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_SET_WIRELESS_VAP          4
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_GET_WIRELESS_VAP          5
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_ADD_WIRELESS_VAP          6
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_DEL_WIRELESS_VAP          7
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_UCI_RESET_WIRELESS_RADIO      8
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_ALL_ASSOC_STA_SUMMARY     9
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_AP_STATS                  10
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_CHANNEL_SCAN_RESULTS      11
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_ASSOC_STA_SUMMARY         12
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_STA_STATS                 13
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_ADD_TO_BLACKLIST              14
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_DEL_FROM_BLACKLIST            15
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_ADD_TO_WHITELIST              16
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_DEL_FROM_WHITELIST            17
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_ADD_ACLRULE                   18
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_DEL_ACLRULE                   19
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_ADD_RATE_LIMIT                20
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_DEL_RATE_LIMIT                21
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_FLUSH_BLACKLIST               22
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_FLUSH_WHITELIST               23
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_ALL_CLIENT_COUNT          24
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_ALL_CLIENT_RSSI           25
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_CLIENT_RATE_LIMIT         26
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_BLACKLIST_CLIENTS         27
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_WHITELIST_CLIENTS         28
#define AIRPRO_WIFI_MGR_DATA_TYPE_CMD_GET_SUPPORTED_RADIO_MODES     29

/*WIFIMGR EVENT */
#define AIRPRO_WIFI_MGR_EVENT_GET_RADIO_STATS                       1
#define AIRPRO_WIFI_MGR_EVENT_UCI_SET_WIRELESS_RADIO                2
#define AIRPRO_WIFI_MGR_EVENT_UCI_GET_WIRELESS_RADIO                3
#define AIRPRO_WIFI_MGR_EVENT_UCI_SET_WIRELESS_VAP                  4
#define AIRPRO_WIFI_MGR_EVENT_UCI_GET_WIRELESS_VAP                  5
#define AIRPRO_WIFI_MGR_EVENT_UCI_ADD_WIRELESS_VAP                  6
#define AIRPRO_WIFI_MGR_EVENT_UCI_DEL_WIRELESS_VAP                  7
#define AIRPRO_WIFI_MGR_EVENT_UCI_RESET_WIRELESS_RADIO              8
#define AIRPRO_WIFI_MGR_EVENT_GET_ALL_ASSOC_STA_SUMMARY             9
#define AIRPRO_WIFI_MGR_EVENT_GET_AP_STATS                          10
#define AIRPRO_WIFI_MGR_EVENT_GET_CHANNEL_SCAN_RESULTS              11
#define AIRPRO_WIFI_MGR_EVENT_GET_ASSOC_STA_SUMMARY                 12
#define AIRPRO_WIFI_MGR_EVENT_GET_STA_STATS                         13
#define AIRPRO_WIFI_MGR_EVENT_ADD_TO_BLACKLIST                      14
#define AIRPRO_WIFI_MGR_EVENT_DEL_FROM_BLACKLIST                    15
#define AIRPRO_WIFI_MGR_EVENT_ADD_TO_WHITELIST                      16
#define AIRPRO_WIFI_MGR_EVENT_DEL_FROM_WHITELIST                    17
#define AIRPRO_WIFI_MGR_EVENT_ADD_ACLRULE                           18
#define AIRPRO_WIFI_MGR_EVENT_DEL_ACLRULE                           19
#define AIRPRO_WIFI_MGR_EVENT_ADD_RATE_LIMIT                        20
#define AIRPRO_WIFI_MGR_EVENT_DEL_RATE_LIMIT                        21
#define AIRPRO_WIFI_MGR_EVENT_FLUSH_BLACKLIST                       22
#define AIRPRO_WIFI_MGR_EVENT_FLUSH_WHITELIST                       23
#define AIRPRO_WIFI_MGR_EVENT_GET_ALL_CLIENT_COUNT                  24
#define AIRPRO_WIFI_MGR_EVENT_GET_ALL_CLIENT_RSSI                   25
#define AIRPRO_WIFI_MGR_EVENT_GET_CLIENT_RATE_LIMIT                 26
#define AIRPRO_WIFI_MGR_EVENT_GET_BLACKLIST_CLIENTS                 27
#define AIRPRO_WIFI_MGR_EVENT_GET_WHITELIST_CLIENTS                 28
#define AIRPRO_WIFI_MGR_EVENT_GET_SUPPORTED_RADIO_MODES             29

struct airpro_wifi_cp_data {
    pthread_t coplane_thread_id;
    pthread_mutex_t lock;
    pthread_cond_t cond;
};

struct airpro_coplane {
    struct airpro_wifi_cp_data cpd;
};

#endif
