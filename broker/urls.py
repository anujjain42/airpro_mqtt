from rest_framework.routers import DefaultRouter
from broker.views import WifiDeviceInfoViewSet, NetworkDeviceInfoViewSet, SystemDeviceInfoInfoViewSet, DeviceViewSet

router = DefaultRouter()
router.register('devices', DeviceViewSet)
router.register('wifi-devices', WifiDeviceInfoViewSet)
router.register('network-devices', NetworkDeviceInfoViewSet)
router.register('system-devices', SystemDeviceInfoInfoViewSet)


urlpatterns=router.urls