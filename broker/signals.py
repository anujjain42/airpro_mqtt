import requests
import json
from django.conf import settings
from django.db.models.signals import post_save
from django.dispatch import receiver
import paho.mqtt.client as paho
from broker.customabstractmethod import publish_to_mqtt
from broker.models import WifiDeviceInfo, NetworkDeviceInfo, Device, BrokerDeviceTopic, BrokerDetail, SystemDeviceInfo

broker = "122.170.105.253"
port = 1883
ui_base_url = settings.UI_BASE_URL

NETWORK_UI_URL      = f"{ui_base_url}/organization/network-info//"
WIFI_UI_URL         = f"{ui_base_url}/organization/wifi-info//"
SYSTEM_UI_URL       = f"{ui_base_url}/organization/system-info//"
DEVICE_UI_URL       = f"{ui_base_url}/organization/device//"

headers             = {'Content-Type': 'application/json'}

@receiver(post_save, sender=NetworkDeviceInfo)
def send_config_mqtt_network_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    headers = {'Content-Type': 'application/json'}
    print("NETWORK_UI_URL",NETWORK_UI_URL)
    if not created:
        if len(broker_device_obj):
            broker_device_obj = broker_device_obj[0]
            publish_to_mqtt(
                instance.data, broker_device_obj.broker,
                broker_device_obj.port, broker_device_obj.device_topic
            )
            data = {"device_id":instance.device_id.device_id,**instance.data}
            data = json.dumps(data)
            requests.patch(NETWORK_UI_URL, data = data , headers=headers)
    else:
        data = {"device_id":instance.device_id.device_id,**instance.data}
        data = json.dumps(data)
        requests.post(NETWORK_UI_URL, data = data, headers=headers)


@receiver(post_save, sender=WifiDeviceInfo)
def send_config_mqtt_wifi_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    print("WIFI_UI_URL",WIFI_UI_URL)
    if not created:
        if len(broker_device_obj):
            broker_device_obj = broker_device_obj[0]
            publish_to_mqtt(
                instance.data, broker_device_obj.broker,
                broker_device_obj.port, broker_device_obj.device_topic
            ) 
            data = {"device_id":instance.device_id.device_id,**instance.data}
            requests.patch(WIFI_UI_URL, data = data, headers=headers)
    else:
        data = {"device_id":instance.device_id.device_id,**instance.data}
        requests.post(WIFI_UI_URL, data = data, headers=headers )


@receiver(post_save, sender=SystemDeviceInfo)
def send_config_mqtt_system_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    print("SYSTEM_UI_URL",SYSTEM_UI_URL)
    if not created:
        if len(broker_device_obj):
            print(broker_device_obj)
            broker_device_obj = broker_device_obj[0]
            publish_to_mqtt(
                instance.data, broker_device_obj.broker,
                broker_device_obj.port, broker_device_obj.device_topic
            )
            data = {"device_id":instance.device_id.device_id,**instance.data['system_info']}
            requests.patch(SYSTEM_UI_URL, data = instance.data, headers=headers)
    else:
        data = {"device_id":instance.device_id.device_id,**instance.data['system_info']}
        requests.post(SYSTEM_UI_URL, data = data, headers=headers)


@receiver(post_save, sender=Device)
def create_client_topic(sender,instance, created, **kwargs):
    if created:
        broker_obj = BrokerDetail.objects.get(id=1)
        data = {
            "broker":broker_obj, "device":instance, "device_topic":'airpro/device/'+instance.device_id
        }
        BrokerDeviceTopic.objects.create(**data)
        data = {"device_id":instance.device_id,"mqtt_status":True}
        requests.patch(DEVICE_UI_URL+str(instance.serial_number), data = data, headers=headers)