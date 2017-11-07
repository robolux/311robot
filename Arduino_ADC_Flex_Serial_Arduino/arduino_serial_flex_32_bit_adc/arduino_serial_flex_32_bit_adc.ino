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



void setup() 
{
  // initalize the  data ready and chip select pins:
  pinMode(ADS1262_DRDY_PIN, INPUT);                  //data ready input line
  pinMode(ADS1262_CS_PIN, OUTPUT);                   //chip enable output line
  pinMode(ADS1262_START_PIN, OUTPUT);               // start 
  pinMode(ADS1262_PWDN_PIN, OUTPUT);                // Power down output   

  Serial.begin(9600);
  //initalize ADS1292 slave
  PC_ADS1262.ads1262_Init();                      // initialise ads1262
  Serial.println("ads1262 Initialised successfully....");

 }

void loop() 
{
  volatile int i,data;

  
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
     //Serial.println(uads1262Count);
     //Serial.println(sads1262Count);
     resolution = (double)((double)VREF/pow(2,31));       //resolution= Vref/(2^n-1) , Vref=2.5, n=no of bits
    // Serial.print(resolution,15);
     volt_V      = (resolution)*(float)sads1262Count;     // voltage = resolution * adc count
     volt_mV   =   volt_V*1000;                           // voltage in mV
     Serial.print("Voltage");
     Serial.print(" : ");
     Serial.print(volt_V,7);
     Serial.println(" V");
     
     float flexR = R_DIV * (VCC / volt_V - 1.0);
     Serial.print("Resistance");
     Serial.print(" : ");
     Serial.print(flexR,7);
     Serial.println(" ohms");

     // Use the calculated resistance to estimate the sensor's
     // bend angle:
     
     float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                      0, 90.0);
     Serial.print("Angle");
     Serial.print(" : ");
     Serial.print(angle,7);
     Serial.println(" degrees");
     Serial.println("");
 
   }
    
  SPI_RX_Buff_Count = 0;
  delay(1000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}