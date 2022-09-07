import requests
import json
from requests.auth import HTTPBasicAuth
from django.conf import settings
import paho.mqtt.client as paho
from abc import abstractmethod
from broker.models import *

auth                = HTTPBasicAuth('admin@admin.com', 'dots@123')
headers             = {'Content-Type': 'application/json'}
ui_base_url = settings.UI_BASE_URL

class MQTTDumpData:

    @abstractmethod
    def mqttDataDumpToDB(Self,*args,**kwargs):
        pass

class SystemDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = SystemDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            SystemDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
            object.save()
        except SystemDeviceInfo.DoesNotExist:
            SystemDeviceInfo.objects.create(**kwargs)


class NetworkDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = NetworkDeviceInfo.objects.get(device_id=kwargs['device_id'])
            NetworkDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
            object.save()
        except NetworkDeviceInfo.DoesNotExist:
            NetworkDeviceInfo.objects.create(**kwargs)


class WifiDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = WifiDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            WifiDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
            object.save()
        except WifiDeviceInfo.DoesNotExist:
            WifiDeviceInfo.objects.create(**kwargs)
            

class ClientDumpDate(MQTTDumpData):
    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = ClientInfo.objects.get(device_id=kwargs['device_id']) 
            ClientInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
            object.save()
        except ClientInfo.DoesNotExist:
            ClientInfo.objects.create(**kwargs)

def getTypeObject(device_type):
    """
        Return Model objects based on type
    """
    type_obj = {"1003":NetworkDumpData(), "1004":ClientDumpDate(), "1005":SystemDumpData(), "1006":WifiDumpData()}
    return type_obj[str(device_type)] if str(device_type) in type_obj.keys() else  None

def publish_to_mqtt(msg, broker, port, device_topic):
    publisher = paho.Client("PUBLISHER")
    publisher.connect(broker,port)
    publisher.publish(device_topic,str(msg))

def update_device_id(device_id, serial_number):
    DEVICE_UI_URL       = f"{ui_base_url}/device/"
    data = {"device_id":device_id,"mqtt_status":True, 'status':'Online'}
    res = requests.patch(f'{DEVICE_UI_URL}{str(serial_number)}/', data = json.dumps(data), headers=headers,auth=auth)