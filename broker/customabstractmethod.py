import paho.mqtt.client as paho
from abc import abstractmethod
from .models import *

class MQTTDumpData:

    @abstractmethod
    def mqttDataDumpToDB(Self,*args,**kwargs):
        pass

class SystemDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        SystemDeviceInfo.objects.create(**kwargs)


class NetworkDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        NetworkDeviceInfo.objects.create(**kwargs)

class WifiDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        WifiDeviceInfo.objects.create(**kwargs)


def getTypeObject(device_type):
    """
        Return Model objects based on type
    """
    type_obj = {"1000":SystemDumpData(),"1001":NetworkDumpData(),"1006":WifiDumpData()}
    return type_obj[str(device_type)] if str(device_type) in type_obj.keys() else  None

def publish_to_mqtt(msg, broker, port, device_topic):
    publisher = paho.Client("PUBLISHER")
    publisher.connect(broker,port)
    publisher.publish(device_topic,str(msg))