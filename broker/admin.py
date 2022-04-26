from django.contrib import admin

from broker.models import WifiDeviceInfo, NetworkDeviceInfo, SystemDeviceInfo, Device, BrokerDetails
# Register your models here.

admin.site.register(WifiDeviceInfo)
admin.site.register(NetworkDeviceInfo)
admin.site.register(SystemDeviceInfo)
admin.site.register(Device)
admin.site.register(BrokerDetails)