
void setup() {
  // Serial port init
  Serial.begin(115200);
  
  pixel_setup();
  server_setup();
}
 
void loop() {
  ligth_mode();
  server_run();
}
