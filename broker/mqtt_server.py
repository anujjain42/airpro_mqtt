import paho.mqtt.client as server_mqtt
import json
from broker.models import Device
from broker.customabstractmethod import getTypeObject

# broker = "122.170.105.253"
broker = "127.0.0.1"
port = 1883

#connecting

def on_connect(subs, obj, flags, rc):
    if rc==0:
        subs.connected_flag=True #set flag
        # print("connected OK")
        # print("rc :" + str(rc))
        subs.subscribe("airpro/dev/to/cloud")
    else:
        print("Bad connection Returned code=",rc)
        subs.bad_connection_flag = True
    

def on_message(subs, obj, msg):
    data = msg.payload.decode()
    data = json.loads(data)
    print(data)
    # data = json.dumps(data)
    device_id = data['device_id']
    data['device_id'] = Device.objects.get(device_id=device_id)  

    mqtt_device_obj = getTypeObject(data['type'])
    print(mqtt_device_obj)
    if mqtt_device_obj:
        mqtt_device_obj.mqttDataDumpToDB(**data)

    publisher = server_mqtt.Client("PUBLISHER")
    subs.connect(broker, port)
    # publisher.username_pw_set(username="airpro_mqtt_server",password="Y3VDWxsijgfuXdE")
    publisher.publish("airpro/device/"+str(device_id),"msg recived")

    

server_mqtt.Client.connected_flag = False
server_mqtt.Client.bad_connection_flag = False
subs = server_mqtt.Client("MQTT_AIRPRO_SUBS")
subs.on_connect = on_connect
subs.on_message = on_message
subs.connect(broker, port)
# subs.loop_forever()
subs.loop_start()

