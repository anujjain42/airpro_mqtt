import threading
from itertools import count
import paho.mqtt.client as paho
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
        subs.subscribe("device_topic2")
    else:
        print("Bad connection Returned code=",rc)
        subs.bad_connection_flag = True
    

def on_message(subs, obj, msg):
    data = msg.payload
    # data = data.decode()
    # print(json.loads(data))
    print(data)

    # print(msg.topic + " " + str(msg.payload))


def airpro_subs():
    subs = paho.Client("SUBS")
    subs.on_connect = on_connect
    subs.on_message = on_message

    subs.connect("127.0.0.1",1883,60)
    subs.loop_start()




#connecting
paho.Client.connected_flag = False
paho.Client.bad_connection_flag = False
publisher = paho.Client("PUBLISHER")
print("connected")
# publisher.username_pw_set(username="steve",password="password")
publisher.connect(broker,port)


# t1 = threading.Thread(target = airpro_subs)
# t1.start()
msg={ "type": 1003, "flag": "REQ", "ver": 1, "timestamp": 1662105679030, "macaddr": "f6:65:e3:d8:b3:81", "device_id": "8801389438", "data": { "ap_stats": { "ap_stats_list": [ { "channel": 6, "status": "ONLINE", "ssid": "Busybrainz", "bssid": "44:d1:fa:ac:91:a0", "numsta": 1, "txpower": 25, "phymode": "11GHE20", "ipaddr": "", "mgmt_ipaddr": "192.168.100.10", "fw_version": "1.0.1", "hw_version": "3.2.1" } ] } } }
msg={ "type": 1004, "flag": "REQ", "ver": 1, "timestamp": 1662105679032, "macaddr": "f6:65:e3:d8:b3:81", "device_id": "8801389438", "data": { "sta_stats": { "sta_stats_list": [ { "macaddr": "23:34:54:22:44:33", "ipaddr": "192.168.10.100", "alias": "airprotest", "ap_serial_num": "0077d3GG55", "ssid": "AirPro", "band": 1, "traffic_mb": 3000, "uplink_mbps": 50, "downlink_mbps": 50, "delay_ms": 300, "pkt_loss_percent": "30%", "full_name": "myphone", "terminal": "abc", "operation_system": "ubuntu", "manufacturer": "qualcomm", "online_time": "22:46:39 IST", "last_seen": "5 sec ago", "uptime": "12:12:12" } ] } } }
msg={ "type": 1005, "flag": "REQ", "ver": 1, "timestamp": 1662105679035, "macaddr": "f6:65:e3:d8:b3:81", "device_id": "8801389438", "data": { "system_stats": { "system_stats_list": { "hw_name": "", "fwinfo": "", "hostname": "", "cpu_usage": 3, "max_memory": 887664, "max_memory_used": 304012, "max_disk_space": 22016, "max_disk_space_used": 100, "uptime": 19845 } } } }
msg={ "type": 1006, "flag": "REQ", "ver": 1, "timestamp": 1662105679193, "macaddr": "f6:65:e3:d8:b3:81", "device_id": "8801389438", "data": { "uci_configs": { "radio_list": [ { "channel": 36, "disabled": 1, "txpower": 25, "country": 840, "radio_type": "5GHz", "hwmode": "11axa", "vap_list": [ ] }, { "channel": 6, "disabled": 0, "txpower": 25, "country": 840, "radio_type": "2.4GHz", "hwmode": "11axg", "vap_list": [ { "ssid": "Busybrainz", "opmode": "ap", "network": "lan", "key": "9431597074", "encryption": "psk2" } ] } ] } } }
msg={
   "type":1006,
   "flag":"REQ",
   "ver":1,
   "timestamp":1662105679193,
   "macaddr":"f6:65:e3:d8:b3:81",
   "device_id":"2374590439",
   "data":{
      
      "uci_configs":{
         "radio_list":[
            {
               "hwmode":"FDF",
               "radio_index":0,
               "channel":40,
               "country":356,
               "txpower":25,
               "disabled":0,
               "region":"IN",
               "vap_list":[
                  {
                     "key":"admin@123",
                     "ssid":"dbcasiu",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":1,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":False
                  },
                  {
                     "key":"admin@123",
                     "ssid":"dfnsdi",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":2,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":False
                  },
                  {
                     "key":"admin@123",
                     "ssid":"sfjsdiufk",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":3,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":True
                  },
                  {
                     "key":"admin@123",
                     "ssid":"fnebwuifk",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":4,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":False
                  }
               ],
               "radio_type":"5GHz"
            },
            {
               "hwmode":"DSFA",
               "radio_index":1,
               "channel":1,
               "country":356,
               "txpower":25,
               "disabled":0,
               "region":"IN",
               "vap_list":[
                  {
                     "key":"admin@123",
                     "ssid":"fklkhadsifu",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":1,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":False
                  },
                  {
                     "key":"admin@123",
                     "ssid":"skfldsajoifh",
                     "opmode":"ap",
                     "network":"lan",
                     "encryption":"psk2",
                     "ssid_index":2,
                     "uprate":"2kb",
                     "downrate":"2kb",
                     "wlan_uprate":"2kb",
                     "wlan_downrate":"2kb",
                     "ishidden":True,
                     "is_deleted":False
                  }
               ],
               "radio_type":"2.4GHz"
            }
         ]
      }
   }
}
count = 0
while True:
    # topic , payload
   #  publisher.publish("airpro/dev/to/cloud",json.dumps(msg))
    print('hfhf')
    publisher.publish("airpro/device/2374590439",json.dumps(msg))
    time.sleep(5)
    count+=1
