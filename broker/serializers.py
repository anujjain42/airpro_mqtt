from rest_framework.serializers import ModelSerializer
from broker.models import *

class WifiDeviceInfoSerializer(ModelSerializer):

    class Meta:
        model = WifiDeviceInfo
        fields = '__all__'

class NetworkDeviceInfoSerializer(ModelSerializer):

    class Meta:
        model = NetworkDeviceInfo
        fields = '__all__'


class SystemDeviceInfoSerializer(ModelSerializer):

    class Meta:
        model = SystemDeviceInfo
        fields = '__all__'


class DeviceRegisterSerializer(ModelSerializer):
    class Meta:
        model = Device
        fields = '__all__'

class BrokerDetailsSerializer(ModelSerializer):
    class Meta:
        model = BrokerDetail
        fields = '__all__'

