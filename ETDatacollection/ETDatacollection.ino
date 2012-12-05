/*
 Enactive Torch v3.0 (ET). 
 Data Collection Code v. 0.2
 Last Modified: 11/29/12
 By: Maurice Lamb
 
 This code is designed to output data for research. Each cycle of data collection
 is timestamped based on the internal clock of the Arudino. Sample rate can be adjusted
 by changing the delay between sensor readings. The button is used to signify the beginning
 or end of a trial and can be used to label data columns. This code is designed to allow for 
 ET to be run for a period of time and then stopped or interupped. It may also be run without
 ending by changing the 'duration' cariable to its maximum value.

 Any questions or problems with the code pleas email:lambmi@mail.uc.edu 
 
 Modified from Bill Bigge's code by: Maurice Lamb 10-26-12
 */

//global variables
int Sensor1, Sensor2;
int AccX, AccY, AccZ;
int MotorValue = 0;
int ToneFrequency = 500;

// Variables for time keeping:
unsigned long time; //time is used to keep track of time since ET turned on
unsigned long MillisREC; //Marks beginning of loop
unsigned long startDELAY = 0; //Define a timed delay before experiment begins. Delay time in Milliseconds.

//Duration defines unperturbed ET run time. This is amount of time motor will run in Milliseconds.
//for unperturbed run set duration = 4294967295
//Note that the program will take some time to run so add about 10 millisec. to account for drift
unsigned long duration = 4294967295;

unsigned long progSTART;  //These may be used to limit drift due to time each loop takes
unsigned long progEND;  //These may be used to limit drift due to time each loop takes
unsigned long progRUNTIME = 0;  //This is used to caclulate total time ET has been running in milliseconds

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
  //In order to change baud rate to lower speed must change speed of bluetooth module first.
  Serial.begin(115200);
  //set up the digital inputs and outputs
  pinMode(SpeakerPin, OUTPUT);
  pinMode(ButtonPin, INPUT);
  pinMode(GreenLED, OUTPUT);
  playTone();

  //Record time at end of setup
  MillisREC = millis();
}

