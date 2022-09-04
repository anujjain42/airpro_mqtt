import paho.mqtt.client as paho
from abc import abstractmethod
from broker.models import *

class MQTTDumpData:

    @abstractmethod
    def mqttDataDumpToDB(Self,*args,**kwargs):
        pass

class SystemDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        del kwargs['uuid'], kwargs['serial_num']

        try:
            object = SystemDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            SystemDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
        except SystemDeviceInfo.DoesNotExist:
            SystemDeviceInfo.objects.create(**kwargs)


class NetworkDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = NetworkDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            NetworkDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
        except NetworkDeviceInfo.DoesNotExist:
            NetworkDeviceInfo.objects.create(**kwargs)


class WifiDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = WifiDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            WifiDeviceInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
        except WifiDeviceInfo.DoesNotExist:
            WifiDeviceInfo.objects.create(**kwargs)
            

class ClientDumpDate(MQTTDumpData):
    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = ClientInfo.objects.get(device_id=kwargs['device_id']) 
            ClientInfo.objects.filter(device_id=kwargs['device_id']).update(**kwargs)
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