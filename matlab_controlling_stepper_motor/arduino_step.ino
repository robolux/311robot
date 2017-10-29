/*
Hunter Phillips -- 10/28/2017
Receiving commands for stepper motor
*/

char user_input;
int matlabData;


void setup() 
{
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  Serial.begin(600);
}
 
void loop() {
  
   if(Serial.available()>0) // if there is data to read
   {
    matlabData=Serial.read(); // read data

    switch(matlabData) {
      case 1: {
        test123();
        break;
      }
    }
  }
}

void test123()
{
    digitalWrite(9, HIGH);
    delay(1);          
    digitalWrite(9, LOW); 
    delay(1);    
}
