
#include <HTTPClient.h>
#include <WiFi.h>
#include <M5Core2.h>
#include "Arduino.h"
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>

#ifndef WIFI_SSID
#define WIFI_SSID "XXXXXX"  // WiFi SSID (2.4GHz only)
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "XXXXX"  // WiFiパスワード
#endif

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

HTTPClient http;
WiFiClientSecure  client;

JSONVar obj1;

JSONVar MyAuthInfo;
bool getAuth =false;

String Token;
String url = "https://192.168.1.49:8443/identity-manager/api/v1/auth/token";
String CtrlxBasicUrl="https://192.168.1.49:8443/automation/api/v1/";
String Url1r32Real=CtrlxBasicUrl+"plc/app/Application/realtime_data/GVL/data/r32Real";
String Url1i16Int=CtrlxBasicUrl+"plc/app/Application/realtime_data/GVL/data/i16Int";

void setup() {
      Serial.begin(115200);
      M5.begin();
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(10, 10);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(1.9);
      M5.Lcd.printf("Ethernet Interface:inited:)!\n");
      WiFi.mode(WIFI_STA);
      delay(500);
      WiFi.config(local_IP, gateway, subnet);
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) 
        {
          delay(1000);
          Serial.println("Connecting...");
        }
      M5.Lcd.printf("IP is 192.168.1.184\n");

}

void loop() {
    client.setInsecure();
    M5.Lcd.fillRect(0,0,300,300,BLACK);  
    if (!getAuth){  
    obj1["name"]="boschrexroth";
    obj1["password"]="boschrexroth";
    

    String result=JSON.stringify(obj1);    
    Serial.println(result);
    M5.Lcd.drawString("[HTTP] begin...\n",0,0);

    http.addHeader("Content-Type", "application/json");
    
    http.begin(client,url);
    
    int responseCode = http.POST(result);
    M5.Lcd.drawString("[HTTP] POST...\n",0,20);
    M5.Lcd.drawString(responseCode+"\n",0,40);

    if (responseCode == 201){
        getAuth=true;
        String body = http.getString();
        Serial.println(body);
        JSONVar AuthInfo;
        AuthInfo=JSON.parse(body);
        
        String Token=AuthInfo["access_token"];
        MyAuthInfo["Authorization"]=Token;
        
    }
    

    Serial.println(responseCode);
  

    http.end();

    ;
    }
    
    bool Error=false;

    if (getAuth){    
        M5.Lcd.fillRect(0,0,300,300,BLACK);    
        String s3=MyAuthInfo["Authorization"];               
        String myToken="Bearer "+s3;

        http.addHeader("Authorization", myToken);
        
        String url1=Url1r32Real;
        http.begin(client,url1);
        int responseCode = http.GET();
        M5.Lcd.setTextColor(ORANGE);
        M5.Lcd.drawString("[HTTP] GET...\n",0,0);
        M5.Lcd.drawString("r32Real\n",0,10);
        
        M5.Lcd.drawString(String(responseCode)+"\n",0,20);
      
      if (responseCode == 200){
          String data = http.getString();
          
          Serial.println(data);
          Serial.println("Sent1");
          M5.Lcd.drawString(data,0,40);
        }
      else{
        getAuth=false;
        Error=true;

      }
      if (!Error){
        
        String url2=Url1i16Int;
        http.begin(client,url2);
        Serial.println("Sent2");
        responseCode = http.GET();
        
        M5.Lcd.setTextColor(YELLOW);
        M5.Lcd.drawString("[HTTP] GET...\n",0,60);
        M5.Lcd.drawString("i16Int\n",0,70);
        M5.Lcd.drawString(String(responseCode)+"\n",0,80);

      if (responseCode == 200){
          String data = http.getString();

          M5.Lcd.drawString(data,0,100);
          Serial.println(data);
        }    
       }
      }

    delay(1000);    
    http.end();
    

}
