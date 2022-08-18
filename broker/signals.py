import requests
import json
from requests.auth import HTTPBasicAuth
from django.conf import settings
from django.db.models.signals import post_save
from django.dispatch import receiver
import paho.mqtt.client as paho
from broker.customabstractmethod import publish_to_mqtt
from broker.models import WifiDeviceInfo, NetworkDeviceInfo, Device, BrokerDeviceTopic, BrokerDetail, SystemDeviceInfo

broker = "122.170.105.253"
port = 1883
ui_base_url = settings.UI_BASE_URL

NETWORK_UI_URL      = f"{ui_base_url}/network-info/"
WIFI_UI_URL         = f"{ui_base_url}/wifi-info/"
SYSTEM_UI_URL       = f"{ui_base_url}/system-info/"
DEVICE_UI_URL       = f"{ui_base_url}/device/"
DEVICE_RADIO_URL       = f"{ui_base_url}/device-redio/"
SSID_URL            = f"{ui_base_url}/ssid-info/"



auth                = HTTPBasicAuth('admin@admin.com', 'dots@123')
headers             = {'Content-Type': 'application/json'}

@receiver(post_save, sender=NetworkDeviceInfo)
def send_config_mqtt_network_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    headers = {'Content-Type': 'application/json'}
    print("NETWORK_UI_URL",NETWORK_UI_URL)
    if not created:
        if len(broker_device_obj):
            broker_device_obj = broker_device_obj[0]
            # publish_to_mqtt(
            #     instance.data, broker_device_obj.broker,
            #     broker_device_obj.port, broker_device_obj.device_topic
            # )
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
            count = ssid_count= 0
            for radio in instance.data['uci_configs']['radio_list']:
                count+=1
                data = {"device_id":instance.device_id.device_id,**radio,"radio_index":count}
                res = requests.post(DEVICE_RADIO_URL, data = json.dumps(data), headers=headers)
                for ssid in radio['vap_list']:
                    ssid_count +=1 
                    ssid_data = {"device_id":instance.device_id.device_id,'radio':res.json()['id'],**ssid,'ssid_index':ssid_count}                    
                    requests.post(SSID_URL, data = json.dumps(ssid_data), headers=headers)           
    else:
        count = ssid_count= 0
        for radio in instance.data['uci_configs']['radio_list']:
            count+=1
            data = {"device_id":instance.device_id.device_id,**radio,"radio_index":count}
            res = requests.post(DEVICE_RADIO_URL, data = json.dumps(data), headers=headers)
            for ssid in radio['vap_list']:
                ssid_count +=1 
                ssid_data = {"device_id":instance.device_id.device_id,'radio':res.json()['id'],**ssid,'ssid_index':ssid_count}                    
                requests.post(SSID_URL, data = json.dumps(ssid_data), headers=headers)


@receiver(post_save, sender=SystemDeviceInfo)
def send_config_mqtt_system_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    print("SYSTEM_UI_URL",SYSTEM_UI_URL)
    if not created:
        if len(broker_device_obj):
            print(len(broker_device_obj))
            broker_device_obj = broker_device_obj[0]
            # publish_to_mqtt(
            #     instance.data, broker_device_obj.broker,
            #     broker_device_obj.port, broker_device_obj.device_topic
            # )
            data = {"device_id":instance.device_id.device_id,**instance.data['system_stats']['system_stats_list']}
            requests.patch(f'{SYSTEM_UI_URL}5131592c-eb59-4fad-9b50-e638ca8c0ab0/', data = json.dumps(data), headers=headers)
    else:
        data = {"device_id":instance.device_id.device_id,**instance.data['system_stats']['system_stats_list']}
        requests.post(SYSTEM_UI_URL, data = json.dumps(data), headers=headers)


@receiver(post_save, sender=Device)
def create_client_topic(sender,instance, created, **kwargs):
    if not created:
        broker_obj = BrokerDetail.objects.get(id=1)
        data = {
            "broker":broker_obj, "device":instance, "device_topic":'airpro/device/'+instance.device_id
        }
        BrokerDeviceTopic.objects.create(**data)
        data = {"device_id":instance.device_id,"mqtt_status":True}
        res = requests.patch(f'{DEVICE_UI_URL}{str(instance.serial_number)}/', data = json.dumps(data), headers=headers,auth=auth)