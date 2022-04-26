import threading
from itertools import count
import paho.mqtt.client as paho
import paho.mqtt.subscribe as sub
import json
import time

broker = "127.0.0.1"
port = 1883


# paho.Client.connected_flag = False
# paho.Client.bad_connection_flag = False
publisher = paho.Client("PUBLISHER")
print("connected")
# publisher.username_pw_set(username="steve",password="password")
publisher.connect(broker,port)

publisher.publish("airpro/dev/to/cloud","hiii from client - times")

