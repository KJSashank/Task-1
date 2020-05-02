#include <NewPing.h>

// HC-SR04 configuration parameters
#define trigger_pin 5
#define echo_pin 4
#define max_distance 30

bool ligth_on = false;

NewPing sonar(trigger_pin, echo_pin, max_distance);

void ligth_mode(){
  int sonar_distance;
  if(distance_measure(&sonar_distance)){
    if(sonar_distance < 5){
      Serial.println("Apagar");
      pixels_off();
      ligth_on = false;
    }
    else if(sonar_distance > 5){
      if(!ligth_on){
        pixels_on();
        Serial.println("ON");
        ligth_on = true;
        delay(500);
      }
      else{
        brightness_select(sonar_distance);
      }
    }
  }
}

bool distance_measure(int *measure){
  int measure_aux = sonar.ping_cm();
  if(measure_aux > 0){
    for(int i=0;i<2;i++){
      if(measure_aux != sonar.ping_cm()){
        return false;
      }
      delay(30);
    }
  }
  else{
    return false;
  }
  (*measure) = measure_aux;
  return true;
}
