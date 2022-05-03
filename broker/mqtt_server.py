import paho.mqtt.client as server_mqtt
import json
import time
from .models import *

broker = "122.170.105.253"
port = 1883

#connecting

def on_connect(subs, obj, flags, rc):
    if rc==0:
        subs.connected_flag=True #set flag
        print("connected OK")
        print("rc :" + str(rc))
        subs.subscribe("airpro/dev/to/cloud")
    else:
        print("Bad connection Returned code=",rc)
        subs.bad_connection_flag = True
    

def on_message(subs, obj, msg):
    f = open("demofile2.txt", "a")
    data = msg.payload.decode()    
    data = json.loads(data)
    f.writelines(json.dumps(data, indent = 4))
    f.writelines('\n')
    f.close()
    print(msg.topic + " " + str(data))
    
    # WifiDeviceInfo.objects.create(**data)
    publisher = server_mqtt.Client("PUBLISHER")
    subs.connect(broker, port)
    # publisher.username_pw_set(username="airpro_mqtt_server",password="Y3VDWxsijgfuXdE")
    publisher.publish(msg.topic,"msg recived")

    

server_mqtt.Client.connected_flag = False
server_mqtt.Client.bad_connection_flag = False
subs = server_mqtt.Client("MQTT_AIRPRO_SUBS")
subs.on_connect = on_connect
subs.on_message = on_message
subs.connect(broker, port)
# subs.loop_forever()
subs.loop_start()

