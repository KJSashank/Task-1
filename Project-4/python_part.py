
import pyautogui
import serial                                 # add Serial library for Serial communication
import time
c=0
Arduino_Serial = serial.Serial('com9',9600)  #Create Serial port object called arduinoSerialData
while(1):
    if(Arduino_Serial.read()=='1'):
        pyautogui.keyDown('space')
        time.sleep(.1)
        pyautogui.keyUp('space')
    
