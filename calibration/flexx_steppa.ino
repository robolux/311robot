/* Hunter Phillips
   09/09/2017	
----------------------
|ads1262 pin label| Pin Function         |Arduino Connection|
|-----------------|:--------------------:|-----------------:|
| DRDY            | Data ready Output pin|  D6              |             
| MISO            | Slave Out            |  D12             |
| MOSI            | Slave In             |  D11             |
| SCLK            | Serial Clock         |  D13             |
| CS              | Chip Select          |  D7              |
| START           | Start Conversion     |  D5              | 
| PWDN            | Power Down/Reset     |  D4              |
| DVDD            | Digital VDD          |  +5V             |
| DGND            | Digital Gnd          |  Gnd             |
| AN0             | Analog Input         |  Flex Sensor     |
| AINCOM          | Analog input common  |                  |
| AVDD            | Analog VDD           |  -               |
| AGND            | Analog Gnd           |  -               |
-----------------------------------------------------------------
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
const float VCC = 5.042; // Measured voltage of Ardunio 5V line
const float R_DIV = 46580.0; // Measured resistance of 3.3k resistor
const float STRAIGHT_RESISTANCE = 13213.39; // resistance when straight
const float BEND_RESISTANCE = 47393.18; // resistance at 90 deg
bool direct_step = HIGH;


void setup() 
{
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(2, direct_step);
  digitalWrite(8, LOW);
  digitalWrite(A0,0);
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  // initalize the  data ready and chip select pins:
  pinMode(ADS1262_DRDY_PIN, INPUT);                  //data ready input line
  pinMode(ADS1262_CS_PIN, OUTPUT);                   //chip enable output line
  pinMode(ADS1262_START_PIN, OUTPUT);               // start 
  pinMode(ADS1262_PWDN_PIN, OUTPUT);                // Power down output   

  Serial.begin(1200);
  //initalize ADS1292 slave
  PC_ADS1262.ads1262_Init();                      // initialise ads1262
  //Serial.println("ads1262 Initialised successfully....");

 }

void loop() 
{
  volatile int i,data;

  if(Serial.available()>0) // if there is data to read
   {
    matlabData=Serial.read(); // read data

    switch(matlabData) {
      case 1: {
        full_step();
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
     sads1262Count = (signed long) (uads1262Count);      // get signed value
     resolution = (double)((double)VREF/pow(2,31));       //resolution= Vref/(2^n-1) , Vref=2.5, n=no of bits
     volt_V      = (resolution)*(float)sads1262Count;     // voltage = resolution * adc count
     
   	 serialFloatPrint(volt_V);
   	 Serial.println();
 
   }
    
  SPI_RX_Buff_Count = 0;
  //delay(1000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

void full_step()
{
    digitalWrite(8, HIGH);
    delay(1);          
    digitalWrite(8,LOW); 
    delay(1);    
}
