#include "common.h"
#include "cm.h"
#include <curl/curl.h>

int airpro_validate_http_resp(char *http_msg, int http_msg_len)
{
    return 0;
}

static size_t airpro_recv_http_resp_cb(void *data, size_t size, size_t nmemb, void *userp)
{
    struct airpro_cm_handle *cm = (struct airpro_cm_handle *)userp;
    struct http_device_discovery *hdd = &cm->hdd;
    size_t realsize = size * nmemb;
 
    memset(hdd->http_msg, 0, sizeof(hdd->http_msg)); 
    memcpy(hdd->http_msg, data, realsize);
    hdd->http_msg_len += realsize;

    return realsize;
}

int airpro_send_recv_device_discovery(struct airpro_cm_handle *cm)
{
    struct curl_slist *headers = NULL;
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    char* jsonObj = "{\"serial_number\":\"7821796924\",\"mac_address\":\"00:1b:21:bc:30:ae\",\"alpn\":\"2.5\",\"type\":\"6\"}";
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charset: utf-8");
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://122.170.105.253:8520/api/devices/");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, airpro_recv_http_resp_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)cm);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return -1;
        }
        curl_easy_cleanup(curl);
    }
    res = airpro_validate_http_resp(cm->hdd.http_msg, cm->hdd.http_msg_len);
    if (res != 0) {
        return -1;
    }

    return 0;
}
