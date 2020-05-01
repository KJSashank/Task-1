char t;

void setup() {
pinMode(13,OUTPUT); 
pinMode(12,OUTPUT); 
pinMode(11,OUTPUT); 
Serial.begin(9600);

}

void loop() {
if(Serial.available()){
t = Serial.read();
Serial.println(t);
}

if(t == 'R'){ 
digitalWrite(13,HIGH);
}
else if(t == 'D'){ 
digitalWrite(13,LOW);
}
else if(t == 'Y'){ 
digitalWrite(12,HIGH);
}
else if(t == 'L'){ 
digitalWrite(12,LOW);
}
else if(t == 'G'){ 
digitalWrite(11,HIGH);
}
else if(t == 'N'){ 
digitalWrite(11,LOW);
}
delay(1000);
}

   
