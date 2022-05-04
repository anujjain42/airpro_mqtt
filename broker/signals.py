from django.db.models.signals import post_save
from django.dispatch import receiver
import paho.mqtt.client as paho
from broker.models import WifiDeviceInfo, NetworkDeviceInfo

broker = "122.170.105.253"
port = 1883

@receiver(post_save, sender=NetworkDeviceInfo)
def send_config_mqtt_client(sender,instance, created, **kwargs):
    if not created:
        publisher = paho.Client("PUBLISHER")
        publisher.connect(broker,port)
        print("airpro/device_"+str(instance.device_id.mac_address))
        publisher.publish("airpro/device_"+str(instance.device_id.mac_address),str(instance.data))