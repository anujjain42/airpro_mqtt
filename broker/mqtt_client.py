import threading
from itertools import count
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
        subs.subscribe("airpro/device/3981451246")
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