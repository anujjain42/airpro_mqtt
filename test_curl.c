#include <stdio.h>
#include <curl/curl.h>
 
int main(void)
{
  CURL *curl;
  CURLcode res;
 
  curl = curl_easy_init();
  struct curl_slist *headers=NULL;
  char* jsonObj = "{\"serial_number\":\"7821796924\",\"mac_address\":\"00:1b:21:bc:30:ae\",\"alpn\":\"2.5\",\"type\":\"6\"}";
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charset: utf-8");
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:8520/api/devices/");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl,CURLOPT_HTTPHEADER,headers);
    // curl_easy_setopt(curl, CURLOPT_USERPWD, "admin:dots@123");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj);
    // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
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
