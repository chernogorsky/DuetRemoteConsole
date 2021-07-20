#define TEMPLATE_PLACEHOLDER '`'
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//#include <FS.h>
//#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

const char* host = "esp32";

//WebServer httpServer(80);
AsyncWebServer httpAsyncServer(80);


void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  if(!index){
    Serial.printf("UploadStart: %s\n", filename.c_str());
    if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
    }
  }
  if (Update.write(data, len) != len) {
       Update.printError(Serial);
  }
  if(final){
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", index+len);
    } else {
        Update.printError(Serial);
    }
  }
}

String serverIndex_processor(const String& var)
{
  String result = "";
  if (var == "wlan") {
      result = "Some Wlan";
  } else if (var == "pip") {
      result = "SomeIP";
  }
  return result;
//  if(var == "HELLO_FROM_TEMPLATE")
//    return F("Hello world!");
//  return String();
}

void web_server_async_setup(){

  httpAsyncServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LITTLEFS, "/www/index.html", "");
  });

  httpAsyncServer.on("/serverIndex", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LITTLEFS, "/www/serverIndex.html", String(), false,serverIndex_processor);
  });

  httpAsyncServer.on("/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse(LITTLEFS, "/www/jquery.min.js", "text/javascript");
    response->addHeader("Content-Encoding", "gzip");
    request->send(response);
  });


  

  httpAsyncServer.on("/update", HTTP_POST, [](AsyncWebServerRequest * request) {
    request->send(200, "application/json", "{}");
    ESP.restart();
  }, handleUpload);

  httpAsyncServer.begin();
}



/* setup function */
void web_server_setup(void) {

//  serverJs = readFile(LITTLEFS,"/jquery.min.js");

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("mDNS responder started");
  

  web_server_async_setup();

}

void web_server_loop(void) {
//  httpServer.handleClient();
}


///Users/mikhaelc/Library/Arduino15/packages/esp32/hardware/esp32/1.0.6/tools/esptool.py --trace --chip esp32 --baud 921600 --port /dev/cu.SLAB_USBtoUART --before  default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 13172736 /var/folders/n7/39w88zzn1z72cp__f8ld398w0000gn/T/arduino_build_87934/M5ez-demo.littlefs.bin
