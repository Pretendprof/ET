/*
This code is designed for the enactive torch v3.0. It is designed for mapping 
sensor voltage values to distance in cm. This code is used to create a look up 
table of voltage to distance. The Sharp sensors have a non-linear response to 
distance so this lookup can be used to linearise the voltage values or determine
the appropriate value range for a particular study. 

Modified by: Maurice Lamb 10-26-12
*/

 //global variables
 int Sensor1, Sensor2;
 //int AccX, AccY, AccZ;
 //int MotorValue = 0;
 int ToneFrequency = 500;
 // variable for time keeping
 unsigned long time;
 
 //pin input and output constants
 //const int MotorOutputPin = 9; //digital pin 4 - PWM OUTPUT
 const int SpeakerPin = 4; //digital pin 4 - OUTPUT
 const int ButtonPin = 8; //digital pin 8 - INPUT
 const int GreenLED = 7; //digital pin 7 - OUTPUT
 const int Sensor1Pin = 1; //analog input pin 1 - Long range sensor 20-150cm
 const int Sensor2Pin = 2; //analog input pin 2 - Short range sensor 8-80cm
 //const int AccXPin = 5; //analog input pin 5
 //const int AccYPin = 4; //analog input pin 4
 //const int AccZPin = 3; //analog input pin 3
 /*
 X axis is along the length of the ET
 Y axis across the width
 Z axis from bottom to top
 */
 
 //a high voltage on the LED pin switches it off
 #define LEDOFF HIGH
 #define LEDON LOW
 
 
 void setup()
 { 
  //Test baud speed, use fastest speed that doesn't introduce errors
  Serial.begin(115200);
  //set up the digital inputs and outputs
  pinMode(SpeakerPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  pinMode(GreenLED, OUTPUT);
  playTone();
 }
 
 void loop()
 {
   /*
   This code takes data from the long range (20-150cm) and the short range sensor (8-80cm) and
   prints it to the serial terminal. 
   
   The USB programmer will only work when the device is switched OFF
   The arduino Pro-mini can draw power from the USB programmer but if
   the ET is switched ON when the programmer is inserted then there is a conflict
   between the USB programmer and the bluetooth device
   (Both share the same Tx and RX pins on the arduino)
   This means that the sensors are OFF when the device is communicating over USB
   and consequently any sensor data will be garbage.
   The speaker is powered by the arduino so it will work with the programmer attached
   */
   //digitalWrite(GreenLED, LEDOFF);
   // Record time in milliseconds
   //time = millis();
   
   Serial.println();
   delay(2000);

   while(!digitalRead(ButtonPin))
   {
    digitalWrite(GreenLED, LEDON);
   Sensor1 = analogRead(Sensor1Pin);
   Sensor2 = analogRead(Sensor2Pin);
   Serial.print(Sensor1);
   Serial.print("\t");
   Serial.println(Sensor2);
   delay(10);
    
    digitalWrite(GreenLED, LEDOFF);
    
 }
   
 }
 
 void playTone()
 {
   //play a quick tune on startup
   digitalWrite(GreenLED, LEDON);
   tone(SpeakerPin, 392); //Start with a tone
   delay(100);
   tone(SpeakerPin, 440); //Up a full tone
   delay(100);
   tone(SpeakerPin, 349); //Down a major third
   delay(100);
   tone(SpeakerPin, 174); //Now drop an octave
   delay(100);
   tone(SpeakerPin, 261); //Up a perfect fifth
   delay(100);
   noTone(SpeakerPin);
   digitalWrite(GreenLED, LEDOFF);
    
 }

