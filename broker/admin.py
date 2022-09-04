from django.contrib import admin

from broker.models import (
    WifiDeviceInfo, NetworkDeviceInfo, SystemDeviceInfo, Device, 
    BrokerDetail, BrokerDeviceTopic, UnregisteredDevice, ClientInfo
)
# Register your models here.

admin.site.register(WifiDeviceInfo)
admin.site.register(NetworkDeviceInfo)
admin.site.register(SystemDeviceInfo)
admin.site.register(Device)
admin.site.register(BrokerDetail)
admin.site.register(BrokerDeviceTopic)
admin.site.register(ClientInfo)

@admin.register(UnregisteredDevice)
class UnregisteredDeviceAdmin(admin.ModelAdmin):
    list_display = ['serial_number','alias','status']