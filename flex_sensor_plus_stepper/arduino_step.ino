/*
Hunter Phillips -- 10/28/2017
Receiving commands for stepper motor
*/

char user_input;
int matlabData;


void setup() 
{
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(8, LOW);
  digitalWrite(A0,0);
  digitalWrite(A1,0);
  digitalWrite(A2,0);
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
    digitalWrite(8, HIGH);
    delay(1);          
    digitalWrite(8,LOW); 
    delay(1);    
}
