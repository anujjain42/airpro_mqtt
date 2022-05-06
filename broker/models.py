import uuid
from django.db import models

# Create your models here.

class Device(models.Model):
    id              = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False) 
    device_id       = models.CharField(max_length=255, unique=True)
    serial_number   = models.CharField(max_length=255, blank=True, null=True)
    mac_address     = models.CharField(max_length=255, blank=True, null=True)
    alpn            = models.CharField(max_length=255, blank=True, null=True)
    type            = models.CharField(max_length=255, blank=True, null=True)
    created_on      = models.DateTimeField(auto_now_add=True)
    updated_on      = models.DateTimeField(auto_now=True)

    def __str__(self) -> str:
        return self.device_id

class BaseModel(models.Model):
    id              = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)
    device_id       = models.ForeignKey(Device,on_delete=models.CASCADE)    
    type            = models.CharField(max_length=255, blank=True, null=True)
    flag            = models.CharField(max_length=255, blank=True, null=True)
    ver             = models.CharField(max_length=255, blank=True, null=True)
    timestamp       = models.CharField(max_length=255, blank=True, null=True)
    token           = models.CharField(max_length=255, blank=True, null=True)   
    ip_address      = models.GenericIPAddressField()
    status          = models.CharField(max_length=255, blank=True, null=True)
    request_Id      = models.CharField(max_length=255, blank=True, null=True)
    updated_time    = models.DateTimeField(auto_now=True)
    data            = models.JSONField()

    class Meta:
        abstract=True

class WifiDeviceInfo(BaseModel):
    pass

    def __str__(self) -> str:
        return str(self.device_id.mac_address)

class NetworkDeviceInfo(BaseModel):
    pass

    def __str__(self) -> str:
        return str(self.device_id.mac_address)

class SystemDeviceInfo(BaseModel):
    pass

    def __str__(self) -> str:
        return str(self.device_id.mac_address)

class BrokerDetail(models.Model):
    broker_ip = models.GenericIPAddressField()
    port = models.IntegerField(default=1883)
    username = models.CharField(max_length=255,unique=True)
    password = models.CharField(max_length=255)
    server_topic = models.CharField(max_length=255)

    def __str__(self) -> str:
        return self.username

class BrokerDeviceTopic(models.Model):
    broker = models.ForeignKey(to=BrokerDetail,on_delete=models.SET_NULL,blank=True, null=True)
    device = models.ForeignKey(to=Device,on_delete=models.SET_NULL,blank=True, null=True)
    device_topic = models.CharField(max_length=255)

    def __str__(self) -> str:
        return self.device_topic