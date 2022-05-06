from django.db.models.signals import post_save, pre_save
from django.dispatch import receiver
import paho.mqtt.client as paho
from broker.models import WifiDeviceInfo, NetworkDeviceInfo, Device, BrokerDeviceTopic, BrokerDetail

broker = "122.170.105.253"
port = 1883

@receiver(post_save, sender=NetworkDeviceInfo)
def send_config_mqtt_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    if not created:
        if len(broker_device_obj):
            broker_device_obj = broker_device_obj[0]
            publisher = paho.Client("PUBLISHER")
            publisher.connect(broker_device_obj.broker,broker_device_obj.port)
            publisher.publish(broker_device_obj.device_topic,str(instance.data))

@receiver(post_save, sender=Device)
def create_client_topic(sender,instance, created, **kwargs):
    if created:
        broker_obj = BrokerDetail.objects.get(id=1)
        data = {
            "broker":broker_obj, "device":instance, "device_topic":'airpro/device/'+instance.device_id
        }
        BrokerDeviceTopic.objects.create(**data)