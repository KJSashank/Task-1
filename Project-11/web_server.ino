#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <AutoConnect.h>

ESP8266WebServer Server;

AutoConnect       Portal(Server);
AutoConnectConfig Config;       // Enable autoReconnect supported on v0.9.4

void huePage(){
  //HTML with the HUE selector and buttom to access to the autconnect menu
  String  content =
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "<link rel=\"icon\" href=\"data:,\">"
    "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">"
    "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jscolor/2.0.4/jscolor.min.js\"></script>"
    "</head>"
    "<body>"
    "<div class=\"container\"><div class=\"row\"><h1>ESP Color Picker</h1></div>"
    "<a class=\"btn btn-primary btn-lg\" href=\"#\" id=\"change_color\" role=\"button\">Change Color</a> "
    "<input class=\"jscolor {onFineChange:'update(this)'}\" id=\"rgb\"></div>"
    "<script>function update(picker) {document.getElementById('rgb').innerHTML = Math.round(picker.rgb[0]) + ', ' +  Math.round(picker.rgb[1]) + ', ' + Math.round(picker.rgb[2]);"
    "document.getElementById(\"change_color\").href=\"?r=\" + Math.round(picker.rgb[0]) + \"&g=\" +  Math.round(picker.rgb[1]) + \"&b=\" + Math.round(picker.rgb[2]) + \"&\";}"
    "</script>"
    "<p></p><p style=\"padding-top:15px;text-align:center\">" AUTOCONNECT_LINK(COG_24) "</p>"
    "</body>"
    "</html>";

  //Send periodically the sever content
  Server.send(200, "text/html", content);
  
  if(Server.args()>0){
    //If the header of the http changes, we will save the red, green and blue value
    for(int i=0;i<3;i++){
      //Save the value of the colours
      rgb[i]=Server.arg(i).toInt();
    }
    //If the light is on, update the color
    
      pixels_on();
   
  }
}


void server_setup(){
  //Set-up web sever and autoconnect wifi utility.
  Config.autoReconnect = true;
  Portal.config(Config);

  Server.on("/", huePage);
    // Establish a connection with an autoReconnect option.
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  }
}

void server_run(){
  Portal.handleClient();
}
