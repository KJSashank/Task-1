#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

const char *ssid = "SSID";
const char *password = "Password";

long utcOffsetInSeconds = -4*60*60; // Initializing with New York Time, UTC -4 Hours
int h,m,s;
int q=1;
int activate = 1;

unsigned long previousMillis = 0; 
const long interval = 1000; 


char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
Serial.begin(115200);
pinMode(2, INPUT);
lcd.begin();
WiFi.begin(ssid, password);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Connecting Wifi");
lcd.setCursor(0, 1);

while ( WiFi.status() != WL_CONNECTED ) {
delay ( 500 );
lcd.print(".");
Serial.print ( "." );
}
timeClient.begin();
lcd.clear();
}

void loop() {

unsigned long currentMillis = millis();

int button_pressed = digitalRead(2);
if (button_pressed == HIGH){
q++;
if (q >= 16){
q=1;}
activate = 1;
delay(300);
lcd.clear();
}
Serial.println(q);
Serial.println(utcOffsetInSeconds);


if (activate == 1){

switch (q)
{
case 1: 
lcd.setCursor(4, 0);
lcd.print("New York");
utcOffsetInSeconds = -4*60*60; //-4 UTC
break;
case 2: 
lcd.setCursor(5, 0);
lcd.print("Mumbai");
utcOffsetInSeconds = 5*60*60+60*30; // +5.5 UTC
break;
case 3: 
lcd.setCursor(5, 0);
lcd.print("Paris");
utcOffsetInSeconds = 2*60*60; // +2 UTC
break;
case 4: 
lcd.setCursor(5, 0);
lcd.print("London");
utcOffsetInSeconds = 1*60*60; // +1 UTC
break;
case 5: 
lcd.setCursor(5, 0);
lcd.print("Tokyo");
utcOffsetInSeconds = 9*60*60; // +9 UTC
break;
case 6: 
lcd.setCursor(4, 0);
lcd.print("Beijing");
utcOffsetInSeconds = 8*60*60; // +8 UTC
break;
case 7: 
lcd.setCursor(3, 0);
lcd.print("Frankfurt");
utcOffsetInSeconds = 2*60*60; // +2 UTC
break;
case 8: 
lcd.setCursor(5, 0);
lcd.print("Dubai");
utcOffsetInSeconds = 4*60*60; // +4 UTC
break;
case 9: 
lcd.setCursor(4, 0);
lcd.print("Toronto");
utcOffsetInSeconds = -4*60*60; // -4 UTC 
break;
case 10: 
lcd.setCursor(4, 0);
lcd.print("Vancouver");
utcOffsetInSeconds = -7*60*60; // -7 UTC
break;
case 11: 
lcd.setCursor(5, 0);
lcd.print("Sydney");
utcOffsetInSeconds = 10*60*60; // +10 UTC
break;
case 12: 
lcd.setCursor(4, 0);
lcd.print("Jakarta");
utcOffsetInSeconds = 7*60*60; // +7 UTC
break;
case 13: 
lcd.setCursor(2, 0);
lcd.print("Kuala Lumpur");
utcOffsetInSeconds = 8*60*60; // +8 UTC
break;
case 14: 
lcd.setCursor(4, 0);
lcd.print("Istanbul");
utcOffsetInSeconds = 3*60*60; // +3 UTC
break;
case 15: 
lcd.setCursor(4, 0);
lcd.print("Cape Town");
utcOffsetInSeconds = 2*60*60; // +2 UTC
break;
}
activate = 0;
}


if (currentMillis - previousMillis >= interval) {
previousMillis = currentMillis;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

timeClient.forceUpdate();
h = timeClient.getHours();
m = timeClient.getMinutes();
s = timeClient.getSeconds();
Serial.print(daysOfTheWeek[timeClient.getDay()]);
Serial.print(", ");
Serial.print(timeClient.getHours());
Serial.print(":");
Serial.print(timeClient.getMinutes());
Serial.print(":");
Serial.println(timeClient.getSeconds());
//Serial.println(timeClient.getFormattedTime());

lcd.setCursor(2, 1);
lcd.print(daysOfTheWeek[timeClient.getDay()]);
lcd.setCursor(6, 1);
if (h <= 9){
lcd.print("0");
lcd.print(h);}
else{
lcd.print(h);}
lcd.print(":");
if (m <= 9){
lcd.print("0");
lcd.print(m);}
else{
lcd.print(m);}
lcd.print(":");
if (s <= 9){
lcd.print("0");
lcd.print(s);}
else{
lcd.print(s);}
}

}