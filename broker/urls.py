from rest_framework.routers import DefaultRouter
from broker.views import (
        WifiDeviceInfoViewSet, NetworkDeviceInfoViewSet, SystemDeviceInfoInfoViewSet, DeviceViewSet,
        UnregisteredDeviceViewSet, BrokerDeviceTopicViewSet
    )

router = DefaultRouter()
router.register('devices', DeviceViewSet)
router.register('unregistered-devices', UnregisteredDeviceViewSet)
router.register('wifi-devices', WifiDeviceInfoViewSet)
router.register('network-devices', NetworkDeviceInfoViewSet)
router.register('system-devices', SystemDeviceInfoInfoViewSet)
router.register('broker-devices', BrokerDeviceTopicViewSet)


urlpatterns=router.urls