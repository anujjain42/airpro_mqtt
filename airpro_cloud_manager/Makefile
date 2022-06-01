CC= gcc
AIRPRO_CLOUD_MANAGER_SRC:= main.c device_discovery.c mqtt.c msg_dev_json.c dummy_wifi_manager.c

LDFLAGS += -lmosquitto -ljson-c -lcurl

all: clean
	$(CC) $(AIRPRO_CLOUD_MANAGER_SRC) $(CFLAGS) $(LDFLAGS) -lpthread -o airpro_cloud_manager
clean:
	# rm -rf airpro_cloud_manager

