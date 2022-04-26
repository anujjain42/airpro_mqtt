#include <stdio.h>
#include <curl/curl.h>
 
int main(void)
{
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  struct curl_slist *headers=NULL;
  // char* jsonObj = "{\"device_id\":\"36ys572dg\",\"serial_number\":\"7821796924\",\"mac_address\":\"00:1b:21:bc:30:ae\",\"ip_address\":\"127.0.0.1\",\"status\":\"panding\",\"request_Id\":\"00:1b:21:bc:30:ae\",\"updated_time\":\"2022-03-28T18:51:46.378Z\",\"data\":{\"radio\":{\"details\":{\"channel\":40,\"country\":0,\"disabled\":0,\"txpower\":25,\"rf_bw\":40,\"mode\":0,\"max_sta\":32,\"radio_type\":\"5GHz\",\"abc\":\"\"}}}}";
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charset: utf-8");
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://122.170.105.253:8520/api/wifi-devices/get_or_check_register_device/?alpn=asdad&mac=fdsad&sn=fdsafas&type=sadfa");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headers);
    curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:dots@123");
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}


// command
// gcc test.c -lcurl
