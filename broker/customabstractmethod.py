import paho.mqtt.client as paho
from abc import abstractmethod
from broker.models import *

class MQTTDumpData:

    @abstractmethod
    def mqttDataDumpToDB(Self,*args,**kwargs):
        pass

class SystemDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = SystemDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            object.update(**kwargs)
        except SystemDeviceInfo.DoesNotExist:
            SystemDeviceInfo.objects.create(**kwargs)
        


class NetworkDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = NetworkDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            object.update(**kwargs)
        except NetworkDeviceInfo.DoesNotExist:
            NetworkDeviceInfo.objects.create(**kwargs)


class WifiDumpData(MQTTDumpData):

    def mqttDataDumpToDB(Self,*args,**kwargs):
        try:
            object = WifiDeviceInfo.objects.get(device_id=kwargs['device_id']) 
            object.update(**kwargs)
        except WifiDeviceInfo.DoesNotExist:
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