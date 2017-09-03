/*
Hunter Phillips -- 08/30/2017
Matlab com with arduino blinking LED
*/

int ledPinBlue = 10;
int ledPinRed = 11;
int ledPinGreen = 12;
const int trigPin = 5;
const int echoPin = 6;
int matlabData;
float f=-5.125345345;
long duration;
float distance;
void setup() 
{
  pinMode(ledPinRed,OUTPUT);
  pinMode(ledPinGreen,OUTPUT);
  pinMode(ledPinBlue,OUTPUT);    
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(600);
}
 
void loop() 
{
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   // Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(echoPin, HIGH);
   // Calculating the distance
   distance= duration*0.034/2;
   
   serialFloatPrint(distance);
   Serial.println();

   if(Serial.available()>0) // if there is data to read
   {
    matlabData=Serial.read(); // read data

    switch(matlabData) {
      case 1: {
        digitalWrite(ledPinBlue,HIGH);
        digitalWrite(ledPinGreen,LOW);
        digitalWrite(ledPinRed,LOW);
        break;
      }
      case 2:{
        digitalWrite(ledPinBlue,LOW);
        digitalWrite(ledPinGreen,HIGH);
        digitalWrite(ledPinRed,LOW);
        break;
      }    
      case 3: {
        digitalWrite(ledPinBlue,LOW);
        digitalWrite(ledPinGreen,LOW);
        digitalWrite(ledPinRed,HIGH);
      }
    }

    /*
    if(matlabData==1)
      digitalWrite(ledPin,HIGH); // turn light on
    else if(matlabData==2)
      digitalWrite(ledPin,LOW); // turn light off
    */
  }
}

void serialFloatPrint(float f) {
  byte * b = (byte *) &f;
  Serial.print("f:");
  for(int i=0; i<4; i++) {
    
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
    
    Serial.print(c1);
    Serial.print(c2);
  }
}
