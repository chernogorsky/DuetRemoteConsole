#include <WiFi.h>
//#include <WiFiClient.h>
//#include <WiFiAP.h>

char wlanName[] = "DuetBuddyAP";
char wlanPSK[] = "1234567890";

void wifi_setup_ap(){
  WiFi.mode(WIFI_AP);
  WiFi.softAP(wlanName,wlanPSK);
  Serial.println("WiFi AP started ...");
//  WiFi.softAP(const char* ssid, const char* passphrase, int channel, int ssid_hidden, int max_connection)
}

String wifi_get_ip(){
  //WiFi.localIP().toString()
  return WiFi.softAPIP().toString();
}
