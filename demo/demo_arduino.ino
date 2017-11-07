/* Hunter Phillips
   311 Robot Project - Group 1
   09/09/2017	
   Modified Heavily 11/07/2017
*/

#include <SPI.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <ads1262.h>

#define PGA 1                     // Programmable Gain = 1
#define VREF 5.00                 // Internal reference of 2.048V
#define VFSR VREF/PGA             
#define FSR (((long int)1<<23)-1)  

ads1262 PC_ADS1262;                     // class

char user_input;
int matlabData;

float volt_V=0;
float volt_mV=0;
volatile int i;
volatile char SPI_RX_Buff[10];
volatile long ads1262_rx_Data[10];
volatile static int SPI_RX_Buff_Count = 0;
volatile char *SPI_RX_Buff_Ptr;
volatile int Responsebyte = false;
volatile signed long sads1262Count = 0;
volatile signed long uads1262Count = 0;
double resolution;
// const float VCC = 4.096 // Measured voltage of Ardunio 5V line
// const float R_DIV = 46880.0; // Measured resistance of 47k resistor
// const float STRAIGHT_RESISTANCE = 13213.39; // resistance when straight
// const float BEND_RESISTANCE = 47393.18; // resistance at 90 deg
bool direct_step = HIGH;


void setup() 
{
  pinMode(2, OUTPUT);   // DIR pin on stepper motor driver
  pinMode(8, OUTPUT);   // STEP pin on stepper motor driver
  pinMode(A0, OUTPUT);  // MS1
  pinMode(A1, OUTPUT);  // MS2
  pinMode(A2, OUTPUT);  // MS3
  digitalWrite(2, direct_step); // set the direction to the init state of high
  digitalWrite(8, LOW);         
  digitalWrite(A0,0);           // these pins all driven LOW will cause a full-step mode to activate
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  // initalize the  data ready and chip select pins:
  pinMode(ADS1262_DRDY_PIN, INPUT);                  // data ready input line
  pinMode(ADS1262_CS_PIN, OUTPUT);                   // chip enable output line
  pinMode(ADS1262_START_PIN, OUTPUT);                // start 
  pinMode(ADS1262_PWDN_PIN, OUTPUT);                 // Power down output   

  Serial.begin(1200);
  // initalize ADS1292 slave
  PC_ADS1262.ads1262_Init();                         // initialise ads1262
 }

void loop() 
{
  volatile int i,data; // init data for 32-bit adc

  if(Serial.available()>0) // if there is data to read
   {
    matlabData=Serial.read(); // read data

    switch(matlabData) {      
      case 1: {               // if user returns a value of 1
        full_step();          // move one step
        break;
      }
      }
    }
  

  
 if((digitalRead(ADS1262_DRDY_PIN)) == LOW)               // monitor Data ready(DRDY pin)
  {  
    SPI_RX_Buff_Ptr = PC_ADS1262.ads1262_Read_Data();      // read 6 bytes conversion register
    Responsebyte = true ; 
  }

  if(Responsebyte == true)
  {
    for(i = 0; i <5; i++)
    {
      SPI_RX_Buff[SPI_RX_Buff_Count++] = *(SPI_RX_Buff_Ptr + i);              
    }
    Responsebyte = false;
  }
  
  if(SPI_RX_Buff_Count >= 5)
  {     


    ads1262_rx_Data[0]= (unsigned char)SPI_RX_Buff[1];  // read 4 bytes adc count
    ads1262_rx_Data[1]= (unsigned char)SPI_RX_Buff[2];
    ads1262_rx_Data[2]= (unsigned char)SPI_RX_Buff[3];
    ads1262_rx_Data[3]= (unsigned char)SPI_RX_Buff[4];

 
     uads1262Count = (signed long) (((unsigned long)ads1262_rx_Data[0]<<24)|((unsigned long)ads1262_rx_Data[1]<<16)|(ads1262_rx_Data[2]<<8)|ads1262_rx_Data[3]);//get the raw 32-bit adc count out by shifting
     sads1262Count = (signed long) (uads1262Count);       // get signed value
     resolution = (double)((double)VREF/pow(2,31));       // resolution= Vref/(2^n-1) , Vref=2.5, n=no of bits
     volt_V      = (resolution)*(float)sads1262Count;     // voltage = (resolution)(adc count)
     
   	 serialFloatPrint(volt_V); // call function to print ADC reading as a parsed float
   	 Serial.println();
 
   }
    
  SPI_RX_Buff_Count = 0;
  //delay(1000);
}

void serialFloatPrint(float f) {
  byte * b = (byte *) &f;
  Serial.print("f:");                                     // print delimiter
  for(int i=0; i<4; i++) {
    
    byte b1 = (b[i] >> 4) & 0x0f;                         // split the float chain into a 4 byte partioned chain
    byte b2 = (b[i] & 0x0f);                              // prepends the upper nibble of a byte
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;     // encode the byte process in a standard IEEE HEX Representation
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;     // prevents incorrect matlab readings due to delimeter tainting
    
    Serial.print(c1);                                     // prints the encoded hex to the serial line
    Serial.print(c2);
  }
}

void full_step()
{
    digitalWrite(8, HIGH); // stepper motor transition sequence for a full step
    delay(1);          
    digitalWrite(8,LOW); 
    delay(1);    
}
