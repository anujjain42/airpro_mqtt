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

class UnregisteredDeviceSerializer(ModelSerializer):
    class Meta:
        model = UnregisteredDevice
        fields = '__all__'

class BrokerDeviceTopicSerializer(ModelSerializer):
    class Meta:
        model = BrokerDeviceTopic
        fields = '__all__'
        depth = 1