import requests
import json
from requests.auth import HTTPBasicAuth
from django.conf import settings
from django.db.models.signals import post_save
from django.dispatch import receiver
import paho.mqtt.client as paho
from broker.customabstractmethod import publish_to_mqtt
from broker.models import ClientInfo, WifiDeviceInfo, NetworkDeviceInfo, Device, BrokerDeviceTopic, BrokerDetail, SystemDeviceInfo

broker = "122.170.105.253"
port = 1883
ui_base_url = settings.UI_BASE_URL

NETWORK_UI_URL      = f"{ui_base_url}/network-info/"
WIFI_UI_URL         = f"{ui_base_url}/wifi-info/"
SYSTEM_UI_URL       = f"{ui_base_url}/system-info/"
DEVICE_UI_URL       = f"{ui_base_url}/device/"
DEVICE_DETAILS_UI_URL = f"{ui_base_url}/device-details/"
DEVICE_RADIO_URL    = f"{ui_base_url}/device-redio/"
SSID_URL            = f"{ui_base_url}/ssid-info/"
SSID_URL_DELETE     = f"{ui_base_url}delete-by-device-id/"
DEVICE_STATISTICS_URL = f"{ui_base_url}/device-statistics/"
CLIENT_URL          = f"{ui_base_url}/client-list/"



auth                = HTTPBasicAuth('admin@admin.com', 'dots@123')
headers             = {'Content-Type': 'application/json'}

@receiver(post_save, sender=NetworkDeviceInfo)
def send_config_mqtt_network_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    if not created:
        ...

    for ap_stats in instance.data['ap_stats']['ap_stats_list']:
        ap_stats_data = {
            "hardware_version":ap_stats['hw_version'],
            "firmware_version":ap_stats['fw_version'],
            "client":ap_stats['numsta'],
            "mac":instance.macaddr,
            "device_model":""
        }
        ap_device_details_data = {
            'ssid':ap_stats['ssid'],
            "mgmt_ip":ap_stats['mgmt_ipaddr'],
            "egress_ip":ap_stats['mgmt_ipaddr'],
            "status":ap_stats['status']
        }
        data = {"device_id":instance.device_id.device_id,**ap_stats_data}
        res = requests.post(DEVICE_STATISTICS_URL, data =json.dumps(data), headers=headers,auth=auth)
        data = {"device_id":instance.device_id.device_id,**ap_device_details_data}
        res = requests.post(DEVICE_DETAILS_UI_URL, data =json.dumps(data), headers=headers,auth=auth)
        print(res.json())


@receiver(post_save, sender=WifiDeviceInfo)
def send_config_mqtt_wifi_client(sender,instance, created, **kwargs):
    broker_device_obj = instance.device_id.brokerdevicetopic_set.filter(device=instance.device_id)
    print("WIFI_UI_URL",WIFI_UI_URL)
    if not created:
        if len(broker_device_obj):
            requests.delete(f"{SSID_URL_DELETE}{instance.device_id}/")
            requests.delete(f"{DEVICE_RADIO_URL}delete-by-device-id/{instance.device_id}/")
            broker_device_obj = broker_device_obj[0]
            
    count = ssid_count= 0
    for radio in instance.data['uci_configs']['radio_list']:
        count+=1
        data = {"device_id":instance.device_id.device_id,'radio_name':radio['radio_type'],'radio_json':radio,"radio_index":count}
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
            broker_device_obj = broker_device_obj[0]
            requests.delete(f"{SYSTEM_UI_URL}delete-by-device-id/{instance.device_id}/")
    data = {"device_id":instance.device_id.device_id,**instance.data['system_stats']['system_stats_list']}
    requests.post(SYSTEM_UI_URL, data = json.dumps(data), headers=headers)


@receiver(post_save,sender=ClientInfo)
def send_config_mqtt_client(sender, instance, created, **kwagrs):
    if not created:
        requests.delete(f"{CLIENT_URL}delete-by-device-id/{instance.device_id}/")
    
    for sta_stats in instance.data['sta_stats']['sta_stats_list']:
        data = {"device_id":instance.device_id.device_id,"client_data":sta_stats}
        res = requests.post(CLIENT_URL, data = json.dumps(data), headers=headers)


@receiver(post_save, sender=Device)
def create_client_topic(sender,instance, created, **kwargs):
    if created:
        broker_obj = BrokerDetail.objects.get(id=1)
        data = {
            "broker":broker_obj, "device":instance, "device_topic":'airpro/device/'+instance.device_id
        }
        BrokerDeviceTopic.objects.create(**data)
        data = {"device_id":instance.device_id,"mqtt_status":True, 'status':'Online'}
        res = requests.patch(f'{DEVICE_UI_URL}{str(instance.serial_number)}/', data = json.dumps(data), headers=headers,auth=auth)