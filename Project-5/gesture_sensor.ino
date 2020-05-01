#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME		800
#define GES_QUIT_TIME			1000

void setup()
{
	uint8_t error = 0;

	Serial.begin(9600);
	Serial.println("\n PAJ7620U2 Gesture Controlled Robot Using Arduino");

	error = paj7620Init();
	if (error) 
	{
		Serial.print("INIT ERROR,CODE:");
		Serial.println(error);
	}
	else
	{
		Serial.println("Gesure Sensor Ready");
	}
	Serial.println("You Can Now Control Your Robot");
}

void loop()
{
	uint8_t data = 0, data1 = 0, error; 

	error = paj7620ReadReg(0x43, 1, &data);
	if (!error) 
	{
		switch (data) 
		{
			case GES_RIGHT_FLAG:
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_LEFT_FLAG) 
				{
					Serial.println("Right-Left");
				}
				else if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Right");
				}          
				break;
			case GES_LEFT_FLAG:
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_RIGHT_FLAG) 
				{
					Serial.println("Left-Right");
				}
				else if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Left");
				}          
				break;
				break;
			case GES_UP_FLAG:
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_DOWN_FLAG) 
				{
					Serial.println("Up-Down");
				}
				else if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Up");
				}
				break;
			case GES_DOWN_FLAG:
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_UP_FLAG) 
				{
					Serial.println("Down-Up");
				}
				else if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Down");
				}
				break;
			case GES_FORWARD_FLAG:
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Forward-Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				break;
			case GES_BACKWARD_FLAG:		  
				delay(GES_REACTION_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Backward-Forward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				break;
			case GES_CLOCKWISE_FLAG:
				Serial.println("Clockwise");
				break;
			case GES_COUNT_CLOCKWISE_FLAG:
				Serial.println("anti-clockwise");
				break;  
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG) 
				{
					Serial.println("wave");
				}
				break;
		}
	}
	delay(100);
}