void loop()
{
  /*
   This code takes data from the long range (20-150cm) sensor only
   and maps it to the motor which has a vibration intensity range of 0-255
   All sensor data including accelerometers is printed over the serial port.
   Sample rate can be adjusted by changing delay between readings. 
   When the button is pressed the torch will print column headers to until the button
   is released.
   
   The USB programmer will only work when the device is switched OFF
   The arduino Pro-mini can draw power from the USB programmer but if
   the ET is switched ON when the programmer is inserted then there is a conflict
   between the USB programmer and the bluetooth device
   (Both share the same Tx and RX pins on the arduino)
   This means that the sensors are OFF when the device is communicating over USB
   and consequently any sensor data will be garbage.
   The speaker is powered by the arduino so it will work with the programmer attached
   */

  // Pause motor for set amount of time before experiment begins 
  delay(startDELAY);

  //Begin ET activity for specified duration (see above)
  while(progRUNTIME <= duration)
  {  
    //progSTART = millis();  //This is for calculating the amount of time it takes for the loop to complete
    
    time = millis();  // Record time in milliseconds.
    // Microseconds will turn over 70 minutes after the Arduino. starts up. Milliseconds turns over after 50 days. 
   
    // Begin Sensor readings
    // Set delay to fit desired sample rate delay may be set as low as 10 microseconds but baud rate limits 
    // total possible samples. At 1 millisecond between each sensor read sample rate is about 250htz
    Sensor1 = analogRead(Sensor1Pin);
    delay(1);
    Sensor2 = analogRead(Sensor2Pin);
    delay(1);
    AccX = analogRead(AccXPin);
    delay(1);
    AccY = analogRead(AccYPin);
    delay(1);
    AccZ = analogRead(AccZPin);

    // Map sensor values to 0-255. Min and Max sensor values depend on sensor voltage output
    // See sensor datasheet for these values or choose appropriate range for ET purpose
    // Min and Max values for Sharp GP2Y0A02YK0F long range sensor are about 25-560 on ET3.0    
    // See lookup table below (TO BE ADDED AT A LATER DATE)
    MotorValue = map(Sensor1, 0, 560, 0, 255);
    
    
    analogWrite(MotorOutputPin, MotorValue);  // set motor speed to MotorValue

     //Print time since Arduino startup in Milliseconds (Can be set to microseconds if 'time' is changed)
     Serial.print(time);
     Serial.print("\t"); 
     // print the sensor data and accellerometer values:
     Serial.print(Sensor1);
     Serial.print("\t");
     //Serial.print(Sensor2);
     //Serial.print("\t");
     Serial.print(AccX);
     Serial.print("\t");
     Serial.print(AccY);
     Serial.print("\t");
     Serial.print(AccZ);
     Serial.print("\t");
     Serial.println(MotorValue);
     
    //This is for calculating the amount of time it takes for the loop to complete
    //progEND = millis();

    // While button is pressed the ET will print Column headers to label data.
    // Illuminate the Green LED while the button is pressed
    while(!digitalRead(ButtonPin))
    {
      digitalWrite(GreenLED, LEDON);

      // print the column headers to label data:
      Serial.print("Time");
      Serial.print("\t"); 
      Serial.print("Long Range");
      Serial.print("\t");
      //Serial.print("Short Range");
      //Serial.print("\t");
      Serial.print("AccX");
      Serial.print("\t");
      Serial.print("AccY");
      Serial.print("\t");
      Serial.print("AccZ");
      Serial.print("\t");
      Serial.println("Motor Speed");

      // Longer delay just to keep data cleaner
      delay(500);
      digitalWrite(GreenLED, LEDOFF);

      // This will reset the experiment time with a button press
      //MillisREC = millis();
      //progRUNTIME = 0;  // Reset ET runtime clock to re-enter while loop.

    }

    progRUNTIME = (millis()- MillisREC); // Calulate total time ET has been running unperterbed 
  } 

  /*
   Once the ET has finished to unperturbed run you can use this section to change the behavior of the 
   ET so. You can simply copy the code above and modify to fit experimental purposes. By default
   the original version of this code will set the motor speed to 0. It will continue to print data in
   the format above with an additional column to indicate that this section of code has begun.
  */
  
 analogWrite(MotorOutputPin, 0); //Turn motor off
    
 time = millis();  // Record time in milliseconds.
 // Microseconds will turn over 70 minutes after the Arduino. starts up. Milliseconds turns over after 50 days. 
  
 // Begin Sensor readings
 // Set delay to fit desired sample rate delay may be set as low as 10 microseconds but baud rate limits 
 // total possible samples. At 1 millisecond between each sensor read sample rate is about 250htz
 Sensor1 = analogRead(Sensor1Pin);
 delay(1);
 Sensor2 = analogRead(Sensor2Pin);
 delay(1);
 AccX = analogRead(AccXPin);
 delay(1);
 AccY = analogRead(AccYPin);
 delay(1);
 AccZ = analogRead(AccZPin);
 
 //Print time since Arduino startup in Milliseconds (Can be set to microseconds if 'time' is changed)
 Serial.print(time);
 Serial.print("\t"); 
 // print the sensor data and accellerometer values:
 Serial.print(Sensor1);
 Serial.print("\t");
 //Serial.print(Sensor2);
 //Serial.print("\t");
 Serial.print(AccX);
 Serial.print("\t");
 Serial.print(AccY);
 Serial.print("\t");
 Serial.print(AccZ);
 Serial.print("\t");
 Serial.print(MotorValue); 
 Serial.print("\t");
 Serial.println("**"); 

 delay(2000);  //Set a delay before begining loop again. Be aware of startDELAY time at begining of loop
 
 //MillisREC = millis();  // Reset time for returning to While loop. This is for times when ET behavior is perterbed
 //ProgRUNTIME = 0;  // Reset ET runtime clock to re-enter while loop. 

}
/*
   //Use this if you want button to provide auditory distance information.
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
 */
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


