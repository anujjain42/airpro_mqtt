import random, string 
from rest_framework.response import Response
from rest_framework.viewsets import ModelViewSet
from rest_framework.permissions import IsAuthenticated, AllowAny
from rest_framework import status
from broker.serializers import *
from broker import mqtt_server
# Create your views here.

class DeviceViewSet(ModelViewSet):
    queryset = Device.objects.all()
    serializer_class = DeviceRegisterSerializer
    http_method_names = ['post']
    permission_classes = [AllowAny]

    def create_device_id(self):
        device_id = ''.join(random.choices(string.digits, k = 10))
        if Device.objects.filter(device_id=device_id).exists():
            self.create_device_id()
            return
        return device_id

    def create(self, request, *args, **kwargs):
        data =  request.data
        obj = Device.objects.filter(**data) 
        broker_obj = BrokerDetail.objects.get(id=1)
        ser = BrokerDetailsSerializer(broker_obj).data
        if len(obj):
            device_id = obj[0].device_id
            ser['device_id'] = device_id         
            ser['client_topic'] = 'airpro/device/'+device_id  
        else:
            request.data['device_id'] = self.create_device_id()
            res = super().create(request, *args, **kwargs)
            ser['device_id'] = res.data['device_id']          
            ser['client_topic'] = 'airpro/device/'+res.data['device_id'] 
        return Response(ser,status=status.HTTP_200_OK)


class WifiDeviceInfoViewSet(ModelViewSet):
    queryset = WifiDeviceInfo.objects.all()
    serializer_class = WifiDeviceInfoSerializer
    http_method_names = ['get','post']
    permission_classes = [IsAuthenticated]


class NetworkDeviceInfoViewSet(ModelViewSet):
    queryset = NetworkDeviceInfo.objects.all()
    serializer_class = NetworkDeviceInfoSerializer
    http_method_names = ['get','post']
    permission_classes = [IsAuthenticated]

class SystemDeviceInfoInfoViewSet(ModelViewSet):
    queryset = SystemDeviceInfo.objects.all()
    serializer_class = SystemDeviceInfoSerializer
    http_method_names = ['get','post']
    permission_classes = [IsAuthenticated]