
#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SEND_INTERVAL 5000    // our device will send data every 5 seconds

const char* ssid = "EmbrayonicWifi06";
const char* password = "hhdkKJDkfhld98";
const char* mqtt_server = "broker.mqtt-dashboard.com";

//  Variables
int Threshold = 550;          // Determine which Signal to "count as a beat" and which to ignore.                
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

void setup() 
{   
  // Configure the PulseSensor object
  pulseSensor.analogInput(A0);   
  pulseSensor.setThreshold(Threshold);   

  // connect Wifi and setup Mqtt
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


long curTime = 0;
int myBPM = 0;
void loop() 
{
   myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".

   // check is there is a beat                                            
  if (pulseSensor.sawStartOfBeat()) 
  {         
    // check if its time to send message
    if (millis() - curTime > SEND_INTERVAL)
    {
      snprintf (msg, 75, "Heart beats per minute #%ld", myBPM);
      client.publish("outTopic", msg);
      curTime = millis();
    }
  }

}



void setup_wifi() 
{
  delay(10);
  // We start by connecting to a WiFi network
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    // Attempt to connect
    if (client.connect("SmartBracelet")) {
      // Once connected, publish an announcement...
      client.publish("mySmartBracelet", "ah ha");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      delay(5000);
    }
  }
}