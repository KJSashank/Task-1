# Task-1
Elec club tasks
# Project 1 - NO-Contact Infrared Thermometer
![thermometer](https://github.com/KJSashank/Task-1/blob/master/Task%201.12.jpg)

![circuit](https://github.com/KJSashank/Task-1/blob/master/task%201.1.jpg)

Materials - Arduino Nano,touch button, resistance (5K ohm, 200 ohm), 5V laser emitter, dual control switch, OLED 0.96 "screen, GY-906 temperature MLX90614 sensor, 9V battery

We use 3d printed parts to build a physical gun kind thermometer, which gives the surface temperature of the object in front of it without any physical contact and with an approx error of 0.5 degree C. We connect the circuit as shown in the figure the led is not mandatory, and the code is uploaded below. This can be used after installing the Adafruits SSD1306 and Adafruits MLX90614 libraries for Arduino IDE.

Code:

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int Laser_Pin=5;  //Laser Pin
int buttonState = 0; 
const int buttonPin = 2;     // the number of the pushbutton pin

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("Adafruit MLX90614 test"); 
  
  pinMode(Laser_Pin,OUTPUT);
  pinMode(buttonPin, INPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  

  display.clearDisplay();
  display.setRotation(2);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 35);
  display.println("Initializing Temp");
  display.display();
  delay(250);
  display.clearDisplay();

  mlx.begin(); 
  
}

void loop() {

  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  Serial.print("*C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(Laser_Pin, HIGH);
    
    display.clearDisplay();
    display.setTextSize(2);  //Size 2 means each pixel is 12 width and 16 high
    display.setCursor(25, 10);
    display.print(mlx.readObjectTempC());
    display.setCursor(95, 10);
    display.print("C");
    display.setTextSize(2);
    display.setCursor(25, 36);
    display.print(mlx.readObjectTempF());
    display.setCursor(95, 36);
    display.print("F");
    display.display();
    
    
  } else {
    // turn LED off:
    digitalWrite(Laser_Pin, LOW);
  
    display.clearDisplay();
    display.setTextSize(2);  //Size 2 means each pixel is 12 width and 16 high
    display.setCursor(35, 10);
    display.print("-----");
    display.setCursor(105, 10);
    display.print("");
    display.setTextSize(2);
    display.setCursor(35, 36);
    display.print("-----");
    display.setCursor(105, 36);
    display.print("");
    display.display();
    
    
  }

  Serial.println();
  delay(500);
}

# Project 2 - PID based line following bot 
A bot with 2 mtors on left and right side which follows a line sensed by QTR sensor. The errors are used wrt PID controller and the volocities of each wheel are given. The sensor pins arefrpm analog 0-5 of the arduino.

#include <AFMotor.h>    //Adafruit Motor Shield Library. First you must download and install AFMotor library
#include <QTRSensors.h> //Pololu QTR Sensor Library. First you must download and install QTRSensors library
  
AF_DCMotor motor1(1, MOTOR12_1KHZ ); //create motor #1 using M1 output on Motor Drive Shield, set to 1kHz PWM frequency
AF_DCMotor motor2(2, MOTOR12_1KHZ ); //create motor #2 using M2 output on Motor Drive Shield, set to 1kHz PWM frequency


#define KP 2 //experiment to determine this, start by something small that just makes your bot follow the line at a slow speed
#define KD 4 //experiment to determine this, slowly increase the speeds and adjust this value.
#define M1_minimum_speed 50  //minimum speed of the Motor1
#define M2_minimum_speed 50  //minimum speed of the Motor2
#define M1_maximum_speed 150 //max. speed of the Motor1
#define M2_maximum_speed 150 //max. speed of the Motor2
#define MIDDLE_SENSOR 4       //number of middle sensor used
#define NUM_SENSORS 5         //number of sensors used
#define TIMEOUT 2500          //waits for 2500 us for sensor outputs to go low
#define EMITTER_PIN 2         //emitterPin is the Arduino digital pin that controls whether the IR LEDs are on or off. Emitter is controlled by digital pin 2
#define DEBUG 0

//sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsRC qtrrc((unsigned char[]) {A4,A3,A2,A1,A0} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN);
  
unsigned int sensorValues [ NUM_SENSORS ] ;
  
void setup()
{

manual_calibration();

set_motors(0,0);
}
  
int lastError = 0;
int last_proportional = 0;
int integral = 0;
  
void loop()
{
unsigned int sensors[5];
int position = qtrrc.readLine(sensors); //get calibrated readings along with the line position
int error = position - 2000;// the position is centered at 2000, hence giving us a positive/negative error.

  
int motorSpeed = KP * error + KD * (error - lastError);// here KD is configured to have ds term in the de/ds, where de is error-last error
lastError = error;
  
int leftMotorSpeed = M1_minimum_speed + motorSpeed;
int rightMotorSpeed = M2_minimum_speed - motorSpeed;
  
// set motor speeds using the two motor speed variables above
set_motors(leftMotorSpeed, rightMotorSpeed);
}
  
void set_motors(int motor1speed, int motor2speed)
{
if (motor1speed > M1_maximum_speed ) motor1speed = M1_maximum_speed;
if (motor2speed > M2_maximum_speed ) motor2speed = M2_maximum_speed;
if (motor1speed < 0) motor1speed = 0; 
if (motor2speed < 0) motor2speed = 0; 
motor1.setSpeed(motor1speed); 
motor2.setSpeed(motor2speed);

motor1.run(FORWARD); 
motor2.run(FORWARD);
}

//calibrate for sometime by sliding the sensors across the line, or you may use auto-calibration instead
void manual_calibration() {
  
int i;
for (i = 0; i < 250; i++)
{
 
qtrrc.calibrate(QTR_EMITTERS_ON);
delay(20);
}
  
if (DEBUG) {
Serial.begin(9600);
for (int i = 0; i < NUM_SENSORS; i++)
{
Serial.print(qtrrc.calibratedMinimumOn[i]);
Serial.print(' ');
}
Serial.println();
  
for (int i = 0; i < NUM_SENSORS; i++)
{
Serial.print(qtrrc.calibratedMaximumOn[i]);
Serial.print(' ');
}
Serial.println();
Serial.println();
}
}
