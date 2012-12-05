 
 //global variables
 int Sensor1, Sensor2;
 int AccX, AccY, AccZ;
 int MotorValue = 0;
 int ToneFrequency = 500;
 
 //pin input and output constants
 const int MotorOutputPin = 9; //digital pin 4 - PWM OUTPUT
 const int SpeakerPin = 4; //digital pin 4 - OUTPUT
 const int ButtonPin = 8; //digital pin 8 - INPUT
 const int GreenLED = 7; //digital pin 7 - OUTPUT
 const int Sensor1Pin = 1; //analog input pin 1 - Long range sensor 20-150cm
 const int Sensor2Pin = 2; //analog input pin 2 - Short range sensor 8-80cm
 const int AccXPin = 5; //analog input pin 5
 const int AccYPin = 4; //analog input pin 4
 const int AccZPin = 3; //analog input pin 3
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
   This code takes data from the long range (20-150cm) sensor only
   and maps it to the motor which has a vibration intensity range of 0-255
   All sensor data including accelerometers is printed over the serial port.
   The sensor reading is also mapped to the speaker if the button is pressed
   (closer = higher pitch) and the LED lights up when the button is pressed.
   
   The USB programmer will only work when the device is switched OFF
   The arduino Pro-mini can draw power from the USB programmer but if
   the ET is switched ON when the programmer is inserted then there is a conflict
   between the USB programmer and the bluetooth device
   (Both share the same Tx and RX pins on the arduino)
   This means that the sensors are OFF when the device is communicating over USB
   and consequently any sensor data will be garbage.
   The speaker is powered by the arduino so it will work with the programmer attached
   */
   Sensor1 = analogRead(Sensor1Pin);
   // wait 10 milliseconds for the analog-to-digital converter
   // to settle after the last reading:
   delay(10);
   Sensor2 = analogRead(Sensor2Pin);
   delay(10);
   AccX = analogRead(AccXPin);
   delay(10);
   AccY = analogRead(AccYPin);
   delay(10);
   AccZ = analogRead(AccZPin);
   
   MotorValue = map(Sensor1, 0, 600, 0, 255);
   analogWrite(MotorOutputPin, MotorValue);
   
   
   // print the sensor data and accellerometer values:
   Serial.print(Sensor1);
   Serial.print("\t");
   Serial.print(Sensor2);
   Serial.print("\t");
   Serial.print(AccX);
   Serial.print("\t");
   Serial.print(AccY);
   Serial.print("\t");
   Serial.print(AccZ);
   Serial.print("\t");
   Serial.println(MotorValue);
   
   //Map the sensor reading to the speaker if the button is pressed
   //illuminate the Green LED if the button is pressed
   if(!digitalRead(ButtonPin))
   {
     ToneFrequency = map(Sensor1, 0, 600, 32, 1046);
     tone(SpeakerPin, ToneFrequency);
     digitalWrite(GreenLED, LEDON);
   }
   else
   {
     noTone(SpeakerPin);
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
