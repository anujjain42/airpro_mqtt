import threading
from itertools import count
import paho.mqtt.client as paho
import paho.mqtt.subscribe as sub
import json
import time

broker = "127.0.0.1"
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
publisher.username_pw_set(username="steve",password="password")
publisher.connect(broker,port)


t1 = threading.Thread(target = airpro_subs)
t1.start()


count = 0
while True:
    # topic , payload
    publisher.publish("airpro/dev/co/cloud","hiii from client2 - times" + str(count))
    time.sleep(5)
    count+=1
