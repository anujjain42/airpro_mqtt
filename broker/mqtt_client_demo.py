import threading
import paho.mqtt.client as server_mqtt
import paho.mqtt.subscribe as sub
import json
import time

broker = "122.170.105.253"
port = 1883

def on_connect(subs, obj, flags, rc):
    if rc==0:
        subs.connected_flag=True #set flag
        print("connected OK")
        print("rc :" + str(rc))
        subs.subscribe("airpro/device/3058152330")
    else:
        print("Bad connection Returned code=",rc)
        subs.bad_connection_flag = True

def on_message(subs, obj, msg):
    data = msg.payload
    print(data)

server_mqtt.Client.connected_flag = False
server_mqtt.Client.bad_connection_flag = False
subs = server_mqtt.Client("MQTT_AIRPRO_SUBS_CLIENT")
subs.on_connect = on_connect
subs.on_message = on_message
subs.connect(broker, port)
# subs.loop_forever()
subs.loop_start()

system = {
   "type":1000,
   "flag":"REQ",
   "ver":2.500000,
   "timestamp":12456662,
   "uuid":"3b85d2aa-1dd2-11b2-9874-99a311b3456",
   "token":"99a311b34hdt5666dddd56",
   "macaddr":"00:00:00:00:00:00",
   "serial_num":"",
   "device_id":3058152330,
   "data":{
      "system_info":{
         "cpu_usage":20,
         "max_memory":344,
         "max_memory_used":45,
         "max_disk_space":12,
         "max_disk_space_used":234,
         "uptime":112
      },
      "firmware_info":{
         "board_info":"AIRPRO-IPQ8072A\n",
         "fw_info":"AIROS-1.0.1-BUILD_15032022\n"
      }
   }
}

net={
   "type":1001,
   "flag":"REQ",
   "ver":2.500000,
   "timestamp":12456662,
   "uuid":"3b85d2aa-1dd2-11b2-9874-99a311b3456",
   "token":"99a311b34hdt5666dddd56",
   "macaddr":"00:00:00:00:00:00",
   "serial_num":"",
   "device_id":3058152330,
   "data":{
      "network_mode":{
         "net_mode":1
      },
      "nat_config":{
         "proto":"static",
         "ip_addr":"192.168.100.10",
         "ip_netmask":"255.255.255.0",
         "def_gw":"192.168.100.1",
         "dns":"8.8.8.8"
      },
      "active_network_config":{
         "network_mode":"NAT",
         "wan_interface":"STATIC",
         "ip_addr":"192.168.100.10"
      },
      "static_lease":{
         "static_lease_list":[
            {
               "client_name":"client1",
               "ip_client":"10.0.0.11",
               "ip_netmask":"255.255.255.0",
               "client_mac":"b0:10:41:02:37:0d",
               "ip_gw":"192.168.100.10",
               "ip_dns":"8.8.8.8"
            },
            {
               "client_name":"client2",
               "ip_client":"10.0.0.12",
               "ip_netmask":"255.255.255.0",
               "client_mac":"b0:10:41:02:37:01",
               "ip_gw":"192.168.100.10",
               "ip_dns":"8.8.8.8"
            }
         ]
      },
      "vlan":{
         "vlan_list":[
            {
               "vlan_name":"vlan1",
               "vlan_id":22,
               "ip_source":1,
               "ip_addr":"192.168.100.10",
               "ip_netmask":"255.255.255.0"
            },
            {
               "vlan_name":"vlan2",
               "vlan_id":42,
               "ip_source":1,
               "ip_addr":"192.168.100.5",
               "ip_netmask":"255.255.255.0"
            }
         ]
      }
   }
}

publisher = server_mqtt.Client("PUBLISHER")
publisher.connect(broker,port)
publisher.publish("airpro/dev/to/cloud",json.dumps(system))